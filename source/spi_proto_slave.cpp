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
#include "mule-1/air_tank.h"

//to toggle the led TODO remove becuase it's a debugging feature
#include "board.h"

namespace spi_proto {

//this is a simple protocol, that doesn't support arbitrary length messages
//TODO this contains seattle demo code fold it into the rewritten receive function
void
slave_get_message(struct spi_packet *p)
{
	//TODO handle mule 1 stuff
	//receive pressure message (float)
	memcpy(&operating_pressure, &p->msg[4], 4);
}

int
slave_send_message(struct slave_spi_proto &p, unsigned char *buf, unsigned int len)
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
slave_spi_proto_rcv_msg(struct slave_spi_proto &p, unsigned char *buf, unsigned int len)
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
