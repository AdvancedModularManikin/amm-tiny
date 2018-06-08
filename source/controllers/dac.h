enum dac_cmd_ty {
	DAC_CMD_SET = 1
};

struct dac_cmd {
	uint8_t dac_id;
	uint8_t ty;
	uint16_t level;
};
#define CHUNK_LEN_DAC sizeof(struct dac_cmd)

void
dac_handle_slave(struct dac_cmd *cmd, struct dac *dacs, size_t dacnum);
