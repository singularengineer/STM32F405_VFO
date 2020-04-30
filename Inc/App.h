/*
 * App.h
 *
 *  Created on: Sep 1, 2019
 *      Author: Prem
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
volatile uint16_t ticker;
volatile uint8_t dataAvailable;
/* USER CODE END Private defines */

 /* USER CODE BEGIN Prototypes */
void MainApp();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */
