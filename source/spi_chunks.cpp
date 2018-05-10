#include "stdint.h"
//#include "stddef.h"

//for memcpy
#include <string.h>

#include "spi_proto.h"
#include "spi_chunks.h"
#include "spi_chunks_slave.h"



/*
assuming the payload of the spi proto is large but most commands are small the command flow is:
	callback for spi_proto (whole message)
	command dispatcher (individual chunks with length)
		component message processors (type_command structs, provided with their type's array of instances)
*/

#if 0
int chunk_dispatcher_master(uint8_t *buf, size_t len)
{
	if (len < 2) return -1; // length zero isn't a real chunk, length 1 can't carry data
	switch(buf[1]) {
	case CHUNK_TYPE_VALVE:
		if (len < 4) break; // TODO maybe increment some error counter somewhere
		// [LEN|TYPE|ID|CMD]
		struct valve_command valvecmd;
		valvecmd.valve_id = buf[2];
		valvecmd.valve_command = buf[3];
		valve_handle_master(valvecmd, valve_arr, VALVE_NUM);
		break;
	case CHUNK_TYPE_GPIO:
		if (len < CHUNK_LEN_GPIO) break; // TODO log bad chunk counter
		// [LEN|TYPE|ID|CMD]
		struct gpio_command gpiocmd;
		gpiocmd.gpio_id = buf[2];
		gpiocmd.gpio_command = buf[3];
		gpio_handle_master(gpiocmd, carrier_gpios, GPIO_NUM);
		break;
	//case CHUNK_TYPE_STRING
	default:
		unknown_chunk_type_msg_count++; // TODO think of better name
	}
}
#endif

int
chunk_packer(struct waiting_chunk *chunks, size_t numchunk,
	uint8_t *buf, size_t len)
{
	for (int i; i < numchunk;i++) {
		if (chunks[i].ready_to_pack) {
			if (chunks[i].buf[0] <= len) {
				memcpy(buf, chunks[i].buf, chunks[i].buf[0]);
				len -= chunks[i].buf[0];
				chunks[i].ready_to_pack = 0;
				if (len < 3) break; // 2 is the smallest possible chunk: [2|ID]
			}
		}
	}
}

//so we need a function to dispatch on chunks of the message
int
spi_msg_chunks(uint8_t *buf, size_t len, int (*chunk_handler)(uint8_t *b, size_t len))
{
	unsigned int p = 0;
	
	while (p < len) {
		if (buf[p]) {
			if (p + buf[p] < len) { // doesn't overflow at least
				int ret = chunk_handler(&buf[p], buf[p]); // TODO this can access the rest of the array possibly, but we probably trust it anyway
				p += buf[p];
				//TODO do something with ret
			} else {
				return -1;
			}
		} else {
			break;
		}
	}
	return 0;
}

//this is the callback
int
spi_chunk_overall(uint8_t *buf, size_t len)
{
	return spi_msg_chunks(buf, len, chunk_dispatcher_slave);
}
