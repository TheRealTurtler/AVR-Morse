/*
 * Morse.h
 *
 * Created: 17.03.2020 14:31:07
 *  Author: Michael
 */


#ifndef MORSE_H_
#define MORSE_H_

// ================ Includes ==============================

#include <ctype.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "Morse_8Bit_Simpel.h"

#ifdef UART_USE_ENABLED
#include "uart.h"
#include "TextService.h"
#endif

// ================ Definitionen ==========================

#define TIMER_FRQ_KHZ					72U		// Anzahl compare timer events pro Sekunde (-> gemessene Frequenz an OCA-Port ist nur 1/2 wegen toggle)
#define DEFAULT_MORSE_PUNKT_LAENGE_MS	400U	// Zeitdauer eines Punkts in Millisekunden

#define MORSE_DDRX	DDRF
#define MORSE_PORTX	PORTF
#define MORSE_BIT	_BV(0)
#define INIT_MORSE_PORT()	(MORSE_DDRX |= MORSE_BIT)
#define SET_MORSE_BIT()		(MORSE_PORTX |= MORSE_BIT)
#define CLEAR_MORSE_BIT()	(MORSE_PORTX &= ~MORSE_BIT)

#define TON_PUNKT_FREQ		(36000U / 600U)		// /Hertz ... ?
#define TON_STRICH_FREQ		(36000U / 900U)

#define TIMER2_DDRX		DDRB
#define TIMER2_OCA		4
#define TIMER2_OCA_BIT	_BV(TIMER2_OCA)

#define MORSE_BUSY_DDRX		DDRF
#define MORSE_BUSY_PORTX	PORTF
#define MORSE_BUSY_BIT		_BV(1)
#define INIT_MORSE_BUSY_PORT()	(MORSE_BUSY_DDRX |= MORSE_BUSY_BIT)
#define SET_MORSE_BUSY_BIT()	(MORSE_BUSY_PORTX |= MORSE_BUSY_BIT)
#define CLEAR_MORSE_BUSY_BIT()	(MORSE_BUSY_PORTX &= ~MORSE_BUSY_BIT)
#define TOGGLE_MORSE_BUSY_BIT()	(MORSE_BUSY_PORTX ^= MORSE_BUSY_BIT)

//#define MORSE_SOUND_USED

// ================ Structs ===============================

typedef struct
{
	unsigned char Aktiv_F : 1;
	unsigned char Start_F : 1;
	unsigned char Stop_F : 1;
	unsigned char Ready_F : 1;
	unsigned char Byte_Ok_F : 1;
	
	unsigned char Punkt_F : 1;
	unsigned char Strich_F : 1;
	unsigned char ZeichenPause_F : 1;
	unsigned char BuchstabenPause_F : 1;
	unsigned char WortPause_F : 1;
} MORSE_FLAGS;


typedef struct
{
	uint32_t u32Geschwindigkeit;
	uint32_t u32SignalLaenge;
	uint16_t u16TonFreq;	
	
	uint32_t u32PunktLaenge;
	uint32_t u32StrichLaenge;
	uint32_t u32ZeichenPauseLaenge;
	uint32_t u32BuchstabenPauseLaenge;
	uint32_t u32WortPauseLaenge;
	
	uint8_t u8MorseVal;
	uint8_t u8ScanVal;
	
	//?
	//uint8_t u8CharSumCnt;
	//uint8_t u8TxBitParser;
	
	//PGM_P pmcaMorse;
} MORSE_DATA;


extern volatile MORSE_FLAGS gstMorseFlags;
extern MORSE_DATA gstMorseData;

// ================ Funktionen ============================

extern void InitMorse( void );
extern void SendMorseString( char* caString, uint8_t u8SizeOfString );
extern void SendMorseChar( uint8_t u8Ascii );
extern void SetMorseSpeed( uint16_t u16SpeedMS );

void InitTimer_72KHZ( void );
uint8_t AsciiToMorse( uint8_t u8Ascii );
void ScanMorseByte( void );

#endif /* MORSE_H_ */