#include "fsl_gpio.h"

#include "ammdk-carrier/carrier_gpio.h"
#include "ammdk-carrier/dac.h"
#include "ammdk-carrier/maxon.h"

#include "controllers/adc.h"
#include "controllers/dac.h"
#include "controllers/gpio.h"
#include "controllers/motor.h"
//just for SPI_MSG_PAYLOAD_LEN
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_chunks.h"
#include "spi_chunk_defines.h"
#include "spi_remote_api.h"


#define NUM_WAIT_CHUNKS 10
struct waiting_chunk wait_chunks[NUM_WAIT_CHUNKS] = {0};

unsigned int unknown_chunk_type_msg_count;

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
	case CHUNK_TYPE_GPIO:
		if (len < CHUNK_LEN_GPIO_M2S) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD|VAL]
		struct gpio_cmd gpiocmd;
		gpiocmd.id = buf[2];
		gpiocmd.cmd = buf[3];
		gpiocmd.val = buf[4];
		gpio_handle_slave(&gpiocmd, carrier_gpios, GPIO_NUM);
		break;
	case CHUNK_TYPE_DAC:
		if (len < CHUNK_LEN_DAC_M2S) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD|VAL|VAL]
		struct dac_cmd daccmd;
		daccmd.id = buf[2];
		daccmd.cmd = buf[3];
		daccmd.val = buf[4] | (((uint16_t)buf[5])<<8);
		dac_handle_slave(&daccmd, carrier_dacs, DAC_NUM);
		break;
	case CHUNK_TYPE_ADC:
	//TODO adapt adcs to use the same schema as the other handlers
		if (len < CHUNK_LEN_ADC_M2S) {short_chunks++;break;}
		// [LEN|TYPE|ID|CMD]
		struct adc_cmd adccmd;
		adccmd.id = buf[2];
		adccmd.cmd = buf[3];
		adc_handle_slave(&adccmd);
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
	return spi_msg_chunks(buf, len, chunk_dispatcher_slave);
}

int
prepare_slave_chunks(void)
{
	uint8_t buf[SPI_MSG_PAYLOAD_LEN];
	int ret2, ret1 = chunk_packer(wait_chunks, NUM_WAIT_CHUNKS, buf, SPI_MSG_PAYLOAD_LEN);
	if (ret1)
		ret2 = slave_send_message(spi_proto::p, buf, SPI_MSG_PAYLOAD_LEN);
	return ret1|ret2;
}
