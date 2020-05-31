/*
 * Copyright (C) 2020, Singular Engineer
 * https://singularengineer.com
 */

/*
=============  4 - Bit operation ================

  ----  ----  ----  ----  ----  ----  ----   ----
 | B7  | B6 |  B5 |  B4 |  BL |  EN |  R/W |  D/C |
  ----  ----  ----  ----  ----  ----  ----   ----
\ ----------------------/\------------------------/
			|							|
		   Data						 Control

 B[4-7] = Data and command bits
 BL  = Backlight
 EN  = Enaable bit (update LCD controller during Hi -> Low transistion
 R/W = Read or Write
 D/C = Data or Command

For bit defenition, please refer datasheet for HD44780
 */

#include "main.h"
#include "LCD_I2C.h"

extern I2C_HandleTypeDef hi2c2;

void LCD_Command(char cmd)
{
	char i2cBuff[4];
	char data_H, data_L;
	data_H = cmd & 0xF0;
	data_L = (cmd & 0x0F) << 4;

	i2cBuff[0] = data_H | En; //load new command byte with En high
	i2cBuff[1] = data_H | LCD_BACKLIGHT; //update command while keeping backlight on
	i2cBuff[2] = data_L | En; //load new command byte with En high
	i2cBuff[3] = data_L | LCD_BACKLIGHT; //update command while keeping backlight on

	HAL_I2C_Master_Transmit (&hi2c2, LCD_ADDRESS,(uint8_t *) i2cBuff, 4, 100);
}

void LCD_Data(char data)
{

	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c2, LCD_ADDRESS,(uint8_t *) data_t, 4, 100);
}

void LCD_Init(void)
{
	LCD_Command (0x30);
	HAL_Delay(50);
	LCD_Command (0x30);
	HAL_Delay(5);
	LCD_Command (0x20);
	HAL_Delay(10);

  	LCD_Command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(2);
	LCD_Command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(2);
	LCD_Command (0x01);  // clear display
	HAL_Delay(2);
	LCD_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(2);
	LCD_Command (0x0E); //Display on/off control --> D = 1, C =1 and B = 0. (Cursor and blink, last two bits)
	HAL_Delay(2);
}

void LCD_MoveCursor(int row, int col)
{
	//sanity check for 16x2 display
	if(row > 1)
		row = 1;
	if(col > 15)
		col = 15;

	switch(row)
	{
		case 0:
			col |= 0x80;
			break;
		case 1:
			col |= 0xC0;
			break;
	}
	LCD_Command(col);
}

void LCD_ClearDisplay()
{
	 LCD_Command(0x01);
}

