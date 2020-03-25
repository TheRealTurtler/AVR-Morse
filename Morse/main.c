/*
Christof Ermer

Bibliothek: file:///C:/WinAVR-20100110/doc/avr-libc/avr-libc-user-manual/modules.html
µController Defines, File Explorer: C:\WinAVR-20100110\avr\include\avr\iom328p.h
int8_t uint8_t int16_t uint16_t int32_t uint32_t ISR main
*/
/* -----------------------------------------------
 Programmed by Christof Ermer
 https://homepages.uni-regensburg.de/~erc24492/
  ------------------------------------------------
*/

#include "main.h"		//with Hardware assign

// Globale Variablen
volatile uint32_t gu32Ticks;
volatile BIT_FIELD_TYPE gstFlags;

char gcaMorseText[] = "sos ";

#define TICK_1S_EVENT		0x04
#define TICK_100MS_EVENT	0x20
#define TICK_FAST_EVENT		0x10

// *******************
ISR( TIMER0_COMPA_vect ) // 250*/sec
// *******************
{
	gu32Ticks++;  // ~250/Sekunde
	
	if ( !( gu32Ticks % TICKS_1000MS ) ) //Modulo ==0 ?
	{
		gstFlags.TickEvent_Flag = 1;	//Set FLAG
	};
	
	if ( !( gu32Ticks % TICKS_100MS ) ) //Modulo ==0 ?
	{
		gstFlags.TICK_100MS__Flag = 1;
		
#ifdef UART_USE_ENABLED
		UART_RX_Check(); // Polling
#endif
		
	}
	
};

// *****************************
void StartTickTimer( void )
// *****************************
{
	// Timer TickCounter starten im Compare Mode
	// M_CPU=16^6 / 256 / 250 = {ebenso} 250!Zähle 0 bis 250-->exakt 1 SeKunde
	TCNT0 = 0;
	OCR0A = 250;//Compare Value
	TCCR0A = ( 1 << WGM01 );	//CTC Mode = Compare Timer Counter mit OCR0A
	TIMSK0 = ( 1 << OCIE0A );	//Compare Interrupt aktivieren
	TCCR0B = ( 1 << CS02 );	//Vorteiler /256
	//TCCR0B = (1<< CS02) | _BV(CS00);	//Vorteiler /1024
};


// *****************
int main( void )
// *****************
{

#ifdef UART_USE_ENABLED
	uart_init( UART_BAUD_SELECT_DOUBLE_SPEED( 115200UL, F_CPU ) );
	//uart_init(UART_BAUD_SELECT( 9600UL, F_CPU) );
	ResetRxBuff();
#endif
	
	wdt_reset();
	wdt_enable( WDTO_1S ); //Totmannknopf
	wdt_reset();
	
	StartTickTimer();
	
	gstFlags.Geschwaetzig = 1;
	
#ifdef MORSE_USED
	InitMorse();
#endif
	
	sei();  // Interrupt einschalten
	
	//SendMorseString("SOS", 4);
	//SendMorseString(gcaMorseText, sizeof(gcaMorseText));
	//SendMorseChar('O');
	
	while ( 1 ) /* loop forever */
	{
		wdt_reset();
		
		if (	gstFlags.TICK_100MS__Flag ) // Ask Flag == TRUE
		{
			gstFlags.TICK_100MS__Flag = 0;
			
			
		};
		
		if ( gstFlags.TickEvent_Flag ) // ?-->1
		{
			gstFlags.TickEvent_Flag = 0; // clear Flag
			
#ifdef UART_USE_ENABLED
			if ( gstFlags.Geschwaetzig )
			{
				uart_puts_p( PSTR( "Tick: " ) );
				uart_puts( ULongToNumStr( gu32Ticks ) );
				CRLF();
				
			};
			
#ifdef MORSE_USED
			//SendMorseString(gcaMorseText, sizeof(gcaMorseText));
			SendMorseString(gcaMorseText, sizeof(gcaMorseText));
			//SendMorseChar('A');
#endif
			
#endif
		};
		
#ifdef UART_USE_ENABLED
		if( gsCmd.UartCREventFlag ) // bearbeite Carriage Return Order
		{
			gsCmd.UartCREventFlag = 0;
			CheckOrder();
			ResetRxBuff();
		};
#endif
	}; //while
	
};
// ******** MAIN END ********