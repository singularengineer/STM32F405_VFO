/*
 * App.c
 *
 *  Created on: Sep 15, 2019
 *      Author: Prem
 */
#include "App.h"
#include "string.h"
#include "usbd_cdc_if.h"

uint8_t tim4[] = "Tim 4: ";
uint8_t newline[] = "\r\n";
uint8_t USBstr[] = "USB Test...";
uint32_t encoder4 = 0;
uint32_t encoder3 = 0;
uint8_t usbbuff[12];

void Init_APP()
{
	LL_SYSTICK_EnableIT();
}

void MainApp()
{
	if(ticker >= 100)
	{
		ticker = 0;
		LL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		while((CDC_Transmit_FS(USBstr,strlen((char *)USBstr)) == USBD_BUSY));

		//while((CDC_Transmit_FS(USBstr,strlen((char *)USBstr)) == USBD_BUSY));
		//while((CDC_Transmit_FS("\r\n",2) == USBD_BUSY));
//		encoder4 = TIM4->CNT>>2;
//		encoder3 = TIM3->CNT>>2;
//		//CDC_Transmit_FS(usbstr,strlen((char *)usbstr));
//		//CDC_Transmit_FS(usbstr,strlen((char *) itoa(encoder, usbbuff, 16)));
//		//itoa(encoder4,usbbuff,10);
//		itoa(encoder3,usbbuff,10);
//
//		while((CDC_Transmit_FS(tim4,strlen((char *)tim4)) == USBD_BUSY));
//		while((CDC_Transmit_FS(usbbuff,strlen((char *)usbbuff)) == USBD_BUSY));
//		while((CDC_Transmit_FS("\r\n",2) == USBD_BUSY));
//		//CDC_Transmit_FS(newline,strlen((char *)newline));
	}
}

