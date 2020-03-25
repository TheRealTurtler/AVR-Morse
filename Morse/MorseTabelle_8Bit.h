

#ifndef EIN_BYTE_MORSE_TABLLE_H
	#define EIN_BYTE_MORSE_TABLLE_H
/*
•    1 Byte pro Zeichen, niederwertiges Bit zuerst 
•    hat das Bit den Wert 0: Morsepunkt (dit) 
•    hat das Bit den Wert 1: Morsestrich (dah) 
•    da Morsezeichen unterschiedlich lang sind, ist ein Ende- Zeichen erforderlich; es ist ebenfalls ein 1 - Bit 
•    Beispiel Morsezeichen  "a" (dit dah):  &B00000110
•    Beispiel Morsezeichen  "9" (dah dah dah dah dit): &B00101111  
Shift nach Rechts. Wenn Wert == 1  ENDE. 
*/
#include "main.h"

typedef struct
	{
	uint8_t u8Ascii;
	uint8_t u8MCode;
	}S_MCode_T;
	
	
static const S_MCode_T PROGMEM MCA_1ByteMorseTab[]={ \
//,{??, 0b00110001}	//PAUSE	-...-
 {02, 0b00110101}	//STX	-.-.-	START(NATO)
,{03, 0b00101010}	//ETX	.-.-.	ENDE(NATO).-.-. 
,{10, 0b00011010}	//LF==CR	.-.- 
,{12, 0b00101010}	//FF	FORM_FEED	.-.-.
,{13, 0b00011010}	//CR==LF	.-.-
,{23, 0b00101010}	//EOT Block synonym== NEW PAGE= .-.-.
,{24, 0b10000000}	//CANCEL ==>SYN:ERROR	........

,{32, 0b00000001}	//SPACE = WORDSPACE"1" {keine Zeichen}

,{33, 0b01110101}	//!	-.-.--
,{34, 0b01010010}	//"	.-..-.
,{38, 0b00100010}	//& .-...
,{40, 0b00101101}	//(	-.--.
,{41, 0b01101101}	//)  -.--.-
,{43, 0b00101010}	//+	.-.-.
,{44, 0b01110011}	//, Komma --..--
,{45, 0b01100001}	//- Minus -....-
,{46, 0b01101010}	//. Punkt .-.-.-
,{47, 0b00101001}	//"/"	Division -..-.
// Ascii 48 - 57  ->  0123456789
,{48, 0b00111111}	//0	-----
,{49, 0b00111110}	//1	.----
,{50, 0b00111100}	//2	..---
,{51, 0b00111000}	//3	...--
,{52, 0b00110000}	//4	....-
,{53, 0b00100000}	//5 .....
,{54, 0b00100001}	//6 -....
,{55, 0b00100011}	//6	--...
,{56, 0b00100111}	//8	---..
,{57, 0b00101111}	//9	----.

,{58, 0b01000111}	//: DoppelPunkt	---... 
,{59, 0b01010101}	//; Semikolon	-.-.-. 
,{61, 0b00110001}	//=	-...-
,{63, 0b01001100}	//?	..--..	
,{64, 0b01010110}	//@	.--.-.

//PStrCRAscii	65-90	ABCDEFGHIJKLMNOPQRSTUVWXYZ
,{65, 0b00000110} 	//A .-
,{66, 0b00010001}	//B -...
,{67, 0b00010101}	//C -.-.
,{68, 0b00001001}	//D -.. 
,{69, 0b00000010}	//E .
,{70, 0b00010100}	//F ..-.
,{71, 0b00001011}	//G --.
,{72, 0b00010000}	//H ....
,{73, 0b00000100}	//I	..
,{74, 0b00011110}	//J	.---
,{75, 0b00001101}	//K	-.-
,{76, 0b00010010}	//L	.-..
,{77, 0b00000111}	//M	--
,{78, 0b00000101}	//N -.
,{79, 0b00001111}	//O	---
,{80, 0b00010110}	//P	.--.
,{81, 0b00011011}	//Q	--.-
,{82, 0b00001010}	//R	.-.
,{83, 0b00001000}	//S	...
,{84, 0b00000011}	//T	-
,{85, 0b00001100}	//U	..-
,{86, 0b00011000}	//V	...-
,{87, 0b00001110}	//W	.--
,{88, 0b00011001}	//X	-..-
,{89, 0b00011101}	//Y	-.--
,{90, 0b00010011}	//Z --..

,{95, 0b01101100}	//"_" Underscore	..--.-
,{96, 0b01011110}	//"'" Apostroph	.----.
,{142, 0b000011010}	//Ä	.-.-
,{152, 0b00010111}	//Ö	---.
,{154, 0b00011100}	//Ü	..--

,{185, 0b00110101}	//STX	-.-.-	START(NATO)
,{186, 0b00110001}	//PAUSE -...-
,{187, 0b00101010}	//ETX	.-.-.	ENDE(NATO).-.-. 

,{223, 0b10011000}	//ß PC-KEYBOARD ...--..
,{225, 0b10011000}	//ß  ...--..
,{228, 0b00011010}	//Ä	PC-KEYBOARD	//142	.-.-  
,{246, 0b00010111}	//Ö	PC-KEYBOARD	//152	---.  
,{252, 0b00011100}	//Ü	PC-KEYBOARD	//154	..--
//,{??, 0b}	//	
};

#define SIZE_MC_STRUCT  sizeof(S_MCode_T)
#define SIZE_CODE_TABLE	sizeof(MCA_1ByteMorseTab)
#define MORSE_CODES_IN_TABLE  (SIZE_CODE_TABLE / SIZE_MC_STRUCT)

/*
Morsetabelle:
// Print_PStrCRAscii 0 - 32 ; hier wird nur Linefeed in <kn> umgesetzt
0,0,0,0,0,0,0,0,0,0,0,0,0, 0b00101101,0,,0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0
// Print_PStrCR Ascii 32 - 47   ->   _!"#$%&// Print_PStrCR()*+,-./
0,0, 0b01010010,0,0,0,0,0, 0b00101101, 0b01101101,0, 0b00101010, 0b01110011, 0b01100001
0b01101010, 0b00101001
// Print_PStrCRAscii 58 - 64  ->  :;<=>?@
0b01000111, 0b01110011, 0b00101101, 0b00110001, 0b01101101, 0b01001100,0
// Print_PStrCRAscii 91 - 96  ->  Muell
0,0,0,0,0,0

// Print_PStrCRAscii 123 - 127  -> Muell

*/

#endif
