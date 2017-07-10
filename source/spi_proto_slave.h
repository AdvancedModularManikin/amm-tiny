/*
 * spi_proto_slave.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_SPI_PROTO_SLAVE_HPP_
#define SOURCE_SPI_PROTO_SLAVE_HPP_
namespace spi_proto {

struct msg {
	uint8_t buf[16];
	int len;
};
/*
struct msg_queue {
	short first_empty;
	short first_full;
	short capacity;
	short occupancy;
	msg que[10];
};
*/
//TODO figure out how to have msg_queue declared separately
//TODO once variable-length messages exist bufer lengths become nontrivial
//specifically as to how buffer length negotiation works
struct spi_proto {
	uint8_t *sendbuf;
	uint8_t *getbuf;
	int buflen;
	//two queues, one of empty messages and one of pending messages. this is an object pool
	struct msg_queue {
		short first_empty;
		short first_full;
		short capacity;
		short occupancy;
		msg que[10];
	};
	struct msg_queue queue;
};



int
slave_get_message(struct spi_proto *p, unsigned char *buf, int len);
int
slave_send_message(struct spi_proto *p, unsigned char *buf, int len);
//int
//slave_handle_spi_interrupt();
int
slave_do_tick(struct spi_proto *p);

int
pop(spi_proto::msg_queue *q, msg *m);
int
push(spi_proto::msg_queue *q, struct msg *m);

int
reset(spi_proto::msg_queue *q);

}

#endif /* SOURCE_SPI_PROTO_SLAVE_HPP_ */
