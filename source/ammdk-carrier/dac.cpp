#include "fsl_dac.h"
#include "dac.h"

struct dac carrier_dacs[DAC_NUM] = {
	{.base = DAC0}//TODO add motor control dac here
};

void
dac_set(struct dac *d, uint16_t val)
{
	//constant 0 here because we do not use the DAC buffer feature, so it's just 1 element
	DAC_SetBufferValue(d->base, 0U, val);
}
