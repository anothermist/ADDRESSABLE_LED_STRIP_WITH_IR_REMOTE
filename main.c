#include "main.h"

int main(void)
{
	//unsigned int savedCode;
	//unsigned int code = 0;
	//unsigned int time;
	//unsigned char protocolLetter;
	//init();
	//savedCode = eeprom_read_word(&eeSavedCode);
	//OSCCAL = 0x70;
	
	// lcd test
		DDRD = 0xFF; PORTD = 0x00;
		
		LCD_Init();
		LCD_DisplEnable_CursOnOffBlink(1, 0, 0);
		
		LCD_String("                ", 0, 0);
		LCD_String("                ", 1, 0);

		drawBigDigits(0, 0);
		drawBigDigits(1, 4);
		drawBigDigits(2, 9);
		drawBigDigits(3, 13);

	while(1)
	{
		
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
		
/*		PORTC ^= _BV(PINC5);
		while(IR_HIGH);
		PORTC ^= _BV(PINC5);
		TIMER_REG = 0;
		while(IR_LOW);
		//PORTC ^= _BV(PINC5);
		
		time = TIMER_REG;
		
		TIMER_REG = 0;

		// Initial pulse length between 9ms and 9.8ms => NEC protocol
		if(time > CONV(9000) && time < CONV(9800)) {
			protocolLetter = 'N';
			code = protocolNEC(code);

			// Initial pulse length between 0.8ms and 1.2ms => RC5 protocol
			} else if(time > CONV(800) && time < CONV(1200)) {
			protocolLetter = 'R';
			code = protocolRC5(code);

			// Initial pulse length between 2ms and 2.8ms => SIRC protocol
			} else if(time > CONV(2000) && time < CONV(2800)) {
			protocolLetter = 'S';
			code = protocolSIRC(code);
			} else {
			protocolLetter = 'U';
			continue;
		}

		if(code == 0) // If code is wrong, we get 0
		continue;

		if((PINB & _BV(PINB0)) == 0) // Learning mode
		{
			savedCode = code;
			eeprom_write_word(&eeSavedCode, savedCode);
			blink();
			continue;
		}

		if(code == savedCode)
		{
			codeMatch(code);
		}

		uartTransmitByte(protocolLetter);	
		uartTransmitHex(0, code);
		uartNewLine(); */
	}
	//return 0;
}
