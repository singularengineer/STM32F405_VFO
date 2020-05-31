/*
 * SI5351.h
 *
 * Author: Singular Engineer
 */

#ifndef SI5351_H_
#define SI5351_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define	SI5351_ADDR	0x60 << 1
#define XTAL_FREQ 25000000 //25MHz

#include <inttypes.h>

#define SI_CLK0_CONTROL	16			// Register definitions
#define SI_CLK1_CONTROL	17
#define SI_CLK2_CONTROL	18
#define SI_SYNTH_PLL_A	26
#define SI_SYNTH_PLL_B	34
#define SI_SYNTH_MS_0		42
#define SI_SYNTH_MS_1		50
#define SI_SYNTH_MS_2		58
#define SI_PLL_RESET		177

#define SI_R_DIV_1		0b00000000			// R-division ratio definitions
#define SI_R_DIV_2		0b00010000
#define SI_R_DIV_4		0b00100000
#define SI_R_DIV_8		0b00110000
#define SI_R_DIV_16		0b01000000
#define SI_R_DIV_32		0b01010000
#define SI_R_DIV_64		0b01100000
#define SI_R_DIV_128		0b01110000

#define SI_CLK_SRC_PLL_A	0b00000000
#define SI_CLK_SRC_PLL_B	0b00100000

void si5351aOutputOff(uint8_t clk);
void si5351aSetFrequency(uint32_t frequency);
uint8_t SI5351_WriteRegister(uint8_t reg, uint8_t data);
uint8_t SI5351_ReadRegister(uint8_t reg, uint8_t *data, uint8_t rxlen);
void SetPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom);
void SetMultiSynth(uint8_t synth, uint32_t divider, uint8_t rDiv);
void SI5351_OutputOff(uint8_t clk);
void SI5351_SetFrequency(uint32_t frequency);


#ifdef __cplusplus
}
#endif

#endif /* SI5351_H_ */
