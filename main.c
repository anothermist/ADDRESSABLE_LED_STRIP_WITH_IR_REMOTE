#include "main.h"

unsigned int rtc_Sec, rtc_Min, rtc_Hrs, rtc_WDay, rtc_Date, rtc_Month, rtc_Year,
last_Sec;

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	irrTimerInit();
	uartInit();
	LCD_Init();
	TWI_Init();
	
	LCD_DisplEnable_CursOnOffBlink(1, 0, 0);
	LCD_String("                ", 0, 0);
	LCD_String("                ", 1, 0);
	
	//for (unsigned int i = 0; i < 3; i++) {
		//LCD_String("* ", 0, 7);
		//LCD_String(" *", 1, 7);
		//_delay_ms(1000);
		//LCD_String(" *", 0, 7);
		//LCD_String("* ", 1, 7);
		//_delay_ms(1000);
	//}
	
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
			
			if (irrValueNow) {
				uartTransmitHex(0, irrValueNow);
				uartNewLine();
				
				LCD_String("                ", 0, 0);
				LCD_String("                ", 1, 0);
				
				char code_string[11];
				snprintf(code_string, 11, "CODE: 0x%02X ", irrValueNow);
				LCD_String(code_string, 0, 0);
				
				_delay_ms(500);
				LCD_String("                ", 0, 0);
				LCD_String("                ", 1, 0);
				
			}
		}
	}
}
