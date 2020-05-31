/*
 * App.h
 *
 * Author: Singular Engineer
 */

#ifndef APP_H_
#define APP_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define LOOPSPEED 100

volatile uint16_t ticker;
volatile uint8_t dataAvailable;
volatile uint8_t SW_EN_Int;


/* USER CODE END Private defines */

 /* USER CODE BEGIN Prototypes */
void MainApp();
void UpdateCursor();
void UpdateFreqency();

int16_t Encoder_Diff();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */
