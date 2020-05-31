/*
 * Copyright (C) 2020, Singular Engineer
 * https://singularengineer.com
 *
 */

#include "main.h"


#include <inttypes.h>
#include "SI5351.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t SI5351_WriteRegister(uint8_t reg, uint8_t data)
{
	uint8_t txbuff[2];
	txbuff[0] = reg;
	txbuff[1] = data;

	HAL_I2C_Master_Transmit (&hi2c2, SI5351_ADDR,(uint8_t *) txbuff, 2, 100);
	return 0;
}

uint8_t SI5351_ReadRegister(uint8_t reg, uint8_t *data, uint8_t rxlen)
{
	//uint8_t stts;
	uint8_t txbuff[1];
	txbuff[0] = reg;
	HAL_I2C_Master_Transmit (&hi2c2, SI5351_ADDR,(uint8_t *) txbuff, 1, 100);
	HAL_I2C_Master_Receive(&hi2c2, SI5351_ADDR, (uint8_t *) data, rxlen, 100);

	return 0;
}

/*
 * Set up specified PLL with mult, num and denom
 * mult is 15..90
 * num is 0..1,048,575 (0xFFFFF)
 * denom is 0..1,048,575 (0xFFFFF)
 *
*/
void SetPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom)
{
	uint32_t P1;					// PLL config register P1
	uint32_t P2;					// PLL config register P2
	uint32_t P3;					// PLL config register P3

	P1 = (uint32_t)(128 * ((float)num / (float)denom));
	P1 = (uint32_t)(128 * (uint32_t)(mult) + P1 - 512);
	P2 = (uint32_t)(128 * ((float)num / (float)denom));
	P2 = (uint32_t)(128 * num - denom * P2);
	P3 = denom;

	SI5351_WriteRegister(pll + 0, (P3 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(pll + 1, (P3 & 0x000000FF));
	SI5351_WriteRegister(pll + 2, (P1 & 0x00030000) >> 16);
	SI5351_WriteRegister(pll + 3, (P1 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(pll + 4, (P1 & 0x000000FF));
	SI5351_WriteRegister(pll + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
	SI5351_WriteRegister(pll + 6, (P2 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(pll + 7, (P2 & 0x000000FF));
}

/*
 * Set up MultiSynth with integer divider and R divider
 * R divider is the bit value which is OR'ed onto the appropriate register, it is a #define in si5351a.h
 */
void SetMultiSynth(uint8_t synth, uint32_t divider, uint8_t rDiv)
{
	uint32_t P1;					// Synth config register P1
	uint32_t P2;					// Synth config register P2
	uint32_t P3;					// Synth config register P3

	P1 = 128 * divider - 512;
	P2 = 0;							// P2 = 0, P3 = 1 forces an integer value for the divider
	P3 = 1;

	SI5351_WriteRegister(synth + 0,   (P3 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(synth + 1,   (P3 & 0x000000FF));
	SI5351_WriteRegister(synth + 2,   ((P1 & 0x00030000) >> 16) | rDiv);
	SI5351_WriteRegister(synth + 3,   (P1 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(synth + 4,   (P1 & 0x000000FF));
	SI5351_WriteRegister(synth + 5,   ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
	SI5351_WriteRegister(synth + 6,   (P2 & 0x0000FF00) >> 8);
	SI5351_WriteRegister(synth + 7,   (P2 & 0x000000FF));
}

/*
 * Switches off Si5351a output
 * will switch off output CLK0
 */
void SI5351_OutputOff(uint8_t clk)
{
	SI5351_WriteRegister(clk, 0x80);		// Refer to SiLabs AN619 to see bit values - 0x80 turns off the output stage
}

/*
 * Set CLK0 output ON and to the specified frequency by the encoder
 * Frequency is in the range 1MHz to 150MHz
 */

void SI5351_SetFrequency(uint32_t frequency)
{
	uint32_t pllFreq;
	uint32_t xtalFreq = XTAL_FREQ;
	uint32_t l;
	float f;
	uint8_t mult;
	uint32_t num;
	uint32_t denom;
	uint32_t divider;

	divider = 900000000 / frequency;// Calculate the division ratio. 900,000,000 is the maximum internal
									// PLL frequency: 900MHz
	if (divider % 2) divider--;		// Ensure an even integer division ratio

	pllFreq = divider * frequency;	// Calculate the pllFrequency: the divider * desired output frequency

	mult = pllFreq / xtalFreq;		// Determine the multiplier to get to the required pllFrequency
	l = pllFreq % xtalFreq;			// It has three parts:
	f = l;							// mult is an integer that must be in the range 15..90
	f *= 1048575;					// num and denom are the fractional parts, the numerator and denominator
	f /= xtalFreq;					// each is 20 bits (range 0..1048575)
	num = f;						// the actual multiplier is  mult + num / denom
	denom = 1048575;				// For simplicity we set the denominator to the maximum 1048575

									// Set up PLL A with the calculated multiplication ratio
	SetPLL(SI_SYNTH_PLL_A, mult, num, denom);
									// Set up MultiSynth divider 0, with the calculated divider.
									// The final R division stage can divide by a power of two, from 1..128.
									// reprented by constants SI_R_DIV1 to SI_R_DIV128 (see si5351a.h header file)
									// If you want to output frequencies below 1MHz, you have to use the
									// final R division stage
	SetMultiSynth(SI_SYNTH_MS_0, divider, SI_R_DIV_1);
									// Reset the PLL. This causes a glitch in the output. For small changes to
									// the parameters, you don't need to reset the PLL, and there is no glitch
	SI5351_WriteRegister(SI_PLL_RESET, 0xA0);
									// Finally switch on the CLK0 output (0x4F)
									// and set the MultiSynth0 input to be PLL A
	SI5351_WriteRegister(SI_CLK0_CONTROL, 0x4F | SI_CLK_SRC_PLL_A);

}

