#include "main.h"

unsigned int rtc_Sec, rtc_Min, rtc_Hrs, rtc_WDay, rtc_Date, rtc_Month, rtc_Year,
last_Sec;

int main(void) {
	DDRD = 0xFF; PORTD = 0x00;
	irrTimerInit();
	uartInit();
	TWI_Init();
	lcd_init();  lcd_twi_init();

	lcd_string("LCD STRING 0 3", 0, 3);   lcd_twi_string("LCD STRING 0 3", 0, 3);
	lcd_string("LCD STRING 1 3", 1, 3);   lcd_twi_string("LCD STRING 1 3", 1, 3);

	_delay_ms(1000);
	lcd_clear(); lcd_twi_clear();
	
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
				lcd_string("* ", 0, 7); lcd_twi_string("* ", 0, 7);
				lcd_string(" *", 1, 7); lcd_twi_string(" *", 1, 7);
				} else {
				lcd_string(" *", 0, 7); lcd_twi_string(" *", 0, 7);
				lcd_string("* ", 1, 7); lcd_twi_string("* ", 1, 7);
			}
			
			lcd_drawBigDigits(rtc_Hrs/10, 0);    lcd_twi_drawBigDigits(rtc_Hrs/10, 0);
			lcd_drawBigDigits(rtc_Hrs % 10, 4);  lcd_twi_drawBigDigits(rtc_Hrs % 10, 4);
			lcd_drawBigDigits(rtc_Min/10, 9);    lcd_twi_drawBigDigits(rtc_Min/10, 9);
			lcd_drawBigDigits(rtc_Min % 10, 13); lcd_twi_drawBigDigits(rtc_Min % 10, 13);

			char time_string[26];
			snprintf(time_string, 26, "TIME %02d:%02d:%02d %02d %02d %02d %02d %02d %02d %02d",
			rtc_Hrs, rtc_Min, rtc_Sec, rtc_WDay, rtc_Date, rtc_Month, rtc_Year);
			uartTransmitString(time_string); uartNewLine();
			
			//float celsius = 12.34; //DS3231_getTemp();
			//char celsius_string[26];
			//snprintf(celsius_string, 26, "TEMPERATURE %f", celsius);
			//uartTransmitString_F(celsius_string); uartNewLine();
			
		}
		
		if (IR_HIGH) {
			
			unsigned int irrValueNow = irrDecode();
			
			if (irrValueNow) {
				uartTransmitHex(0, irrValueNow);
				uartNewLine();
				
				lcd_clear(); lcd_twi_clear();
				
				char code_string[11];
				snprintf(code_string, 11, "CODE: 0x%02X ", irrValueNow);
				lcd_string(code_string, 0, 0); lcd_twi_string(code_string, 0, 0);
				
				_delay_ms(500);
				
				lcd_clear(); lcd_twi_clear();
			}
		}
	}
}
