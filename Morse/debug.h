

// Version 12.02.2020
#ifndef DEBUG_TOOLS_HEADER
	#define DEBUG_TOOLS_HEADER


/*
Beispiel:
uart_puts( ByteToBin( gu8DS1337_Status )); uart_putc(13);		
Toggle_Bit(2);
*/

// ***  Schalte ein was du brauchst  ***
#define TOGGLE_FLAG_USED
#define TOGGLETIME_FLAG_USED
// #define TOGGLE_STATIC_BIT_USED
 #define BYTE_TO_BIN_USED
 #define WORD_TO_BIN_USED
// #define BYTE_TO_HEX_USED
// #define SHOW_BYTE_USED
// #define LED_LEISTE_USED

//PC4=SDA, PC5=SCL, PB5=interne LED
#define TOGGLE_PIN		5  //Pin Nummer
#define TOGGLE_PORT		PORTB
#define TOGGLE_DIRREG	DDRB
#define TOOGGLE_TIME_US	1

#ifdef TOGGLE_FLAG_USED
extern void Toggle_Bit(uint8_t u8Cnt);
#endif

#ifdef TOGGLETIME_FLAG_USED
extern void ToggleTime_Bit(uint8_t u8Cnt, uint8_t u8Time);
#endif

#ifdef TOGGLE_STATIC_BIT_USED
extern void Toggle_On(void);
extern void Toggle_Off(void);
#endif

#ifdef BYTE_TO_BIN_USED
extern char * ByteToBin(uint8_t u8Byte);
#endif

#ifdef WORD_TO_BIN_USED
extern char* WordToBin(uint16_t u16Word);
#endif


#ifdef BYTE_TO_HEX_USED
extern char * ByteToHex(uint8_t u8Byte);
#endif

#ifdef SHOW_BYTE_USED
extern void ShowByte(uint8_t u8Byte, char* pText, uint8_t u8Nr);
#endif


#ifdef LED_LEISTE_USED
//LEDLEISTE 
// split in 2*4 Bit
#define LOW_DDR		DDRD
#define LOW_PORT	PORTD

#define HIGH_DDR	DDRC
#define HIGH_PORT	PORTC

#define LOW_TOPORT_BITS		0b00111100
#define HIGH_TOPORT_BITS	0x0F
extern void LEDLeiste_Init(void);
extern void LEDLeisteTx(uint8_t u8Byte);
#endif 

//void ShowByte(uint8_t u8Byte, uint8_t u8Nr);

#endif //DEBUG_TOOLS_HEADER