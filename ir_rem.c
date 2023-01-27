#include "ir_rem.h" 

unsigned char repeatCount = 0;

unsigned int protocolNEC(unsigned int code)
{
	unsigned char bitVal;
	unsigned int time;
	unsigned char i;
	unsigned int invertedCode = 0;
	static unsigned int lastCode = 0;

		while(IR_HIGH);/*
			if(TIMER_REG > CONV(5000))
				return 0;*/

		time = TIMER_REG;
		TIMER_REG = 0;

		PORTC ^= _BV(PINC5);

		// 4200 us
		if(time > CONV(4200))
		{
			// regular button press
			repeatCount = 0;
		} else {
			
			// hold button press
			// send last keycode
			if(++repeatCount == NEC_REPEAT_RATE)
			{
				repeatCount = 0;
				return lastCode;
			}
			return 0;
		}
		
		code = 0;
		
		// Read 32 data bits
		for(i = 0; i < 32; i++)
		{
			while(IR_LOW);
			/*	if(TIMER_REG > CONV(5000))
					return 0;*/
			while(IR_HIGH);/*
				if(TIMER_REG > CONV(5000))
					return 0;*/

			time = TIMER_REG;
			TIMER_REG = 0;
			
			// 1650 us
			if(time > CONV(1650))
			{
				bitVal = 1;
			} else {
				bitVal = 0;
			}

			PORTC ^= _BV(PINC5);
			
			if((i < 8) || (i >= 16 && i < 24))
			{
				code = code << 1;
				code |= bitVal;
			}

			if((i >= 8 && i < 16) || (i >= 24 && i < 32))
			{
				invertedCode = invertedCode << 1;
				invertedCode |= bitVal;
			}
		}

		if(code != ~invertedCode)
			return 0;

		lastCode = code;
		return code;
}

unsigned int protocolRC5(unsigned int code)
{
	unsigned char repeatBit;
	unsigned char i;

	static unsigned int lastCode = 0;
	static unsigned char lastRepeatBit = 0;

	unsigned char errorBit;

	code = 0;

	// 3160 us offset from the first pulse
	while(TIMER_REG < CONV(3160));
	TIMER_REG = 0;

	// Read "repeat" bit
	repeatBit = IR_VAL;
	PORTC ^= _BV(PINC5);

	// Move 1760us to the first data bit
	while(TIMER_REG < CONV(880));
	TIMER_REG = 0;

	// Read 12 data bits (5 address & 7 command)
	for(i = 0; i < 11; i++)
	{
		code = code << 1;
		PORTC ^= _BV(PINC5);
		errorBit = IR_VAL;

		//while(TIMER_REG < CONV(RC5_DELAY_0));
		//TIMER_REG = 0;

		// Read second half of bit, which has to be inverted
		// so we check whether the code is ok

		while(errorBit == IR_VAL)
		if(TIMER_REG > CONV(1000))
		return 0;

		TIMER_REG = 0;
		while(TIMER_REG < CONV(400));
		TIMER_REG = 0;

		PORTC ^= _BV(PINC5);
		
		if(IR_VAL)
		code |= 0x0001;

		// If the previous and current bit is the same - we have error code
		if(IR_VAL && errorBit)
		return 0;
		
		while(TIMER_REG < CONV(800));
		TIMER_REG = 0;
	}

	if(code == lastCode && repeatBit == lastRepeatBit)
	{
		if(++repeatCount == RC5_REPEAT_RATE)
		{
			repeatCount = 0;
			return code;
			} else {
			return 0;
		}
	}

	if(repeatBit != lastRepeatBit)
	{
		repeatCount = 0;
	}

	lastCode = code;
	lastRepeatBit = repeatBit;
	return code;
}

unsigned int protocolSIRC(unsigned int code)
{
	unsigned int time;
	unsigned char i;
	static unsigned int lastCode = 0;

	code = 0;

	// Read 32 data bits
	for(i = 0; i < 12; i++)
	{	
		while(IR_LOW)
		if(TIMER_REG > CONV(2000))
		return 0;
		while(IR_HIGH)
		if(TIMER_REG > CONV(2000))
		return 0;

		time = TIMER_REG;
		TIMER_REG = 0;

		// error check - pulse length between 500-2000us
		if(time < CONV(400) || time > CONV(2000))
		return 0;

		code = code << 1;

		// 1440 us
		if(time > CONV(1440))
		code |= 1;
	}
	
	if(code == lastCode)
	{
		if((repeatCount++) == SIRC_REPEAT_RATE)
		{
			repeatCount = 0;
			return code;
			} else {
			return 0;
		}
	}
	lastCode = code;
	return code;
}
