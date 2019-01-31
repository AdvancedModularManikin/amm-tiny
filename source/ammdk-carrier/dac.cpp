#include "fsl_dac.h"
#include "dac.h"

struct dac carrier_dacs[DAC_NUM] = {
	{.base = DAC0},
	{.base = DAC1},
};

void
dac_init(void)
{
	dac_config_t dac0_config;
	DAC_GetDefaultConfig(&dac0_config);
	DAC_Init(DAC0, &dac0_config);
	DAC_SetBufferReadPointer(DAC0, 0U);
	
	dac_config_t dac1_config;
	DAC_GetDefaultConfig(&dac1_config);
	DAC_Init(DAC1, &dac1_config);
	DAC_SetBufferReadPointer(DAC1, 0U);
}

void
dac_set(struct dac *d, uint16_t val)
{
	//constant 0 here because we do not use the DAC buffer feature, so it's just 1 element
	DAC_SetBufferValue(d->base, 0U, val);
}
