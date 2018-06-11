#include "fsl_gpio.h"

#include "carrier_gpio.h"
#include "ammdk-carrier/dac.h"
#include "ammdk-carrier/solenoid.h"
#include "ammdk-carrier/maxon.h"

#include "controllers/dac.h"
#include "controllers/gpio.h"
#include "controllers/motor.h"
#include "controllers/valve.h"
//just for SPI_MSG_PAYLOAD_LEN
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_proto_lib/spi_chunks.h"
#include "spi_proto_lib/spi_chunk_defines.h"


#define NUM_WAIT_CHUNKS 10
struct waiting_chunk wait_chunks[NUM_WAIT_CHUNKS] = {0};

//in slave because it is a convenience method
int
send_chunk(uint8_t *buf, size_t len)
{
	//find an open waiting_chunk in waiting_chunks and copy it in
	for (int i = 0; i < NUM_WAIT_CHUNKS; i++) {
		if (!wait_chunks[i].ready_to_pack) {
			memcpy(wait_chunks[i].buf, buf, len);
			wait_chunks[i].buf[0] = len; // just in case
			wait_chunks[i].ready_to_pack = 1;
			return 0;
		}
	}
	return -1;
}

unsigned long int short_chunks;
//invariant: *buf = len, due to how it's called but also because it is part of the packet shape
//TODO use endian-converting read functions
int
chunk_dispatcher_slave(uint8_t *buf, size_t len)
{
	//call appropriate type function on chunk, or record if it's an unknown type
	//TODO could potentially provide a histogram of how many bytes the unrecognized chunk was, would this be useful? just store an array and incremement the length index
	if (len < 2) return -1; // length zero isn't a real chunk, length 1 can't carry data
	//TODO log too-short chunks
	switch(buf[1]) {
	case CHUNK_TYPE_VALVE:
		if (len < CHUNK_LEN_VALVE) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD]
		struct valve_command valvecmd;
		valvecmd.valve_id = buf[2];
		valvecmd.valve_command = buf[3];
		valve_handle_slave(&valvecmd, solenoids, VALVE_NUM);
		break;
	case CHUNK_TYPE_GPIO:
		if (len < CHUNK_LEN_GPIO) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD]
		struct gpio_command gpiocmd;
		gpiocmd.gpio_id = buf[2];
		gpiocmd.gpio_command = buf[3];
		gpio_handle_slave(&gpiocmd, carrier_gpios, GPIO_NUM);
		break;
	case CHUNK_TYPE_DAC:
		if (len < CHUNK_LEN_DAC) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD|VAL]
		struct dac_cmd daccmd;
		daccmd.dac_id = buf[2];
		daccmd.ty = buf[3];
		daccmd.level = *(uint16_t*)&buf[4]; // TODO fix
		dac_handle_slave(&daccmd, carrier_dacs, DAC_NUM);
		break;
	case CHUNK_TYPE_MOTOR:
		if (len < CHUNK_LEN_MOTOR) {short_chunks++;break;}
		// [LEN|TYPE|ID|???]
		struct motor_command motorcmd; // TODO motor cmd parsing
		motorcmd.motor_id = buf[2];
		//motorcmd.ty = buf[3];
		motor_handle_slave(&motorcmd, carrier_motors, MOTOR_NUM);
		break;
	case CHUNK_TYPE_ECHO:
		buf[1] = CHUNK_TYPE_ECHO_RETURN;
		send_chunk(buf, len); //for testing
		break;
	//case CHUNK_TYPE_STRING
	default:
		unknown_chunk_type_msg_count++; // TODO think of better name
		return -1;
	}
	return 0;
}

int
chunk_dispatcher_echo(uint8_t *buf, size_t len)
{
	if (len < 2) {short_chunks++; return -1;} // length zero isn't a real chunk, length 1 can't carry data
	switch(buf[1]) {
	default:
		buf[1] = CHUNK_TYPE_ECHO_RETURN;
		send_chunk(buf, len); //for testing
		break;
	}
	return 0;
}

//this is the callback, call it on the SPI proto payload
int
spi_chunk_overall(uint8_t *buf, size_t len)
{
	return spi_msg_chunks(buf, len, chunk_dispatcher_echo);
}

int
prepare_slave_chunks(void)
{
	uint8_t buf[SPI_MSG_PAYLOAD_LEN];
	int ret1 = chunk_packer(wait_chunks, NUM_WAIT_CHUNKS, buf, SPI_MSG_PAYLOAD_LEN);
	int ret2 = slave_send_message(spi_proto::p, buf, SPI_MSG_PAYLOAD_LEN);
	return ret1|ret2;
}
