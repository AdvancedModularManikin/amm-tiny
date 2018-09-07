//#include "fsl_adc.h"
//#include "../ammdk-carrier/carrier_adc.h"
#include <stdint.h>
#include <stddef.h>

#include "adc.h"
#include "fsl_adc16.h"
#include "../pressuresensor.h"

#include "spi_remote.h"
#include "spi_proto_lib/spi_chunk_defines.h"
#include "spi_chunks_slave.h"

int adc_bad_chunks = 0;

//OP_SET currently has no meaning for adcs
void
adc_handle_slave(struct adc_cmd *cmd)
{
	if (cmd->id >= ADC_NUM) {adc_bad_chunks++; return;}
	
	uint32_t adc_read;
	switch (cmd->cmd) {
	case OP_GET:
		if (cmd->id < ADC_NUM) {
			adc_read = carrier_sensors[cmd->id].raw_pressure;
			uint8_t buf[] = {0, CHUNK_TYPE_ADC, cmd->id, OP_GET,
				adc_read&0xff, (adc_read>>8)&0xff,
				(adc_read>>16)&0xff, (adc_read>>24)&0xff};
			send_chunk(buf, 8);
			return;
		} else {
			{adc_bad_chunks++; return;}
		}
	default:
		{adc_bad_chunks++; return;}
	}

	return;
}
