/*
 * App.c
 *
 *  Created on: Sep 15, 2019
 *      Author: Prem
 */
#include "App.h"
#include "string.h"
#include "usbd_cdc_if.h"

uint8_t encoder_str[] = "F: ";
uint8_t newline[] = "\r\n";

uint32_t encoder_old = 0;
uint32_t encoder_new = 0;
char usbbuff[12];

void MainApp()
{
	if(ticker >= 100)
	{
		ticker = 0;
		LL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		encoder_new = TIM4->CNT>>2;

		//Transmit USB only if there is encoder update
		if(encoder_old != encoder_new)
		{
			itoa(encoder_new,(char *)usbbuff,10);
			while((CDC_Transmit_FS(encoder_str,strlen((char *)encoder_str)) == USBD_BUSY));
			while((CDC_Transmit_FS((uint8_t *)usbbuff,strlen(usbbuff)) == USBD_BUSY));
			while((CDC_Transmit_FS((uint8_t *)newline,2) == USBD_BUSY));
			encoder_old = encoder_new;
		}
	}
}

