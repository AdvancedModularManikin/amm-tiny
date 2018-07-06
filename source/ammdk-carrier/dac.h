//very simple
struct dac {
	DAC_Type *base;
};

#define DAC_NUM 1
extern struct dac carrier_dacs[DAC_NUM];

void
dac_set(struct dac *d, uint16_t val);
