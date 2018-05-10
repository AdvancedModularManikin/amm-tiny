#include "fsl_gpio.h"
#include "solenoid.h"
#include "valve.h"
#include "carrier_gpio.h"
#include "controllers/gpio.h"
//just for SPI_MSG_PAYLOAD_LEN
#include "spi_proto.h"
#include "spi_chunks.h"



#define CHUNK_TYPE_VALVE 1
#define CHUNK_TYPE_GPIO 2
#define CHUNK_LEN_GPIO 4

//invariant: *buf = len, due to how it's called but also because it is part of the packet shape
//TODO put this in spi_chunk_slave.cpp
int
chunk_dispatcher_slave(uint8_t *buf, size_t len)
{
	//call appropriate type function on chunk, or record if it's an unknown type
	//TODO could potentially provide a histogram of how many bytes the unrecognized chunk was, would this be useful? just store an array and incremement the length index
	if (len < 2) return -1; // length zero isn't a real chunk, length 1 can't carry data
	switch(buf[1]) {
	case CHUNK_TYPE_VALVE:
		if (len < 4) break; // TODO maybe increment some error counter somewhere
		// [LEN|TYPE|ID|CMD]
		struct valve_command valvecmd;
		valvecmd.valve_id = buf[2];
		valvecmd.valve_command = buf[3];
		valve_handle_slave(valvecmd, solenoids, VALVE_NUM);
		break;
	case CHUNK_TYPE_GPIO:
		if (len < CHUNK_LEN_GPIO) break; // TODO log bad chunk counter
		// [LEN|TYPE|ID|CMD]
		struct gpio_command gpiocmd;
		gpiocmd.gpio_id = buf[2];
		gpiocmd.gpio_command = buf[3];
		gpio_handle_slave(gpiocmd, carrier_gpios, GPIO_NUM);
		break;
	//case CHUNK_TYPE_STRING
	default:
		unknown_chunk_type_msg_count++; // TODO think of better name
	}
}
