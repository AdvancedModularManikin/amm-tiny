/*
 * spi_proto_slave.c
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */
#include "stdint.h"
#include <cstring>

#include "fsl_debug_console.h"

#include "FreeRTOS.h" // required for heartrate.h

#include "heartrate.h"
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "solenoid.h"

//to toggle the led TODO remove becuase it's a debugging feature
#include "board.h"

namespace spi_proto {

//this is a simple protocol, that doesn't support arbitrary length messages
//TODO this contains seattle demo code fold it into the rewritten receive function
void
slave_get_message(struct spi_packet *p)
{
	//TODO parses the message and does any required processing

	/* //for debugging SPI
	for (int i = 0; i < len; i++) {
		PRINTF("%02x ", buf[i]);
	}
	PRINTF("\r\n");
	//*/
	//if it's heartrate,
	//led_delay_time = 0.5/(((float)slaveReceiveBuffer[0]) * (1.0/60.0) * 0.001);
	heart_rate = p->msg[0];
	heart_delay_time = 0.5/(((float)(p->msg[0])) * (1.0/60.0) * 0.001);
	//second 1/2 is because rate counts inhales and exhales
	breath_rate = p->msg[1];
	breath_delay_time = 0.5 * 0.5/(((float)(p->msg[1])) * (1.0/60.0) * 0.001);
	//tourniquet is second byte
	if (p->msg[2]){
		//set tourniquet on, so stop bleeding
		tourniquet_on = true;
	}
	if (p->msg[3]) {
		//start bleeding
		hemorrhage_enabled = true;
	}

	//TODO make this control solenoid stuff
	//return 0;

}

int
slave_send_message(struct slave_spi_proto &p, unsigned char *buf, int len)
{
	return spi_proto_send_msg(&p.proto, buf, len);
}

//do the things the normal interrupt handler did
//int
//slave_handle_spi_interrupt(){}

int
slave_do_tick(struct slave_spi_proto &p)
{
	//handles once-per-message-cycle events such as moving data in and out of buffers.

	spi_proto_prep_msg(&p.proto, p.sendbuf, TRANSFER_SIZE);
	return 0;
}

void
spi_proto_echo(struct spi_packet *pack)
{
	//TODO send the message back
	spi_proto_send_msg(&spi_proto::p.proto, pack->msg, SPI_MSG_PAYLOAD_LEN);
}

int
slave_spi_proto_rcv_msg(struct slave_spi_proto &p, unsigned char *buf, int len)
{
	//parse an spi_packet out of getbuf

	//TODO confirm that any values larger than 16 bits are appropriately encoded to network order
	if (len < sizeof(struct spi_packet)) return -1;
	struct spi_packet pack = *((struct spi_packet *)buf);

	spi_msg_callback_t seattle_msg_callback = &slave_get_message;//spi_proto_echo;
	spi_proto_rcv_msg(&p.proto, &pack, seattle_msg_callback);
	return 0;
}

void
spi_proto_slave_initialize(struct slave_spi_proto *s)
{
	//TODO initialize the rest of slave_spi_proto (buffers, length)
	spi_proto_initialize(&s->proto);
}
}
