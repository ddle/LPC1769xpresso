///*
//=================================================================================
// Name        : spiLCDtest.c
// Author      : Le Dang Dung
// Version     : 0.1
// Copyright   : ...
// Description :
//     A simple PCD8544 LCD (Nokia3310) driver. Target board is LPCxpresso LPC1769.
//     This code uses SSP API provided by NXP.
//
//     + Mode: Master/TX-only/use-CS
//	 + Device : PCD8544-nokia3310
//	 + Connection:
//
//	 LCD pins      LPC1769
//	 LCD1 - GND    J6.1  - GND
//	 LCD2 - VCC    J6.28 - 3.3V
//	 LCD3 - CLK    J6.7  - SCK1/P0.7
//	 LCD4 - Din    J6.5  - MOSI1/P0.9
//	 LCD5 - D/C    J6.9  - P0.0
//	 LCD6 - CS     J6.8  - SSEL1/P0.6
//	 LCD7 - RST    J6.10 - P0.1
//	 LCD8 - LED    J6.28 - 3.3V
//
// References  :
// http://www.arduino.cc/playground/Code/PCD8544
// http://ladyada.net/products/nokia5110/
//
//=================================================================================
// */
//
//#include <cr_section_macros.h>
//#include <NXP/crp.h>
//#include "LPC17xx.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include "timer.h"
//#include "pin.h"
//#include <ctype.h>
//__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
//
//#include "ssp.h"
//#include "PCD8544.h"
//#include "keypad.h"
//
//
//
//
//
////
////#define GPIOCLR(PORT,PIN)                ( (PORT)->FIOCLR ) |=  ( 0x1 << (PIN) )
////#define GPIOSET(PORT,PIN)                ( (PORT)->FIOSET ) |=  ( 0x1 << (PIN) )
////// this one might be slower, use when VAL is unknown at compile time, if known use the above
////#define digitalWrite(PORT,PIN,VAL)       ( ((VAL) == LOW) ? ( ((PORT)->FIOCLR) |=  (0x1 << (PIN)) ) : ( ((PORT)->FIOSET) |=  (0x1 << (PIN)) ) )
////#define GPIODIR(PORT,PIN,MODE)           ( ((MODE) == INPUT) ? ( ((PORT)->FIODIR) &= ~(0x1 << (PIN)) ) : ( ((PORT)->FIODIR) |=  (0x1 << (PIN)) ) )
////
////// useful macros for our program
////#define PIN_SCE   6
////#define PIN_RESET 1
////#define PIN_DC    0
////#define PIN_SDIN  9
////#define PIN_SCLK  7
////
////#define LCD_C     0  // cmd
////#define LCD_D     1  // data
////
////#define LOW       0
////#define HIGH      1
////#define INPUT     0
////#define OUTPUT    1
////#define LCD_X     84
////#define LCD_Y     48
////#define LSBFIRST  0
////#define MSBFIRST  1
////#define DATAPORT  LPC_GPIO0
////
////static const  char ASCII[][5] =
////{
//// {0x00, 0x00, 0x00, 0x00, 0x00} // 20
////,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
////,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
////,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
////,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
////,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
////,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
////,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
////,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
////,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
////,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
////,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
////,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
////,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
////,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
////,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
////,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
////,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
////,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
////,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
////,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
////,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
////,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
////,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
////,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
////,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
////,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
////,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
////,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
////,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
////,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
////,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
////,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
////,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
////,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
////,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
////,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
////,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
////,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
////,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
////,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
////,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
////,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
////,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
////,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
////,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
////,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
////,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
////,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
////,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
////,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
////,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
////,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
////,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
////,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
////,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
////,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
////,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
////,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
////,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
////,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
////,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
////,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
////,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
////,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
////,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
////,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
////,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
////,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
////,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
////,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
////,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
////,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
////,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
////,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
////,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
////,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
////,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
////,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
////,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
////,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
////,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
////,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
////,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
////,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
////,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
////,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
////,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
////,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
////,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
////,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
////,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
////,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
////,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
////,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
////,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
////};
//
////
////// write one byte to LCD, either commnand or data
////static void LCDWriteByte(uint8_t DC,  char data)
////{
////  // set D/C mode
////  digitalWrite(LPC_GPIO0,0, DC); // P0.0 set to whatever DC's value
////
////  // assert chip select, ignore if USE_CS
////  // note: configure  P0.6 as GPIO/Output has been taken care of, see ssp.c
////#if !USE_CS
////  GPIOCLR(LPC_GPIO0,6);
////#endif
////
////  // if NOT busy and TX FIFO not full.
////  while ( (LPC_SSP1->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
////
////  LPC_SSP1->DR = data;
////
////  while ( (LPC_SSP1->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
////
////  // de-assert chip select
////#if !USE_CS
////  GPIOSET(LPC_GPIO0,6);
////#endif
////
////  return;
////}
////
////// write buffer (data) to LCD
////static void LCDWriteBuf(  char *buf, uint32_t Length )
////{
////  uint32_t i;
////  // set D/C mode
////  digitalWrite(LPC_GPIO0,0, LCD_D); // P0.0 set to whatever DC's value
////
////#if !USE_CS
////  GPIOCLR(LPC_GPIO0,6);
////#endif
////
////  for ( i = 0; i < Length; i++ )
////  {
////	  /* Move on only if NOT busy and TX FIFO not full. */
////	  while ( (LPC_SSP1->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
////
////	  LPC_SSP1->DR = *buf;
////	  buf++;
////
////	  while ( (LPC_SSP1->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
////  }
////
////#if !USE_CS
////  GPIOSET(LPC_GPIO0,6);
////#endif
////
////  return;
////}
////
////void LCDInit(void)
////{
////  // done in SSP1init():
////  //pinMode(PIN_SCE, OUTPUT);
////  //LPC_PINCON->PINSEL0 &= ~(0x3<<12);   // clear bit 13:12 to select GPIO mode
////  //LPC_GPIO0->FIODIR |= (0x1<<6);	     // (GPIO) P0.6 as Outputs
////  //pinMode(PIN_SDIN, OUTPUT);
////  //pinMode(PIN_SCLK, OUTPUT);
////
////  //pinMode(PIN_RESET, OUTPUT);
////	GPIODIR(LPC_GPIO0,1,OUTPUT);
////  //pinMode(PIN_DC, OUTPUT);
////	GPIODIR(LPC_GPIO0,0,OUTPUT);
////
////  //digitalWrite(PIN_RESET, LOW);
////  digitalWrite(LPC_GPIO0,1,LOW);
////  //digitalWrite(PIN_RESET, HIGH);
////  digitalWrite(LPC_GPIO0,1,HIGH);
////  LCDWriteByte(LCD_C, 0x20);
////  LCDWriteByte(LCD_C, 0x0C);
////}
////
////void LCDChar( char character)
////{
////  LCDWriteByte(LCD_D, 0x00);
////  uint8_t index;
////  for (index = 5; index > 0; index--)
////  {
////    LCDWriteByte(LCD_D, ASCII[character - 0x20][index]);
////    //LCDWriteBuf(ASCII[character - 0x20],5)
////  }
////  LCDWriteByte(LCD_D, 0x00);
////}
////
////void LCDString( char *characters)
////{
////  while (*characters)
////  {
////    LCDChar(*characters++);
////  }
////}
////
////void LCDClear(void)
////{
////  uint32_t index;
////  for (index =  LCD_X * LCD_Y / 8; index > 0 ; index--)
////  {
////    LCDWriteByte(LCD_D, 0x00);
////  }
////}
//
//
//
////========================
//
//
//
////
////void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
////{
////	uint8_t i;
////	uint32_t j;
////
////	for (i = 0; i < 8; i++)  {
////		if (bitOrder == LSBFIRST)
////			digitalWrite(DATAPORT, dataPin, !!(val & (1 << i)));
////		else
////			digitalWrite(DATAPORT, dataPin, !!(val & (1 << (7 - i))));
////
////		digitalWrite(DATAPORT,clockPin, HIGH);
////		for (j = 1000; j > 0; j--);
////		digitalWrite(DATAPORT,clockPin, LOW);
////	}
////}
////
////void LcdWrite(uint8_t dc, char data)
////{
////  digitalWrite(DATAPORT,PIN_DC, dc);
////  digitalWrite(DATAPORT,PIN_SCE, LOW);
////  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
////  digitalWrite(DATAPORT,PIN_SCE, HIGH);
////}
////
////void LcdCharacter(char character)
////{
////  int index;
////  LcdWrite(LCD_D, 0x00);
////  for (index = 0; index < 5; index++)
////  {
////    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
////  }
////  LcdWrite(LCD_D, 0x00);
////}
////
////void LcdClear(void)
////{
////  int index;
////  for (index = 0; index < LCD_X * LCD_Y / 8; index++)
////  {
////    LcdWrite(LCD_D, 0x00);
////  }
////}
////
////void LcdString(char *characters)
////{
////  while (*characters)
////  {
////    LcdCharacter(*characters++);
////  }
////}
////
////void LcdInitialise(void)
////{
////  uint32_t j;
////  LPC_PINCON->PINSEL0 &= ~(0x3<<(PIN_SCE*2));   // clear bit 13:12 to select P0.6 (CS) as GPIO mode
////  LPC_PINCON->PINSEL0 &= ~(0x3<<(PIN_RESET*2));
////  LPC_PINCON->PINSEL0 &= ~(0x3<<(PIN_DC*2));
////  LPC_PINCON->PINSEL0 &= ~(0x3<<(PIN_SDIN*2));
////  LPC_PINCON->PINSEL0 &= ~(0x3<<(PIN_SCLK*2));
////
////  LPC_GPIO0->FIODIR |= (0x1<<PIN_SCE);	     // (GPIO) P0.6 as Outputs
////  LPC_GPIO0->FIODIR |= (0x1<<PIN_RESET);
////  LPC_GPIO0->FIODIR |= (0x1<<PIN_DC);
////  LPC_GPIO0->FIODIR |= (0x1<<PIN_SDIN);
////  LPC_GPIO0->FIODIR |= (0x1<<PIN_SCLK);
////
////
////
////
//////  pinMode(PIN_SCE, OUTPUT);
//////  pinMode(PIN_RESET, OUTPUT);
//////  pinMode(PIN_DC, OUTPUT);
//////  pinMode(PIN_SDIN, OUTPUT);
//////  pinMode(PIN_SCLK, OUTPUT);
////
////  digitalWrite(DATAPORT,PIN_RESET, LOW);
////  for (j = 10000; j > 0; j--);
////  digitalWrite(DATAPORT,PIN_RESET, HIGH);
////  LcdWrite(LCD_C, 0x20);
////  LcdWrite(LCD_C, 0x0C);
////}
//
//#define ROWS  4 //four rows
//#define COLS  3 //three columns
//
//// Define the keymaps.  The blank spot (lower left) is the space character.
//char alphaKeys[4][3] = {
//		{ 'a','d','g' },
//		{ 'j','m','p' },
//		{ 's','v','y' },
//		{ '*',' ','#' }
//};
//
//char numberKeys[4][3] = {
//		{ '1','2','3' },
//		{ '4','5','6' },
//		{ '7','8','9' },
//		{ '*','0','#' }
//};
//
//bool alpha = false;   // Start with the numeric keypad.
//
//// Take care of some special events.
//void keypadEvent(KeypadEvent key) {
//	static char virtKey = NO_KEY;      // Stores the last virtual key press. (Alpha keys only)
//	static char physKey = NO_KEY;      // Stores the last physical key press. (Alpha keys only)
//	static char buildStr[100];
//	static uint8_t buildCount;
//	static uint8_t pressCount;
//
//	switch (KeyPadGetState())
//	{
//	case PRESSED:
//		if (isalpha(key))
//		{              // This is a letter key so we're using the letter keymap.
//			if (physKey != key)
//			{        // New key so start with the first of 3 characters.
//				pressCount = 0;
//				virtKey = key;
//				physKey = key;
//			}
//			else
//			{                       // Pressed the same key again...
//				virtKey++;               // so select the next character on that key.
//				pressCount++;            // Tracks how many times we press the same key.
//			}
//			if (pressCount > 2)
//			{        // Last character reached so cycle back to start.
//				pressCount = 0;
//				virtKey = key;
//			}
//			//Serial.print(virtKey);       // Used for testing.
//
//			LCDdrawchar(0,40,virtKey);
//			LCDdisplay();
//		}
//		if (isdigit(key) || key == ' ' || key == '.')
//		{
//			//Serial.print(key);
//			LCDdrawchar(0,40,key);
//			LCDdisplay();
//		}
//		if (key == '#')
//		{
//			//Serial.println();
//			LCDdrawchar(0,40,'#'); // send...
//			LCDdisplay();
//		}
//		if (key == '*') // del
//		{
//			//Serial.println();
//			if (buildCount > 0)
//			{
//				buildStr[buildCount-1] = '\0';
//				buildCount--;
//				LCDclear();
//				LCDdrawstring(0,0,buildStr);
//				LCDdisplay();
//			}
//		}
//		break;
//
//	case HOLD:
//		if (key == '#')
//		{                   // Toggle between keymaps.
//			if (alpha == true)
//			{            // We are currently using a keymap with letters
//				KeyPadBegin(*numberKeys);   // and want to change to numbers.
//				alpha = false;
//			}
//			else
//			{                          // Or, we are currently using a keymap with numbers
//				KeyPadBegin(*alphaKeys);    // and want to change to letters.
//				alpha = true;
//			}
//			LCDdrawchar(0,40,'>'); // switch...
//			LCDdisplay();
//		}
//		else
//		{                             // Some key other than '#' was pressed.
//			if(key != '*')
//			{
//				//buildStr[buildCount++] = (isalpha(key)) ? (virtKey) : key;
//				//buildStr[buildCount] = '\0';
//				//Serial.println();
//				//Serial.println(buildStr);
//				char c;
//				if (isalpha(key))
//				{
//					uint8_t row_index = KeyPadCurrentRow();
//					uint8_t col_index = KeyPadCurrentCol();
//					buildStr[buildCount] = numberKeys[row_index][col_index];
//				}
//
//				else
//				{
//					buildStr[buildCount] = key;
//				}
//
//				buildCount++;
//				buildStr[buildCount] = '\0';
//				LCDdrawstring(0,0,buildStr);
//				LCDdisplay();
//			}
//		}
//		break;
//
//	case RELEASED:
//		if (buildCount >= sizeof(buildStr))  buildCount = 0;    // Our string is full. Start fresh.
//		break;
//
//	}  // end switch-case
//}  // end keypad events
//
//
///******************************************************************************
// **   Main Function  main()
// ******************************************************************************/
//int main (void)
//{
//
//	// __beginner notes__
//	// e.g 1: switching between pin functions, see user manual, section 8.5
//	//    LPC_PINCON->PINSEL0 &= ~(0x3<<12);   // clear bit 13:12 to select GPIO mode
//	//	LPC_GPIO0->FIODIR |= (0x1<<6);	     // (GPIO) P0.6 as Outputs
//	//	LPC_PINCON->PINSEL0 |= 0x2<<14;      // SCK1
//	//	LPC_PINCON->PINSEL0 |= 0x2<<18;	     // MOSI1
//	//
//	//    PINSEL0       name       00                01           10      11      reset
//	//	13:12         P0.6       GPIO Port 0.6     I2SRX_SDA    SSEL1   MAT2.0  00
//	//
//	// e.g 2: set & clear
//	//    #define PIN_MASK_CS (1<<2)
//	//	LPC_GPIO2->FIODIR 	|= 1<<2;	     // set bit #3, P2.2 as CSn, output
//	//	LPC_GPIO2->FIOCLR |= PIN_MASK_CS;	 // CS low
//	//    LPC_GPIO2->FIOSET |= PIN_MASK_CS;    // CS High
//	//
//	// e.g 3: here's a blinky one, using the above implementation
//
//	// test RST pin with an LED, method 1
//	//		uint32_t j;
//	//		LPC_GPIO0->FIODIR 	|= 1<<1;	// P0.1 - RST,output
//	//		while(1)
//	//		{
//	//			LPC_GPIO0->FIOCLR 	|= 1<<1;	// P0.1 - RST, LOW
//	//			for(j = 3000000; j > 0; j--);
//	//			LPC_GPIO0->FIOSET 	|= 1<<1;	// P0.1 - RST, HIGH
//	//			for(j = 3000000; j > 0; j--);
//	//		}
//
//	//  // test RST pin with an LED, method 2
//	//		uint32_t j;
//	//		//GPIOOUT(LPC_GPIO0,1);	// P0.1 - RST,output
//	//		GPIODIR(LPC_GPIO0,1,OUTPUT);
//	//		while(1)
//	//		{
//	//			GPIOCLR(LPC_GPIO0,1);	// P0.1 - RST, LOW
//	//			for(j = 500000; j > 0; j--);
//	//			digitalWrite(LPC_GPIO0,1,HIGH);	// P0.1 - RST, HIGH
//	//			for(j = 500000; j > 0; j--);
//	//		}
//
//
//
//
//
//	//SSP1Init();
//
//	//  LCDInit();
//	//  LCDClear();
//	//  LCDString("Hello Wolrd!");
//
//	//LcdInitialise();
//	//LcdClear();
//	//LcdString("Hello World!");
//
//
//
//	//	  	LPC_GPIO0->FIODIR 	|= 1<<1;	// P0.1 - RST,output
//	//	  	while(1)
//	//	  	{
//	//	  		LPC_GPIO0->FIOCLR 	|= 1<<1;	// P0.1 - RST, LOW
//	//	  		_delay_ms(1000);
//	//	  		LPC_GPIO0->FIOSET 	|= 1<<1;	// P0.1 - RST, HIGH
//	//	  		_delay_ms(1000);
//	//	  	}
//
//	//	/* SystemClockUpdate() updates the SystemFrequency variable */
//	SystemClockUpdate(); // (?)
//	//	// wiring and initializing
//	//	// PIN_SCE   6
//	//	// PIN_RESET 1
//	//	// PIN_DC    0
//	//	// PIN_SDIN  9
//	//	// PIN_SCLK  7
//	//	//lcdInit(7,9,0,6,1,50);
//	// pin declared on port 0
//	//LCDInit(15,18,23,16,24,50);
//	// init with port resolver
//	LCDInit(12,15,20,13,21,50);
//	//setContrast(55);
//	LCDcommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYALLON);
//	_delay_ms(500);
//	// back to normal
//	LCDcommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
//
//	LCDshowLogo();
//	_delay_ms(3000);
//	LCDclear();
//	// set up free running timer for our keypad's millis()
//	init_timer( 0, TIME_INTERVAL ); // clock ticks every 10ms, we can set smaller time interval but that would produce more interrupts
//	enable_timer( 0 );
//
//	//uint8_t rowPins[4] = {11, 5, 6, 8}; //connect to the row pinouts of the keypad
//	//uint8_t colPins[3] = {10, 12, 7}; //connect to the column pinouts of the keypad
//
//	uint8_t rowPins[4] = {63, 57, 58, 60}; //connect to the row pinouts of the keypad
//	uint8_t colPins[3] = {62, 64, 59}; //connect to the column pinouts of the keypad
//
//	KeyPadInit( makeKeymap(numberKeys), rowPins, colPins, 4, 3 );
//	// Add an event listener.
//	KeyPadAddEventListener(keypadEvent);
//
//	while(1)
//	{
//		delayMs(1,100); // slow polling down a little bit
//
//		//	  char key = KeyPadGetKey();
//		//	  if (key != NO_KEY)
//		//	  {
//		//		  LCDdrawchar(0,10,key);
//		//		  //LCDdrawchar(0,10,*a_key);
//		//		  LCDdisplay();
//		//	  }
//	}
//
//	/////  test millis()
//	//	unsigned int lastMillis = 0;
//	//	while(1)
//	//	{
//	//		//LCDshowLogo();
//	//		//_delay_ms(1000);
//	//		delayMs(1,1000); // use timer1, otherwise conflict with timer1 ( this delay reset the timer in every period)
//	//		LCDclear();
//	//		//LCDdrawstring(0,0, "MeshPhone: affordable mobile Mesh Networking for local communications, now 's ARMed!");
//	//		//if (timer0_counter > 10)
//	//		//	timer0_counter = 0;
//	//		char testString[16];
//	//        itoa((unsigned int)(millis()-lastMillis), testString,10);
//	//        lastMillis = millis();
//	//		//LCDdrawchar(0,timer0_counter,'m');
//	//        LCDdrawstring(0,0,testString);
//	//		LCDdisplay();
//	//		//_delay_ms(1000);
//	//		//delayMs(0,1000);
//	//	}
//
//	while ( 1 );
//	return 0;
//}
//
//
///******************************************************************************
// **                            End Of File
// ******************************************************************************/

