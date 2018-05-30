#define SPI_MSG_PAYLOAD_LEN 32
//Queue is 16 so that all seq and ack values are valid indexes
#define SPI_MSG_QUEUE_SIZE 16

#ifdef CPP
extern "C" {
#endif

#define SPI_PROTO_MAGIC_REAL 0xa6
#define SPI_PROTO_MAGIC_FILLER 0x54
struct __attribute__((packed)) spi_packet {
	//TODO add version number
	uint8_t magic; // includes at least a version number
	uint8_t seq : 4;
	uint8_t preack : 4; //or "anticipatory ack", or "expected ack"
	uint8_t msg[SPI_MSG_PAYLOAD_LEN];
	uint16_t crc; // TODO confirm byte order on wire here
};

#define SPI_PACKET_LEN sizeof(struct spi_packet)

//basic rule is send the message seq # 1 after your last received ack, which is really "expected". If you send n and the message you receive simultaneously is "I expect n this round" everything is good move on
//corrected: if the sent seq # and the received "expected ack" # for a round are the same, sent the next seq number. If they are not, send the "expected ack" number

//there can't ever be more than 16 messages waiting in the queue

/*
Queue with various indices
 [--------------------------]
   |   |   |
   |   |   +first_avail_seq
   |   +first_unsent_seq
   + first_unconfirmed_seq

spaces before first_unconfirmed_seq and after first_avail_seq are open

*/
struct spi_state {
	//each side has its own
	uint8_t our_seq, our_next_preack;
	
	uint8_t we_sent_seq, we_sent_preack; // this side's this round sendings (same as packet we're analyzing in receive_msg)
	
	uint8_t our_prev_sent_seq, our_prev_sent_preack; // this side's last round sendings
	
	//for received values
	uint8_t last_round_rcvd_seq, last_round_rcvd_preack;

	//bookkeeping
	uint8_t first_unconfirmed_seq;
	
	uint8_t first_unsent_seq;
	
	uint8_t first_avail_seq;
	
	//logging of sorts
	int num_sent_successfully;
	int num_received_successfully;
	
	//occupancy controls
	uint8_t num_unsent;
	uint8_t num_sent_but_unconfirmed;
	uint8_t num_avail;
	
	
	struct spi_packet queue[SPI_MSG_QUEUE_SIZE];
};

typedef void (*spi_msg_callback_t)(struct spi_packet *);

void
spi_proto_initialize(struct spi_state *s);
int
spi_proto_check_invariants(struct spi_state *s);
void
print_spi_state(struct spi_state *s);
void
print_spi_state_full(struct spi_state *s);
void
print_spi_packet(struct spi_packet *p);
void
print_spi_occs(struct spi_state *p);

void
spi_proto_rcv_msg(struct spi_state *s, struct spi_packet *p, spi_msg_callback_t cb);
int
spi_proto_prep_msg(struct spi_state *s, void *buf, size_t n);
int
spi_proto_send_msg(struct spi_state *s, void *buf, size_t n);

uint16_t
spi_msg_crc(struct spi_packet *p);

#ifdef CPP
} // extern C
#endif
