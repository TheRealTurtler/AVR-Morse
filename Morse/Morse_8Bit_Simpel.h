

#ifndef SIMPLE_8BIT_MORSE
	#define SIMPLE_8BIT_MORSE

#include "main.h"
// u8MorseByte = pgm_read_byte_near( MCA_8BIT_SIMPELMORSE + u8Offset)
/* 
"." Zeit 100mS
"_" = 3 Punkte 
Buchstabenlücke = 3 Punkte 
Wortlückke = 7 Punkte 

*/

#define ZAHL_OFFEST		0
#define CHAR_OFFEST		10
#define SPACE_OFFSET	36
//lese  0 = "."   1="-"   wenn 1 übrig bleibt = Fertig
static const uint8_t PROGMEM MCA_8BIT_SIMPELMORSE[]= \
// Ascii 48 - 57  ->  0123456789
 {
 0b00111111		//0	-----
, 0b00111110	//1	.----
, 0b00111100	//2	..---
, 0b00111000	//3	...--
, 0b00110000	//4	....-
, 0b00100000	//5 .....
, 0b00100001	//6 -....
, 0b00100011	//6	--...
, 0b00100111	//8	---..
, 0b00101111	//9	----.
//10
, 0b00000110 	//A .-
, 0b00010001	//B -...
, 0b00010101	//C -.-.
, 0b00001001	//D -.. 
, 0b00000010	//E .
, 0b00010100	//F ..-.
, 0b00001011	//G --.
, 0b00010000	//H ....
, 0b00000100	//I	..
, 0b00011110	//J	.---
, 0b00001101	//K	-.-
, 0b00010010	//L	.-..
, 0b00000111	//M	--
, 0b00000101	//N -.
, 0b00001111	//O	---
, 0b00010110	//P	.--.
, 0b00011011	//Q	--.-
, 0b00001010	//R	.-.
, 0b00001000	//S	...
, 0b00000011	//T	-
, 0b00001100	//U	..-
, 0b00011000	//V	...-
, 0b00001110	//W	.--
, 0b00011001	//X	-..-
, 0b00011101	//Y	-.--
, 0b00010011	//Z --..
// SPACE
, 0b00000001	//SPACE = WORDSPACE"1" {keine Zeichen}
 };

#endif