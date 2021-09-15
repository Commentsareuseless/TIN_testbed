/*
 * main.c
 *
 * Created: 9/6/2021 5:19:46 PM
 *  Author: Aracoin
 */ 

#include <xc.h>
#include <avr/iom328pb.h>

/*** MACROS *******************************************************************/

#define F_CPU 16000000UL			// required for setbaud & other libraries
#define BAUD 9600UL					// desired baud
#define BAUD_TOL 2					// desired baud rate tolerance (+/- %)

/*** INCLUDES *****************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>					// SFR/Bit identifiers
#include <util/setbaud.h>			// Baud rate calculation macro helpers

/*** GLOBAL VARIABLES *********************************************************/

char g_OutString[20];				// output buffer used with sprintf
char g_OutChar;							
uint8_t g_Command;

/*** LOCAL FUNCTION PROTOTYPES ************************************************/

void SYSTEM_Init(void);				// Initialize HW and variables
void TIMER1_Init(void);				// Timer/Counter1 initialization
void USART0_Init(void);				// USART0 initialization
void USART0_Put(uint8_t data);		// Transmit a byte
void USART0_PutString(char *ptr);	// Transmit a string
char USART0_GetChar(void);			// Receive a character (if available) 			

/*** main() *******************************************************************/

int main(void)
{
    SYSTEM_Init();	
	
	while(1){
		g_Command = (uint8_t) USART0_GetChar();

		uint8_t g_typeOfCommand = (g_Command >> 6);
		uint8_t g_gpioPort = ((g_Command >> 4) & 3);
		uint8_t g_gpioSR = ((g_Command >> 3) & 1); //1 = set; 0 = reset;
		uint8_t g_gpioPin = (g_Command & 7);
		switch(g_typeOfCommand){
			case 1: //GPIO write
				switch(g_gpioSR){
					case 0: //reset
						switch(g_gpioPort){
							case 0: //PORTB
								PORTB &= ~(1 << g_gpioPin); 
							break;
							
							case 1: //PORTC
								PORTC &= ~(1 << g_gpioPin);
							break;
							
							case 2: //PORTD
								PORTD &= ~(1 << g_gpioPin);
							break;
								
							case 3: //PORTE
								PORTE &= ~(1 << g_gpioPin);
							break;
							
							default:
							break;
						}
					break;
					
					case 1: //set
						switch(g_gpioPort){
							case 0: //PORTB
							PORTB |= (1 << g_gpioPin);
							break;
							
							case 1: //PORTC
							PORTC |= (1 << g_gpioPin);
							break;
							
							case 2: //PORTD
							PORTD |= (1 << g_gpioPin);
							break;
							
							case 3: //PORTE
							PORTE |= (1 << g_gpioPin);
							break;
							
							default:
							break;
						}
					break;
					
					default:
					break;
				}
				break;
			
			case 2: //GPIO read
				//not implemented
			case 3:
				//not implemented
			case 0: //no data
				
			default:
				break;
		}
		//*/
	}
} // main()

void SYSTEM_Init(void){
	
	DDRB |= 0b00111111;
	DDRC |= 0b00111111;
	DDRD |= 0b11111100;
	DDRE |= 0b00001111;
	PORTB &= ~(0b11000000);
	PORTC &= ~(0b11000000);
	PORTD &= ~(0b00000011);
	PORTE &= ~(0b11110000);

	// USART0
	USART0_Init();
		
}

/*** USART0_Init() *************************************************************/

void USART0_Init(void){
	
	// Set the BAUD rate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X					// USE_2X defined by setbaud.h based on inputs
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	// Set the Mode & Frame Parameters
	// Asynchronous, 8-data, No parity, 1 stop
	UCSR0C = 0x06;
	
	// Enable USART0 Transmitter and Receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

} // USART0_Init()

/*** USART0_Put() **************************************************************/

void USART0_Put(uint8_t data){
	
	//Checking to see if USART TX buffer is empty for new data
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Initiating transfer
	UDR0 = data;

} // USART0_Put()

/*** USART0_PutString() ********************************************************/

void USART0_PutString(char *ptr){
	
	while(*ptr){			// Loop until end of string (*s = '\0')
		USART0_Put(*ptr++);	// Send the character and point to the next one
	}
	
} // USART0_PutString()

/*** USART0_GetChar() **************************************************************/

char USART0_GetChar(void){
	
	char rxdata;
	
	if(UCSR0A & (1<<RXC0)){		// checking if USART RX data is available
		rxdata = UDR0;			// reading the received byte (clears RXC0)
		return rxdata;			// return the data
	}
	
	return 0b00000000;

} // USART0_GetChar()
