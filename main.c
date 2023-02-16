#include "main.h"

unsigned int out = 0;
float t = 0;

unsigned int EEMEM eeSavedCode = 0x02;
unsigned int savedCode;
unsigned int irrValue = 0;
unsigned int timerCount = 0;
unsigned int secUpTime = 0;
unsigned int irrDelayCount = 0;



//DDRD = 0xFF; PORTD = 0x00;
//
//LCD_Init();
//LCD_DisplEnable_CursOnOffBlink(1, 0, 0);
//
//LCD_String("                ", 0, 0);
//LCD_String("                ", 1, 0);
//
//drawBigDigits(0, 0);
//drawBigDigits(1, 4);
//drawBigDigits(2, 9);
//drawBigDigits(3, 13);
//
//while(1)
//{
//
////LCD_String("+ ", 0, 7);
////LCD_String(" +", 1, 7);
////LCD_String("+", 1, 3);
////LCD_String("+", 0, 12);
////LCD_String(" ", 0, 3);
////LCD_String(" ", 1, 12);
////
////_delay_ms(1000);
////
////LCD_String(" +", 0, 7);
////LCD_String("+ ", 1, 7);
////LCD_String("+", 1, 3);
////LCD_String("+", 0, 12);
////LCD_String(" ", 0, 3);
////LCD_String(" ", 1, 12);
////
////_delay_ms(1000);
////
//}
//return 0;
//}


int main(void)
{
	DDRD = 0xFF; PORTD = 0x00;
	irrTimerInit();
	uartInit();
	LCD_Init();
	LCD_DisplEnable_CursOnOffBlink(1, 0, 0);
	LCD_String("                ", 0, 0);
	LCD_String("                ", 1, 0);
	
	drawBigDigits(0, 0);
	drawBigDigits(1, 4);
	drawBigDigits(2, 9);
	drawBigDigits(3, 13);
	
	LCD_String("+ ", 0, 7);
	LCD_String(" +", 1, 7);
	LCD_String("+", 1, 3);
	LCD_String("+", 0, 12);
	LCD_String(" ", 0, 3);
	LCD_String(" ", 1, 12);
	
	_delay_ms(1000);
	
	LCD_String(" +", 0, 7);
	LCD_String("+ ", 1, 7);
	LCD_String("+", 1, 3);
	LCD_String("+", 0, 12);
	LCD_String(" ", 0, 3);
	LCD_String(" ", 1, 12);
	
	_delay_ms(1000);

	savedCode = eeprom_read_word(&eeSavedCode);
	
	while (1) {
		
		if (IR_HIGH)
		{
			
			unsigned int irrValueNow = irrDecode();
			
			if ((irrValueNow) && (irrValue != irrValueNow || irrDelayCount != secUpTime))
			{
				irrDelayCount = secUpTime;
				uartTransmitHex(0, irrValueNow);
				uartNewLine();
				
				LCD_String("                ", 0, 0);
				LCD_String("                ", 1, 0);
				
				//LCD_String(snprintf(irrValueNow), 0, 0);
				
				char code_string[11];
				snprintf(code_string, 11, "CODE: 0x%02X ", irrValueNow);
				LCD_String(code_string, 0, 0);
				
				if (irrValueNow == savedCode)
				{
					//clear
				}
				irrValue = irrValueNow;
			}
		}
	}
}