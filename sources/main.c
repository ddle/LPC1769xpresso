/*
=================================================================================
 Name        : main.c
 Author      : Le Dang Dung
 Version     : 0.1
 Copyright   : ...
 Description :
     Test LCD, KeyPad, Radio driver on Target board LPCxpresso LPC1769.
//                      +-------------------+
//                 ANT 1|      RFM22B       |9  GND
//                     2|                   |10 SDN->GND
//                     3|                   |11 NIRQ
//                     4|            --     |12 NSEL
//                 VCC 5|           |  |    |13 SCK
//                     6|     ----  |  |    |14 SDI
//                     7|    | IC |  --     |15 SDO
//                     8|     ----          |16
//                      +-------------------+
 interrupt 0 pin D2-----------NIRQ  (interrupt request out)
          SS pin p0.6/j8----------NSEL  (chip select in)
         SCK pin p0.7/j7----------SCK   (SPI clock in)
        MOSI pin p0.9/j5----------SDI   (SPI Data in)
        MISO pin p0.8/j6----------SDO   (SPI data out)
        P0.9

=================================================================================
 */

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "LPC17xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "pin.h"
#include <ctype.h>
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

//#include "ssp.h"
#include "PCD8544.h"
#include "keypad.h"
#include "RF22.h"

#define ROWS  4 //four rows
#define COLS  3 //three columns

// Define the keymaps.  The blank spot (lower left) is the space character.
char alphaKeys[4][3] = {
		{ 'a','d','g' },
		{ 'j','m','p' },
		{ 's','v','y' },
		{ '*',' ','#' }
};

char numberKeys[4][3] = {
		{ '1','2','3' },
		{ '4','5','6' },
		{ '7','8','9' },
		{ '*','0','#' }
};

bool alpha = false;   // Start with the numeric keypad.

// Take care of some special events.
void keypadEvent(KeypadEvent key) {
	static char virtKey = NO_KEY;      // Stores the last virtual key press. (Alpha keys only)
	static char physKey = NO_KEY;      // Stores the last physical key press. (Alpha keys only)
	static char buildStr[100];
	static uint8_t buildCount;
	static uint8_t pressCount;

	switch (KeyPadGetState())
	{
	case PRESSED:
		if (isalpha(key))
		{              // This is a letter key so we're using the letter keymap.
			if (physKey != key)
			{        // New key so start with the first of 3 characters.
				pressCount = 0;
				virtKey = key;
				physKey = key;
			}
			else
			{                       // Pressed the same key again...
				virtKey++;               // so select the next character on that key.
				pressCount++;            // Tracks how many times we press the same key.
			}
			if (pressCount > 2)
			{        // Last character reached so cycle back to start.
				pressCount = 0;
				virtKey = key;
			}
			LCDdrawchar(0,40,virtKey);
			LCDdisplay();
		}
		if (isdigit(key) || key == ' ' || key == '.')
		{
			LCDdrawchar(0,40,key);
			LCDdisplay();
		}
		if (key == '#')
		{
			LCDdrawchar(0,40,'#'); // send...
			LCDdisplay();
		}
		if (key == '*') // del
		{
			if (buildCount > 0)
			{
				buildStr[buildCount-1] = '\0';
				buildCount--;
				LCDclear();
				LCDdrawstring(0,0,buildStr);
				LCDdisplay();
			}
		}
		break;

	case HOLD:
		if (key == '#')
		{                   // Toggle between keymaps.
			if (alpha == true)
			{            // We are currently using a keymap with letters
				KeyPadBegin(*numberKeys);   // and want to change to numbers.
				alpha = false;
			}
			else
			{                          // Or, we are currently using a keymap with numbers
				KeyPadBegin(*alphaKeys);    // and want to change to letters.
				alpha = true;
			}
			LCDdrawchar(0,40,'>'); // switch...
			LCDdisplay();
		}
		else
		{                             // Some key other than '#' was pressed.
			if(key != '*')
			{
				if (isalpha(key))
				{
					uint8_t row_index = KeyPadCurrentRow();
					uint8_t col_index = KeyPadCurrentCol();
					buildStr[buildCount] = numberKeys[row_index][col_index];
				}
				else
				{
					buildStr[buildCount] = key;
				}
				buildCount++;
				buildStr[buildCount] = '\0';
				LCDdrawstring(0,0,buildStr);
				LCDdisplay();
			}
		}
		break;

	case RELEASED:
		if (buildCount >= sizeof(buildStr))  buildCount = 0;    // Our string is full. Start fresh.
		break;

	}  // end switch-case
}  // end keypad events

int recvCount = 0;
void rx()
{
  //delay(1000);
  if (available())
  {
    uint8_t rssi = rssiRead();
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    //rf22.waitAvailable(); // block until recv
    //lcd.clear();


    if (recv(buf, &len))
    {
       recvCount++;
       char countString[8]; // one byte, max 255

       itoa(recvCount, countString, 10); // base 10
       LCDclear();
       LCDdrawstring(0,0,countString);
       LCDdrawstring(0,10,"data:");
       LCDdrawstring(0,20,(char*)buf);
       //lcd.clear();
       //lcd.print(countString);
       //lcd.print(" data:");
       //lcd.print((char*)buf);

       //Serial.println((char*)buf);

       char rssiString[3]; // one byte, max 255

       itoa(rssi, rssiString, 10); // base 10
       LCDdrawstring(0,30,rssiString);

    }
    else
    {
    	LCDdrawstring(0,0,"recv failed");
    }
    LCDdisplay();
  }
}

/******************************************************************************
 **   Main Function  main()
 ******************************************************************************/
int main (void)
{
	//	/* SystemClockUpdate() updates the SystemFrequency variable */
	SystemClockUpdate();

	//	// wiring and initializing
	//	// PIN_SCE   6
	//	// PIN_RESET 1
	//	// PIN_DC    0
	//	// PIN_SDIN  9
	//	// PIN_SCLK  7

	// init with port resolver
	LCDInit(12,15,20,13,21,50);
	//setContrast(55);
	LCDcommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYALLON);
	_delay_ms(500);
	// back to normal
	LCDcommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	LCDshowLogo();
	_delay_ms(3000);
	LCDclear();

	// set up free running timer for our keypad's millis()
	init_timer( 0, TIME_INTERVAL ); // clock ticks every 10ms, we can set smaller time interval but that would produce more interrupts
	enable_timer( 0 );

	uint8_t rowPins[4] = {63, 57, 58, 60}; //connect to the row pinouts of the keypad
	uint8_t colPins[3] = {62, 64, 59}; //connect to the column pinouts of the keypad

	KeyPadInit( makeKeymap(numberKeys), rowPins, colPins, 4, 3 );
	// Add an event listener.
	KeyPadAddEventListener(keypadEvent);

	// test RF22
	//init radio : chip select - D6/p0.6 ; nIRQ - D56/p2.4 ; 1 == SSP1
	if (RF22init(6,56,1) == false)
	{
		LCDdrawstring(0,0,"init failed");
	}
	else if (!setModemConfig(FSK_Rb9_6Fd45))
	{
		LCDdrawstring(0,0,"setConf failed");
	}
	else
	{
		LCDdrawstring(0,0,"radio ok");
	}

	LCDdisplay();

	_delay_ms(1000);

	//setTxPower(RF22_TXPOW_11DBM);

	while(1)
	{
		// test keypad:
		//char key = KeyPadGetKey();
		//	  if (key != NO_KEY)
		//	  {
		//		  LCDdrawchar(0,10,key);
		//		  //LCDdrawchar(0,10,*a_key);
		//		  LCDdisplay();
		//	  }

		_delay_ms(100);
		// test radio
		rx();   // receive and display on LCD
		//tx(); // send back message (ping)
	}

	while ( 1 );
	return 0;
}


/******************************************************************************
 **                            End Of File
 ******************************************************************************/

