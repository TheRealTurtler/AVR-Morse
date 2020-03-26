/*
 * Morse.c
 *
 * Created: 17.03.2020 14:30:30
 *  Author: Michael
 */

#include "Morse.h"

#ifdef MORSE_USED


// ================ Globale Variablen =====================

volatile MORSE_FLAGS gstMorseFlags;
MORSE_DATA gstMorseData;


// ================ Interrupts ============================

ISR( TIMER2_COMPA_vect )
{
	static uint32_t u32MorseTicks = 0;
	
	if ( gstMorseFlags.Start_F )
	{
		if ( gstMorseFlags.ZeichenPause_F
		        || gstMorseFlags.BuchstabenPause_F
		        || gstMorseFlags.WortPause_F )
		{
			CLEAR_MORSE_BIT();
			CLEAR_MORSE_BUSY_BIT();
		}
		else
		{
			SET_MORSE_BIT();
			SET_MORSE_BUSY_BIT();
		}
		
		u32MorseTicks = 0;
		
		gstMorseFlags.Start_F = 0;
		gstMorseFlags.Aktiv_F = 1;
	}
	
	if ( gstMorseFlags.Aktiv_F )
	{
		u32MorseTicks++;
		
		if ( u32MorseTicks > gstMorseData.u32SignalLaenge )
		{
			CLEAR_MORSE_BIT();
			CLEAR_MORSE_BUSY_BIT();
			
			gstMorseFlags.Aktiv_F = 0;
			gstMorseFlags.Start_F = 0;
			gstMorseFlags.Ready_F = 1;
		}
		
#ifdef MORSE_SOUND_USED
		if ( ! ( u32MorseTicks % gstMorseData.u16TonFreq )
		        && ! ( gstMorseFlags.ZeichenPause_F )
		        && ! ( gstMorseFlags.BuchstabenPause_F )
		        && ! ( gstMorseFlags.WortPause_F ) )
		{
			// Ton ausgeben
		}
#endif
		
	}
}


// ================ Funktionen ============================

// **************************
void InitMorse( void )
// **************************
{
	INIT_MORSE_PORT();
	INIT_MORSE_BUSY_PORT();
	
	// init ton
	
	CLEAR_MORSE_BIT();
	CLEAR_MORSE_BUSY_BIT();
	
	// clear ton
	
	gstMorseFlags.Punkt_F = 0;
	gstMorseFlags.Strich_F = 0;
	gstMorseFlags.ZeichenPause_F = 0;
	gstMorseFlags.BuchstabenPause_F = 0;
	gstMorseFlags.WortPause_F = 0;
	
	gstMorseFlags.Ready_F = 1;
	gstMorseFlags.Stop_F = 1;
	gstMorseFlags.Aktiv_F = 0;
	gstMorseFlags.Start_F = 0;
	
	SetMorseSpeed( DEFAULT_MORSE_PUNKT_LAENGE_MS );
	
// 	gstMorseData.u32Geschwindigkeit = DEFAULT_MORSE_PUNKT_LAENGE;
// 	gstMorseData.u32SignalLaenge = gstMorseData.u32Geschwindigkeit;
//
// 	gstMorseData.u32PunktLaenge = gstMorseData.u32Geschwindigkeit;
// 	gstMorseData.u32StrichLaenge = gstMorseData.u32Geschwindigkeit * 3;
// 	gstMorseData.u32ZeichenPauseLaenge = gstMorseData.u32Geschwindigkeit;
//
// 	// Buchstaben- und Wortpause sind verkürzt, da automatisch  an jedes Zeichen eine Zeichenpause angehängt wird
// 	gstMorseData.u32BuchstabenPauseLaenge = gstMorseData.u32Geschwindigkeit * 2;
// 	gstMorseData.u32WortPauseLaenge = gstMorseData.u32Geschwindigkeit * 6;

	InitTimer_72KHZ();
}

// **************************
void InitTimer_72KHZ( void )
// **************************
{
	// Timer Port Ausgabe
	TIMER2_DDRX |= TIMER2_OCA_BIT;
	
	TCNT2 = 0;
	OCR2A = 222;
	TCCR2A = _BV( WGM21 ) | _BV ( COM2A0 );	// CTC, toggle OC2A on compare match
	TCCR2B = _BV( CS20 );					// Vorteiler /1
	TIMSK2 = _BV( OCIE2A );					// Interrupt
	
	sei();
}


// **************************
uint8_t AsciiToMorse ( uint8_t u8Ascii )
// **************************
{
	uint8_t u8Offset = 0;
	
	if ( isalpha( u8Ascii ) )
	{
		u8Offset = CHAR_OFFEST + toupper( u8Ascii ) - 'A';
	}
	else if ( isdigit( u8Ascii ) )
	{
		u8Offset = ZAHL_OFFEST + u8Ascii - '0';
	}
	else if ( isblank( u8Ascii ) )
	{
		u8Offset = SPACE_OFFSET;
	}
	else
	{
#ifdef UART_USE_ENABLED
		uart_puts_p( PSTR( "Zeichen nicht gefunden." ) );
		CRLF();
		
		return 0;
#endif
	}

/*	
#ifdef UART_USE_ENABLED
	strcpy( gcaNumStr, UIntToNumStr( u8Offset ) );
	uart_puts( gcaNumStr );
	CRLF();
#endif
*/
	
	return pgm_read_byte_near( MCA_8BIT_SIMPELMORSE + u8Offset );
}


// **************************
void SendMorseChar( uint8_t u8Ascii )
// **************************
{
	gstMorseData.u8ScanVal = AsciiToMorse( u8Ascii );
	gstMorseData.u8MorseVal = gstMorseData.u8ScanVal;
	gstMorseFlags.Byte_Ok_F = 1;
	
	if ( gstMorseData.u8ScanVal == 0b00000001 )
	{
		// Leerzeichen -> naechstes Wort
		
#ifdef UART_USE_ENABLED
		uart_puts_p( PSTR( "Leerzeichen" ) );
		CRLF();
#endif
		
		gstMorseData.u32SignalLaenge = gstMorseData.u32WortPauseLaenge;
		
		gstMorseFlags.Punkt_F = 0;
		gstMorseFlags.Strich_F = 0;
		gstMorseFlags.ZeichenPause_F = 0;
		gstMorseFlags.BuchstabenPause_F = 0;
		gstMorseFlags.WortPause_F = 1;
		
		gstMorseFlags.Ready_F = 0;
		//gstMorseFlags.Stop_F = 0;
		//gstMorseFlags.Aktiv_F = 0;
		gstMorseFlags.Start_F = 1;
		
		while ( !gstMorseFlags.Ready_F )
		{
			// Warten bis senden fertig
			wdt_reset();
			
			// cli() und sei(), verwenden, da interrupt port setzen unterbrechen kann
			cli();
			TOGGLE_MORSE_BUSY_BIT();
			sei();
		}
		
		cli();
		CLEAR_MORSE_BUSY_BIT();
		sei();
		
		gstMorseData.u8ScanVal = 0;
		
		return;
	}
	
	if ( gstMorseFlags.Byte_Ok_F )
	{
		while ( gstMorseData.u8ScanVal > 1 )
		{
			ScanMorseByte();
			
			// Punkt oder Strich senden
			
			gstMorseFlags.Ready_F = 0;
			//gstMorseFlags.Stop_F = 0;
			//gstMorseFlags.Aktiv_F = 0;
			gstMorseFlags.Start_F = 1;
			
			while ( !gstMorseFlags.Ready_F )
			{
				// Warten bis senden fertig
				wdt_reset();
				
				// cli() und sei(), verwenden, da interrupt port setzen unterbrechen kann
				cli();
				TOGGLE_MORSE_BUSY_BIT();
				sei();
			}
			
			cli();
			CLEAR_MORSE_BUSY_BIT();
			sei();
			
			// Zeichenpause senden
			
			gstMorseData.u32SignalLaenge = gstMorseData.u32ZeichenPauseLaenge;
			gstMorseData.u16TonFreq = 0;		// Ton
			
			gstMorseFlags.Punkt_F = 0;
			gstMorseFlags.Strich_F = 0;
			gstMorseFlags.ZeichenPause_F = 1;
			gstMorseFlags.BuchstabenPause_F = 0;
			gstMorseFlags.WortPause_F = 0;
			
			gstMorseFlags.Ready_F = 0;
			//gstMorseFlags.Stop_F = 0;
			//gstMorseFlags.Aktiv_F = 0;
			gstMorseFlags.Start_F = 1;
			
			while ( !gstMorseFlags.Ready_F )
			{
				// Warten bis senden fertig
				wdt_reset();
				
				// cli() und sei(), verwenden, da interrupt port setzen unterbrechen kann
				cli();
				TOGGLE_MORSE_BUSY_BIT();
				sei();
			}
			
			cli();
			CLEAR_MORSE_BUSY_BIT();
			sei();
		}
		
		// Buchstabenpause senden
		
		gstMorseData.u32SignalLaenge = gstMorseData.u32BuchstabenPauseLaenge;
		gstMorseData.u16TonFreq = 0;		// Ton
		
		gstMorseFlags.Punkt_F = 0;
		gstMorseFlags.Strich_F = 0;
		gstMorseFlags.ZeichenPause_F = 0;
		gstMorseFlags.BuchstabenPause_F = 1;
		gstMorseFlags.WortPause_F = 0;
		
		gstMorseFlags.Ready_F = 0;
		//gstMorseFlags.Stop_F = 1;
		//gstMorseFlags.Aktiv_F = 0;
		gstMorseFlags.Start_F = 1;
		
		while ( !gstMorseFlags.Ready_F )
		{
			// Warten bis senden fertig
			wdt_reset();
			
			// cli() und sei(), verwenden, da interrupt port setzen unterbrechen kann
			cli();
			TOGGLE_MORSE_BUSY_BIT();
			sei();
		}
		
		cli();
		CLEAR_MORSE_BUSY_BIT();
		sei();
	}
	else
	{
		gstMorseFlags.Punkt_F = 0;
		gstMorseFlags.Strich_F = 0;
		gstMorseFlags.ZeichenPause_F = 0;
		gstMorseFlags.BuchstabenPause_F = 0;
		gstMorseFlags.WortPause_F = 0;
		
		gstMorseFlags.Ready_F = 1;
		//gstMorseFlags.Stop_F = 1;
		//gstMorseFlags.Aktiv_F = 0;
		gstMorseFlags.Start_F = 0;
	}
}


// **************************
void ScanMorseByte( void )
// **************************
{
	if ( gstMorseData.u8ScanVal & 0x01 )
	{
		// Letztes Bit = 1 -> Strich
		
#ifdef UART_USE_ENABLED
		uart_puts_p( PSTR( "Strich" ) );
		CRLF();
#endif
		
		gstMorseData.u32SignalLaenge = gstMorseData.u32StrichLaenge;
		gstMorseData.u16TonFreq = TON_STRICH_FREQ;
		
		gstMorseFlags.Punkt_F = 0;
		gstMorseFlags.Strich_F = 1;
	}
	else
	{
		// Letztes Bit = 0 -> Punkt
		
#ifdef UART_USE_ENABLED
		uart_puts_p( PSTR( "Punkt" ) );
		CRLF();
#endif
		
		gstMorseData.u32SignalLaenge = gstMorseData.u32PunktLaenge;
		gstMorseData.u16TonFreq = TON_PUNKT_FREQ;
		
		gstMorseFlags.Punkt_F = 1;
		gstMorseFlags.Strich_F = 0;
	}
	
	gstMorseFlags.ZeichenPause_F = 0;
	gstMorseFlags.BuchstabenPause_F = 0;
	gstMorseFlags.WortPause_F = 0;
	
	gstMorseData.u8ScanVal >>= 1;		// BitShift um 1 nach rechts
}


// **************************
void SendMorseString( char* caString, uint8_t u8SizeOfString )
// **************************
{
	gstMorseFlags.Stop_F = 0;
	
	for ( uint8_t u8TextPos = 0; u8TextPos < u8SizeOfString - 1; u8TextPos++ )
	{
		SendMorseChar( *( caString + u8TextPos ) );
		
		while ( !gstMorseFlags.Ready_F )
		{
			wdt_reset();
			
			// cli() und sei(), verwenden, da interrupt port setzen unterbrechen kann
			cli();
			TOGGLE_MORSE_BUSY_BIT();
			sei();
		}
		
		cli();
		CLEAR_MORSE_BUSY_BIT();
		sei();
	}
	
	gstMorseFlags.Stop_F = 1;
}


// **************************
void SetMorseSpeed( uint16_t u16SpeedMS )
// **************************
{
	gstMorseData.u32Geschwindigkeit = ( uint32_t ) ( u16SpeedMS *
	                                  ( TIMER_FRQ_KHZ / 2U ) );
	                                  
	gstMorseData.u32SignalLaenge = gstMorseData.u32Geschwindigkeit;
	
	gstMorseData.u32PunktLaenge = gstMorseData.u32Geschwindigkeit;
	gstMorseData.u32StrichLaenge = gstMorseData.u32Geschwindigkeit * 3;
	gstMorseData.u32ZeichenPauseLaenge = gstMorseData.u32Geschwindigkeit;
	
	// Buchstaben- und Wortpause sind verkürzt, da automatisch an jedes Zeichen eine Zeichenpause angehängt wird
	// Wortpause zusätzlich verkürzt, da nach jedem Buchstaben eine Buchstabenpause angehängt wird
	gstMorseData.u32BuchstabenPauseLaenge = gstMorseData.u32Geschwindigkeit * 2;
	gstMorseData.u32WortPauseLaenge = gstMorseData.u32Geschwindigkeit * 4;
}

#endif