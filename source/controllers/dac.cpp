#include <stddef.h>
#include <stdint.h>
#include "fsl_dac.h"
#include "ammdk-carrier/dac.h"
#include "gpio.h"
#include "dac.h"

uint16_t dac_bad_chunks;

void
dac_handle_slave(struct dac_cmd *cmd, struct dac *dacs, size_t dac_num)
{
	if (cmd->dac_id >= dac_num) {dac_bad_chunks++;}
	
	switch (cmd->ty) {
		case DAC_CMD_SET:
		dac_set(&dacs[cmd->dac_id], cmd->level);
		break;
		default:
		dac_bad_chunks++;
	}
}
