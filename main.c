#include "main.h"

uint16_t EEMEM eeSavedCode = 0x207E; // NEC code for LG remote and unused SimpLink button

void blink()
{
	PORTC |= _BV(PINC3); // Debug LED on
	TIMER_REG = 0; // Wait 20ms
	while(TIMER_REG < CONV(20000));
	PORTC &= ~_BV(PINC3); // Debug LED off
}

void codeMatch(unsigned int code)
{ // The input IR code is matching the learned code
	START_DDR |= _BV(START_PIN); // pull-down the pin connected to the computer's power on button
	blink();
	START_DDR &= ~(_BV(START_PIN)); // Hi-Z again
}

void init()
{
	TIMER_INIT(); // Initialize timer's prescaler
	if(IR_ENABLE_PULLUP) // Enable pull-up on IR pin
	IR_PORT |= _BV(IR_PIN_NUM);
	PORTB |= _BV(PINB0); // Jumper pull-up
	DDRC = 255; // Debug LED output - all pins on PORTC
	uartInit();
}

int main(void)
{
	unsigned int savedCode;
	unsigned int code = 0;
	unsigned int time;
	unsigned char protocolLetter;
	init();
	savedCode = eeprom_read_word(&eeSavedCode);
	OSCCAL = 0x70;

	while(1)
	{
		PORTC ^= _BV(PINC5);
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
		uartNewLine();
	}
	return 0;
}
