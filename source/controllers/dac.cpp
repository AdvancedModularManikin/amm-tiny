#include <stddef.h>
#include <stdint.h>
#include "fsl_dac.h"
#include "ammdk-carrier/dac.h"
#include "gpio.h"
#include "dac.h"
#include "spi_remote_api.h"
#include "spi_chunk_defines.h"
#include "spi_chunks_slave.h"

uint16_t dac_bad_chunks;

//TODO delete or make more comprehensive
uint16_t last_dac_write;

//TODO remove hard-coding of DAC0
void
dac_handle_slave(struct dac_cmd *cmd, struct dac *dacs, size_t dac_num)
{
	if (cmd->id >= dac_num) {dac_bad_chunks++;}
	uint8_t buf[CHUNK_LEN_DAC_S2M];
	switch (cmd->cmd) {
	case OP_SET:
		//dac_set(&dacs[cmd->id], cmd->val);
		last_dac_write = cmd->val;
		DAC_SetBufferValue(DAC0, 0U, cmd->val);
		buf[0] = 0;
		buf[1] = CHUNK_TYPE_DAC;
		buf[2] = cmd->id;
		buf[3] = OP_SET;
		send_chunk(buf, 4);
		break;
	default:
		dac_bad_chunks++;
	}
	
}
