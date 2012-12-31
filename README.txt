Arduino to ARM LPC1769xpresso port. Ported routines:

void pinMode()
void digitalWrite()
uint8_t digitalRead()
void attachInterrupt()
void detachInterrupt()
void shiftOut()

Importing pin.h and pin.c to an ARM project to utilize Arduino's libraries that require the above routines. 
This is helpful for quick prototyping and reduce porting effort, when large amount of sensor and IO
libraries have been written for the Arduino. 

Example main.c tests this port on the following devices and libraries:
- RFM22B,  Arduino library from Mike McCauley
- PCD8544 LCD (Nokia3310), Arduino library from Limor Fried, Adafruit Industries
- Keypad, Arduino library from Mark Stanley, Alexander Brevig