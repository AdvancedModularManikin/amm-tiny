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
extern "C" {
#include "spi_proto.h"
#include "spi_chunks.h"
}
#include "spi_proto_slave.h"
#include "spi_chunks_slave.h"


namespace spi_proto {

//this is a simple protocol, that doesn't support arbitrary length messages

int
slave_send_message(struct slave_spi_proto &p, unsigned char *buf, unsigned int len)
{
	return spi_proto_send_msg(&p.proto, buf, len);
}

int
slave_do_tick(struct slave_spi_proto &p)
{
	//handles once-per-message-cycle events such as moving data in and out of buffers.

	spi_proto_prep_msg(&p.proto, p.sendbuf, TRANSFER_SIZE);
	return 0;
}

//sometimes useful for debugging
void
spi_proto_echo(struct spi_packet *pack)
{
	spi_proto_send_msg(&spi_proto::p.proto, pack->msg, SPI_MSG_PAYLOAD_LEN);
}

int
slave_spi_proto_rcv_msg(struct slave_spi_proto &p, unsigned char *buf, unsigned int len)
{
	//parse an spi_packet out of getbuf

	if (len < sizeof(struct spi_packet)) return -1;
	struct spi_packet pack = *((struct spi_packet *)buf);

	spi_proto_rcv_msg(&p.proto, &pack, p.spi_cb);
	return 0;
}

void
spi_proto_slave_initialize(struct slave_spi_proto *s)
{
  //only s->proto has meaningul init but may as well init the whole struct
  memset(s, 0, sizeof(struct slave_spi_proto));
	spi_proto_initialize(&s->proto);
}
}
