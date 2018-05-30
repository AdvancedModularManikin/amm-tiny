#include <string.h>
/*
#include <stdint.h>
#include <stdio.h>
*/
#ifdef CPP
extern "C" {
#endif
#include "config.h"
#include "misc/crc16.h"
#ifdef CPP
}
#endif

#include "spi_proto.h"

//TODO this was made CPP to fix a linker error, instead determine what was causing the cross-language linking to fail and fix that so this can be pure C

//update the protocol state with a message. Doesn't do any processing of the message itselves
#ifdef CPP
extern "C" {
#endif
void
spi_proto_rcv_msg(struct spi_state *s, struct spi_packet *p, spi_msg_callback_t f)
{
	uint16_t rcvd_crc = p->crc;
	uint16_t calc_crc = spi_msg_crc(p);
#ifdef DEBUG_SPI_PROTO
	printf("rcvd_crc: 0x%04x\n", rcvd_crc);
	printf("calc_crc: 0x%04x\n", calc_crc);
	print_spi_state(s);
	printf("p->seq: %d\n", p->seq);
#endif
	//validate checksum. If it's wrong, take no action. The lack of change in our ACK will be the signal the other side needs.
	//if the checksum is correct, verify that p->seq == s->their_next_ack. If so, increase their_next_ack. If not, take no action, they will resend.
	if (rcvd_crc == calc_crc) {
		if (p->seq == s->we_sent_preack) { // the seq they sent, vs the preack we sent
			s->our_next_preack++;
			s->our_next_preack %= 16; // this isn't really a magic number because bytes aren't changing size anytime soon
			//process it
			if (p->magic == SPI_PROTO_MAGIC_REAL) {
				s->num_received_successfully++;
				if (f)
					f(p);
				else {
					//can't take an action
	#ifdef DEBUG_SPI_PROTO
					printf("%s: no function provided!\n", __func__);
	#endif
				}
			}
		} else {
			//TODO CONFIRM probably nothing, but how to handle a desync? is such a desync even possible?
			//take no action on this side. Either our message was sent correctly (so the other side knows to change) or it wasn't, so they don't have any information about what we expect but they can learn later. XXX possible optimization would be to revert to last-known-expected if an incoming message is garbled, look at that later
		}

		// if this round we got N preack and last round we got N-1, mark N-1 as confirmed
#ifdef DEBUG_SPI_PROTO
#define PRINTIT(x) printf( #x ": %d\n", x)
		PRINTIT(s->our_prev_sent_seq);
		PRINTIT(s->last_round_rcvd_preack);
		printf("last_round_rcvd_preack + 1 = %d\n",(s->last_round_rcvd_preack + 1)%16);
		PRINTIT(p->preack);
#undef PRINTIT
#endif
		if ( (s->our_prev_sent_seq == s->last_round_rcvd_preack)
			&& ((s->last_round_rcvd_preack + 1)%16) == p->preack) {
			// mark s->last_round_rcvd_preack as confirmed and those before it in the sent_but_unconfirmed section
			// a position P is in a range of the queue if the start S of the queue slice and the length L of the queue slice include it, so if S + k = P mod 16 and 0 <= k <= L
			//TODO improve this bad algorithm (written while tired)
#ifdef DEBUG_SPI_PROTO
			puts ("case triggered");
#endif
			s->num_sent_but_unconfirmed--;
			s->num_avail++;
			s->num_sent_successfully++;
			s->first_unconfirmed_seq++;
		}
		if (p->preack == s->we_sent_seq) {
			//the anticipatory ack was the same as our send, so assume send was successful and increment. If it wasn't we'll find out next completed round
			//send the next packet (by incrementing seq which changes the index)
			//TODO mark this location as confirmed phase 1 also
			//s->waiting_for_confirm = p->preack;
			//TODO moved this to prep_msg. erase when shown correct
			//s->our_seq++;
			//s->our_seq %= 16;
		} else {
			//reset our send counter to the most recent received "expected ack"
			//next sent is first_unsent so resend it, like go-back-n
			//TODO determine if a more appropriate response is needed
			//TODO print these out and check validity
			//TODO find difference between s->first_unsent_seq = s->first_unconfirmed_seq, subtract from num_unconfirmed, add to num_unent
			int diff;
			if (s->first_unsent_seq < s->first_unconfirmed_seq) {
				diff = s->first_unsent_seq + 16 - s->first_unconfirmed_seq;
			} else {
				diff = s->first_unsent_seq - s->first_unconfirmed_seq;
			}
#ifdef DEBUG_SPI_PROTO
			printf("diff: %d %d -> %d\n", s->first_unsent_seq, s->first_unconfirmed_seq, diff);
#endif
			s->num_unsent += diff;
			s->num_sent_but_unconfirmed -= diff;
			//TODO assert s->num_sent_but_unconfirmed >= 0
			s->first_unsent_seq = s->first_unconfirmed_seq;
			s->our_seq = s->first_unsent_seq;
#ifdef DEBUG_SPI_PROTO
			if (p->preack != s->our_seq) {
				puts("first_unconfirmed_seq was out of sync with counterpart's preack");
			}
#endif
			//this will either not move the line of messages that must be kept forward, or move it forward by one
		}
		//handle releasing messages that were being held for resending. Release those with confirmed receipt. This is done by freeing that space in the message queue.
		//this means release held messages before this p->preack. Write a loop, but it will only ever clear one at a time I think.
		
		//no queue modification necessary, just modify the variables holding the information about the queue.
		
		
		//set up last_round values for future use
		s->last_round_rcvd_seq = p->seq;
		s->last_round_rcvd_preack = p->preack;
		s->our_prev_sent_seq = s->we_sent_seq;
		s->our_prev_sent_preack = s->we_sent_preack;
		//done bookkeeping. don't process message because it's not the right time
	} else {
		//no action. XXX possibly increment send counter but seems likely we will have to go back anyway so don't
#ifdef DEBUG_SPI_PROTO
		puts("bad crc!");
#endif
	}
}

int
spi_proto_prep_msg(struct spi_state *s, void *buf, size_t n)
{
	//give it the buffer to write the message into
	//TODO make #defs for return values
	if (n < SPI_PACKET_LEN) return -1;
	//if no message just write all zeros but still write the SEQ/ACK/CRC
	//do the seq/ack/crc insertion in this function
	struct spi_packet *pack;
	struct spi_packet p; // possibly not used
	if (s->num_unsent) {
		//responsibility of receive_msg to control these
		s->queue[s->first_unsent_seq].seq = s->our_seq;
		s->we_sent_seq = s->our_seq;
		s->queue[s->first_unsent_seq].preack = s->our_next_preack;
		s->we_sent_preack = s->our_next_preack;
		
		s->queue[s->first_unsent_seq].magic = SPI_PROTO_MAGIC_REAL;
		pack = &s->queue[s->first_unsent_seq];
		
		//TODO maybe bump seq?
		s->queue[s->first_unsent_seq].crc = spi_msg_crc(&s->queue[s->first_unsent_seq]);
		s->first_unsent_seq++;
		s->first_unsent_seq %= 16;
		s->our_seq++;
		s->our_seq %= 16;
		s->num_unsent--;
		s->num_sent_but_unconfirmed++;
	} else {
		//safe to send seq here because of s->first_unsent_seq
		//still need to update our own sending stuff
		memset(&p,0,sizeof(struct spi_packet));
		p.seq = s->our_seq;
		p.preack = s->our_next_preack;
		p.magic = SPI_PROTO_MAGIC_FILLER;
		p.crc = spi_msg_crc(&p);
		pack = &p;
		
		s->we_sent_seq = s->our_seq;
		s->we_sent_preack = s->our_next_preack;
		s->our_seq++;
		s->our_seq %= 16;
	}
	memset(buf, 0, SPI_PACKET_LEN);
	memcpy(buf, pack, SPI_PACKET_LEN);
	return 0;
}

int
spi_proto_send_msg(struct spi_state *s, void *buf, size_t n)
{
	//this puts the message in the queue, or returns -1
	//TODO add #defs for return values
	if (n > SPI_MSG_PAYLOAD_LEN) return -1;
	
	//no need to modify other fields of message, that is handled when it's about to be sent
	if (s->num_avail) {
		//copy to buffer, maintain invariants
		memset(s->queue[s->first_avail_seq].msg, 0, SPI_MSG_PAYLOAD_LEN);
		memcpy(&s->queue[s->first_avail_seq].msg, buf, n);
		s->first_avail_seq++;
		s->first_avail_seq %= 16;
		s->num_avail--;
		s->num_unsent++;
		return 0;
	} else {
		return -2;
	}
}

void
spi_proto_initialize(struct spi_state *s)
{
	//most of these aren't necessary, but perhaps they should be present for a "reinitialize"

	memset(s, 0, sizeof(struct spi_state));
	s->num_avail = SPI_MSG_QUEUE_SIZE;
	/*
	s->num_unsent = 0;
	s->num_sent_but_unconfirmed = 0;
	
	s->oldest_unconfirmed_seq = 0;
	s->first_unconfirmed_seq = 0;
	s->first_unsent_seq = 0;
	s->first_avail_seq = 0;
	
	
	s->our_seq = 0;
	s->our_next_preack = 0;
	s->we_sent_seq = 0;
	s->we_sent_preack = 0;
	//*/
}
#ifdef DEBUG_SPI_PROTO
void
print_spi_state(struct spi_state *s)
{
#define PRINTIT(x) printf( #x ": %d\n", s-> x)
	if (s) {
		printf("our_seq: %d\n", s->our_seq);
		printf("our_next_preack: %d\n", s->our_next_preack);
	
		PRINTIT(we_sent_seq);
		PRINTIT(we_sent_preack);
		
		PRINTIT(last_round_rcvd_seq);
		PRINTIT(last_round_rcvd_preack);
		PRINTIT(num_sent_successfully);
	
		PRINTIT(first_unconfirmed_seq);
		PRINTIT(first_unsent_seq);
		PRINTIT(first_avail_seq);
		PRINTIT(num_unsent);
		PRINTIT(num_sent_but_unconfirmed);
		PRINTIT(num_avail);
		
	} else {
		puts("spi_state NULL!");
	}
#undef PRINTIT
}

void
print_spi_occs(struct spi_state *s)
{
#define PRINTIT(x) printf( #x ": %d\n", s-> x)
	PRINTIT(num_unsent);
	PRINTIT(num_sent_but_unconfirmed);
	PRINTIT(num_avail);
#undef PRINTIT
}

void
print_spi_state_full(struct spi_state *s)
{
	return;
	print_spi_state(s);
	if (s) {
		//TODO print out spi queue
		for (unsigned int j = 0; j < SPI_MSG_QUEUE_SIZE; j++) {
			print_spi_packet(&s->queue[j]);
		}
	}
}

void
print_spi_packet(struct spi_packet *p)
{
	return;
	unsigned char *pp = (unsigned char *) p;
	for (unsigned int i = 0; i < SPI_PACKET_LEN;i++) {
		printf("0x%02x ",pp[i]);
	}
	printf("\n");
}
#endif

int
spi_proto_check_invariants(struct spi_state *s)
{
	//TODO give seperate return codes, ensure this is all invariants
	if (!s) return -1;
	
	if (s->we_sent_seq > 16) return -2;
	if (s->we_sent_preack > 16) return -2;
	if (s->our_seq > 16) return -2;
	if (s->our_next_preack > 16) return -2;
	
	if ((s->num_avail + s->num_unsent + s->num_sent_but_unconfirmed) != 16)
		return -3;
	
	return 0;
}

uint16_t
spi_msg_crc(struct spi_packet *p)
{
	//run crc16 over all of message except for last two bytes (which are crc)
	uint16_t crc_res = crc16_block(0, (uint8_t *) p, sizeof (struct spi_packet) - 2);
	//p->crc = crc_res;
	return crc_res;
}
#ifdef CPP
} // extern C
#endif
