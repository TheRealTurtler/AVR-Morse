

// Verison 05.03.2020
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <stdint.h> 
#include <stdlib.h>
#include <util/delay.h>
#include "debug.h"

//mit Oszilloscope Testpin am µController an geeigneter Steelle watchen.
// Zahl der Toggles gibt Position an. 
#ifdef  TOGGLE_FLAG_USED
// ****************************************************************
void Toggle_Bit(uint8_t u8Cnt)
// ****************************************************************
{ // #include <util\delay.h>  muss da sein
TOGGLE_DIRREG |= (1<< TOGGLE_PIN); //Out
while(u8Cnt--)
	{
	TOGGLE_PORT |= (1<< TOGGLE_PIN);
	_delay_us( TOOGGLE_TIME_US );
	TOGGLE_PORT &= ~(1<< TOGGLE_PIN);
	_delay_us( TOOGGLE_TIME_US );
	};
};
#endif

#ifdef TOGGLETIME_FLAG_USED
// ****************************************************************
void ToggleTime_Bit(uint8_t u8Cnt, uint8_t u8Time)
// ****************************************************************
{ // #include <util\delay.h>  muss da sein
TOGGLE_DIRREG |= (1<< TOGGLE_PIN); // Out
while(u8Cnt--)
	{
	TOGGLE_PORT |= (1<< TOGGLE_PIN);
	_delay_us( u8Time );
	TOGGLE_PORT &= ~(1<< TOGGLE_PIN);
	_delay_us( u8Time );
	};
};
#endif

#ifdef TOGGLE_STATIC_BIT_USED
// ***********************
void Toggle_On(void)
// ***********************
{
TOGGLE_PORT |= TOGGLE_BIT;
};

// ***********************
void Toggle_Off(void)
// ***********************
{
TOGGLE_PORT &= ~TOGGLE_BIT;
};
#endif 

#ifdef BYTE_TO_BIN_USED
// *******************************************************************
char * ByteToBin(uint8_t u8Byte)
// *******************************************************************
{
static char caHex[9]; //groesse 8+1
char * pcParser = caHex;
int8_t i8NN=7;
do
	{
	if( u8Byte & (1 << i8NN) )
		{
		*pcParser = '1';
		}
	else
		{
		*pcParser = '0';
		};
	pcParser++;
	}while(i8NN--);
caHex[8]=0; //Terminierung
return caHex;
};
#endif 


//WORD Anzeige in Binaerer Form
#ifdef WORD_TO_BIN_USED
// *******************************************************************
char * WordToBin(uint16_t u16Word)
// *******************************************************************
{
static char ucaBin[18]; // Array  zeigt Lücke 00000000 00000000
char * pcParser = ucaBin;
uint8_t u8NN=15;
do
	{
/*	
	if(u8NN == 7)
		{*pcParser =' '; pcParser++;}; // Füge Lücke ein im BYTE
*/
	if(u16Word & (1<<u8NN) )
		{
		*pcParser = '1';
		}
	else
		{
		*pcParser = '0';
		};
	pcParser++;
	}while(u8NN--);
ucaBin[17]=0;
return  ucaBin;
};
#endif


#ifdef BYTE_TO_HEX_USED
static char caHex[5]; //groesse 8+1
// **************************
//Hilfsfuktion;
void FillHex(uint8_t u8Pos, uint8_t u8Mem)
// **************************
{
if(u8Mem > 9 )
	{
	switch(u8Mem)
		{
		case 10:
			{
			caHex[u8Pos]='A';
			break;
			};
		case 11:
			{
			caHex[u8Pos]='B';
			break;
			};
		case 12:
			{
			caHex[u8Pos]='C';
			break;
			};
		case 13:
			{
			caHex[u8Pos]='D';
			break;
			};
		case 14:
			{
			caHex[u8Pos]='E';
			break;
			};
		case 15:
			{
			caHex[u8Pos]='F';
			break;
			};
		};
	}
else
	{
	caHex[u8Pos]=u8Mem+48; //ASCII ZAHL 0
	};
}

// ******************************
char * ByteToHex(uint8_t u8Byte)
// ******************************
{
caHex[0]='0';
caHex[1]='x';
FillHex(2,(u8Byte & 0xF0) >> 4);
FillHex(3,u8Byte & 0x0F);
caHex[4]=0;
return caHex;
};
#endif 


#ifdef LED_LEISTE_USED
// ************************
void LEDLeiste_Init(void)
// ************************
{
LOW_DDR |= LOW_TOPORT_BITS;
HIGH_DDR |= HIGH_TOPORT_BITS;
};

// ************************
void LEDLeisteTx(uint8_t u8Byte)
// ************************
{
// split in 2*4 Bit
LEDLeiste_Init();
LOW_PORT = (LOW_PORT & ~LOW_TOPORT_BITS) |  ((u8Byte & 0x0F)  << 2); // 2 Bit  nach oben
HIGH_PORT = (HIGH_PORT & ~HIGH_TOPORT_BITS) | ((u8Byte & 0xF0) >> 4); // 4 Bit nach unten
};
#endif


/*
// *******************************************************************
char * WordToBin(uint16_t u16Word)
// *******************************************************************
{
static char ucBin[18]; // Array  zeigt Lücke 0000 0000
char * pcParser = ucBin;
uint8_t u8NN=15;

do
{
if(u8NN == 7)
	{*pcParser =' '; pcParser++;}; // Füge Lücke ein im BYTE
if(u16Word & (1<<u8NN) )
	{
	*pcParser = '1';
	}
else
	{
	*pcParser = '0';
	};
pcParser++;
}while(u8NN--);
ucBin[17]=0;
return  ucBin;
};

uart_putc(13); uart_putc(10);
uart_puts(  dtostrf(  RAND_MAX,10,5,gcaNumStr) );
*/