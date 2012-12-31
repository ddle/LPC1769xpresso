/*
||
|| @file Keypad.h
|| @version 1.9
|| @author Mark Stanley, Alexander Brevig
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for using matrix
|| | keypads. It supports the use of multiple keypads with the
|| | same or different sets of keys.  It also supports user
|| | selectable pins and definable keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#include "LPC17xx.h"

#ifndef KEYPAD_H
#define KEYPAD_H


#define makeKeymap(x) ((char*)x)

typedef char KeypadEvent;

typedef enum {
	IDLE=0,
	PRESSED,
	RELEASED,
	HOLD
} KeypadState;

typedef struct {
    uint8_t rows : 4;
    uint8_t columns : 4;
} KeypadSize;

#define NO_KEY  '\0'
#define KEY_RELEASED NO_KEY

/*
Keypad();
Keypad(uint8_t row[], uint8_t col[], uint8_t rows, uint8_t cols);
*/
void KeyPadInit(char *userKeymap, uint8_t *row, uint8_t *col, uint8_t rows, uint8_t cols);

//void begin(); //DEPRECATED!
void KeyPadBegin(char *userKeymap);
char KeyPadGetKey();
KeypadState KeyPadGetState();
void KeyPadSetDebounceTime(unsigned int debounce);
void KeyPadSetHoldTime(unsigned int hold);
void KeyPadAddEventListener(void (*listener)(char));
uint8_t KeyPadCurrentRow();
uint8_t KeyPadCurrentCol();
//void KeyPadDynamic(uint8_t *row, uint8_t *col);


#endif

/*
|| @changelog
|| | 1.9 2011-11-09 - Le Dang Dung : ported to LPC1769
|| | 1.8 2009-07-08 - Alexander Brevig : No longer uses arrays
|| | 1.7 2009-06-18 - Alexander Brevig : This library is a Finite State Machine
|| | 										every time a state changes the keypadEventListener will trigger, if set
|| | 1.7 2009-06-18 - Alexander Brevig : Added setDebounceTime
|| |										setHoldTime specifies the amount of microseconds before a HOLD state triggers
|| | 1.7 2009-06-18 - Alexander Brevig : Added transitionTo
|| | 1.6 2009-06-15 - Alexander Brevig : Added getState() and state variable
|| | 1.5 2009-05-19 - Alexander Brevig : Added setHoldTime()
|| | 1.4 2009-05-15 - Alexander Brevig : Added addEventListener
|| | 1.3 2009-05-12 - Alexander Brevig : Added lastUdate, in order to do simple debouncing
|| | 1.2 2009-05-09 - Alexander Brevig : Changed getKey()
|| | 1.1 2009-04-28 - Alexander Brevig : Modified API, and made variables private
|| | 1.0 2007-XX-XX - Mark Stanley : Initial Release
|| #
*/
