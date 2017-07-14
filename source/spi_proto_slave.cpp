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
#include "spi_proto_slave.h"
#include "solenoid.h"

namespace spi_proto {

//this is a simple protocol, that doesn't support arbitrary length messages
int
slave_get_message(struct spi_proto &p, unsigned char *buf, int len)
{
	//TODO parses the message and does any required processing

	/* //for debugging SPI
	for (int i = 0; i < len; i++) {
		PRINTF("%02x ", buf[i]);
	}
	PRINTF("\r\n");
	*/
	//if it's heartrate,
	//led_delay_time = 0.5/(((float)slaveReceiveBuffer[0]) * (1.0/60.0) * 0.001);
	led_delay_time = 0.5/(((float)(p.getbuf[0])) * (1.0/60.0) * 0.001);
	//second 1/2 is because rate counts inhales and exhales
	breath_delay_time = 0.5 * 0.5/(((float)(p.getbuf[1])) * (1.0/60.0) * 0.001);

	//tourniquet is second byte
	if (p.getbuf[2]){
		//set tourniquet on, so stop bleeding
		tourniquet_on = true;
	}

	//TODO make this control solenoid stuff
	return 0;

}

int
slave_send_message(struct spi_proto &p, unsigned char *buf, int len)
{
	//TODO assert len less than max spi msg len

	msg m;
	//TODO but buf and len into m
	if (len <= TRANSFER_SIZE) { // TODO remove magic number (it's TRANSFER_SIZE, for reference)
		//TODO remove extra copy
		memcpy(m.buf, buf, len);
		m.len = len;
		push(p.queue, m);
		return 0;
	}

}

//do the things the normal interrupt handler did
//int
//slave_handle_spi_interrupt(){}

int
slave_do_tick(struct spi_proto &p)
{
	//handles once-per-message-cycle events such as moving data in and out of buffers.
	if (p.queue.occupancy) {
		//pop message and push into outgoing buffer to be sent
		msg m;
		pop(p.queue, m);
		if (p.buflen >= m.len) {
			//enough space, we can proceed
			memcpy(p.sendbuf, &m.buf, m.len);
			//not copying len because it's a buffer
			//TODO need additional lengths so that spi_proto knows how many bytes of message contain real info
			return 0;
		}
		return 1;

	} else {
		//no messages in queue, do nothing
		return 0;
	}
}

//PRE both parameters not null, all messages in q have valid lengths
int
pop(spi_proto::msg_queue &q, struct msg &m)
{
	//TODO
	if (!q.occupancy) return -1;
	//TODO lock
	//copy data
	memcpy(m.buf, q.que[q.first_full].buf, q.que[q.first_full].len);
	m.len = q.que[q.first_full].len;
	//advance first_full, wrap it
	q.first_full++;
	q.first_full %= q.capacity;
	//decrement occupancy
	q.occupancy--;
	//TODO unlock
	return 0;
}
int
push(spi_proto::msg_queue &q, struct msg &m)
{
	//TODO
	if (!(q.occupancy < q.capacity)) return -1;
	//TODO lock
	//check data length validity? no, precondition. TODO more principled look at message length management
	//copy data
	memcpy(q.que[q.first_empty].buf, m.buf, m.len);
	q.que[q.first_empty].len = m.len;
	//advance first_empty, wrap it
	q.first_empty++;
	q.first_empty %= q.capacity;
	//increment occupancy
	q.occupancy++;
	//TODO unlock
	return 0;
}

int
reset(spi_proto::msg_queue &q)
{
	q.capacity = 10;
	q.first_empty = 0;
	q.first_full = 0;
	q.occupancy = 0;
	return 0;
}
}
