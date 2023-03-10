#include "uart.h"

void uartInit() {
	unsigned int ubrr = ((F_CPU/16)/(UART_BAUD+1));
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C=(1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01)|(1<<USBS0);
	uartTransmitString_F (PSTR("OK"));
	uartNewLine();
}

unsigned char uartReceiveByte() { // Function to receive a single byte
	unsigned char data, status;
	
	while(!(UCSR0A & (1<<RXC0))); 	// Wait for incomming data
	
	status = UCSR0A;
	data = UDR0;
	
	return(data);
}

void uartTransmitByte(unsigned char data) { //	Function to transmit a single byte
	while (!(UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer */
	UDR0 = data; 			        /* Start transmition */
}

void uartTransmitHex(unsigned char dataType, unsigned long data) { // Function to transmit hex format data. first argument indicates type: CHAR, INT or LONG. Second argument is the data to be displayed
	unsigned char count, i, temp;
	unsigned char dataString[] = "0x        ";

	if (dataType == CHAR) count = 2;
	if (dataType == INT) count = 4;
	if (dataType == LONG) count = 8;

	for(i=count; i>0; i--)
	{
		temp = data % 16;
		if((temp>=0) && (temp<10)) dataString [i+1] = temp + 0x30;
		else dataString [i+1] = (temp - 10) + 0x41;

		data = data/16;
	}

	uartTransmitString (dataString);
}

void uartTransmitString(unsigned char* string) { // Function to transmit a string in RAM
	while (*string)
	uartTransmitByte(*string++);
}

void uartTransmitString_F(char* string) { // Function to transmit a string in Flash
	while (pgm_read_byte(&(*string)))
	uartTransmitByte(pgm_read_byte(&(*string++)));
}

void uartNewLine() {
	TX_NEWLINE;
}
