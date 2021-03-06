/*
Header Kurs 2020
Version "12.02.2020"
Header Bibliothek Browser:
K:\PHY\elfort\0000_Kurs2020_ATMega328_Progs\AT328-Main_LEER
file:///C:/WinAVR-20100110/doc/avr-libc/avr-libc-user-manual/index.html
�Controller Defines File Explorer: C:\WinAVR-20100110\avr\include\avr\iom328p.h
*/
#ifndef MAIN_HEADER  //Prevents multiple includes
#define MAIN_HEADER

#define MCA_NOBRAIN_MSG "*Habe kein Hirn*"
#define MCA_YOUROWN_MSG "*Benutze Deins*"

// =============================================================

#define UART_USE_ENABLED
#define MORSE_USED

// =============================================================

#include <ctype.h>
#include <inttypes.h>
#include <avr/io.h>
//#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <stdlib.h>
//#include <stdarg.h>
#include <avr/interrupt.h>  // obsolete #include <avr\signal.h>
#include <avr/wdt.h>
#include <util/delay.h>	//_delay_loop_1 _delay_loop_2 delay_ms
//#include "debug.h"
//#include "uartATM328.h"
#include "uart.h"
#include "TextService.h"
#include "Befehle.h"
#include "Morse.h"

#define _CLEARMASK_	&=	//Und= l�scht Bits OHNE Komplement
#define _CLEARBIT_	&=~	//Und=~ l�scht Bits mit Komplement
#define _SETBIT_	|=	//Oder= setzts Bits
#define _TOOGLEBIT_	^=  //XOR= TOGGLE  BITS  Wie ein Wechselschalter
#define _UNGLEICH_	!=
#define _AND_LOG_	&&
#define _OR_LOG_	||
#define _AND_BIN_	&
#define _OR_BIN_	|

#define DISABLE_ALL_PULLUP()	(SFIOR |= _BV(PUD));
#define ENABLE_ALLUP_SCL()		(SFIOR &= ~(_BV(PUD)));

#define LED5	0x20
#define SETLED() (PORTB |= LED5)
#define CLRLED() (PORTB &= ~LED5)


// GLOBALE VARIABLEN
//CONSTANT ARRAYs IN .TEXT = ROM ARREA   .DATA IST SRAM !! .BSS SRA
#define TICKS_1000MS	250
#define TICKS_100MS		25
#define TICKS_25HZ		10

typedef struct
{
	unsigned char TickEvent_Flag	: 1;
	unsigned char TICK_100MS__Flag	: 1;
	unsigned char Geschwaetzig		: 1;
} BIT_FIELD_TYPE;

//=========================================================
//PORT_ASSIGMNEMTS

// ========================================================
// *******   PORTB   ****************

#define INTERN_LED_BIT		(1 << PB5)					// Shiftleft "1" 5 mal
#define INTERN_LED_PORT		PORTB
#define INTERN_LED_INIT()	(DDRB |= INTERN_LED_BIT)	// OR = 
#define INTERN_LED_ON()		(PORTB |= INTERN_LED_BIT)	// OR =
#define INTERN_LED_OFF()	(PORTB &= ~INTERN_LED_BIT)	// AND = Komplement
#define INTERN_LED_TOGGLE()	(PORTB ^= INTERN_LED_BIT)	// XOR =


extern volatile BIT_FIELD_TYPE gstFlags;
extern volatile uint32_t gu32Ticks;

#endif //MAIN_HEADER
