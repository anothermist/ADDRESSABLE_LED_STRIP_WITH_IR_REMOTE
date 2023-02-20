#include "main.h"

unsigned int out = 0;
float t = 0;

unsigned int EEMEM eeSavedCode = 0x02;
unsigned int savedCode;
unsigned int irrValue = 0;
unsigned int timerCount = 0;
unsigned int secUpTime = 0;
unsigned int irrDelayCount = 0;
unsigned int rtc_Sec, rtc_Min, rtc_Hrs, rtc_WDay, rtc_Date, rtc_Month, rtc_Year,
last_Sec;



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


int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	irrTimerInit();
	uartInit();
	LCD_Init();
	TWI_Init();
	
	LCD_DisplEnable_CursOnOffBlink(1, 0, 0);
	LCD_String("                ", 0, 0);
	LCD_String("                ", 1, 0);
	
	savedCode = eeprom_read_word(&eeSavedCode);
	
	while (1) {
		
		DS3231_Update();
		rtc_Sec = DS3231_getSec();
		
		if (last_Sec != rtc_Sec) {
			last_Sec = rtc_Sec;
			rtc_Min = DS3231_getMin();
			rtc_Hrs = DS3231_getHrs();
			rtc_WDay = DS3231_getWDay();
			rtc_Date = DS3231_getDate();
			rtc_Month = DS3231_getMonth();
			rtc_Year = DS3231_getYear();
			
			if (rtc_Sec % 2 != 0) {
				LCD_String("* ", 0, 7);
				LCD_String(" *", 1, 7);
				} else {
				LCD_String(" *", 0, 7);
				LCD_String("* ", 1, 7);
			}
			
			drawBigDigits(rtc_Hrs/10, 0);
			drawBigDigits(rtc_Hrs % 10, 4);
			drawBigDigits(rtc_Min/10, 9);
			drawBigDigits(rtc_Min % 10, 13);

			char time_string[26];
			snprintf(time_string, 26, "TIME %02d:%02d:%02d %02d %02d %02d %02d %02d %02d %02d",
			rtc_Hrs, rtc_Min, rtc_Sec, rtc_WDay, rtc_Date, rtc_Month, rtc_Year);
			uartTransmitString(time_string); uartNewLine();
			
		}
		
		if (IR_HIGH) {
			
			unsigned int irrValueNow = irrDecode();
			
			if ((irrValueNow) && (irrValue != irrValueNow || irrDelayCount != secUpTime)) {
				irrDelayCount = secUpTime;
				uartTransmitHex(0, irrValueNow);
				uartNewLine();
				
				LCD_String("                ", 0, 0);
				LCD_String("                ", 1, 0);
				
				//LCD_String(snprintf(irrValueNow), 0, 0);
				
				char code_string[11];
				snprintf(code_string, 11, "CODE: 0x%02X ", irrValueNow);
				LCD_String(code_string, 0, 0);
				
				if (irrValueNow == savedCode) {
					//clear
				}
				irrValue = irrValueNow;
			}
		}
	}
}
