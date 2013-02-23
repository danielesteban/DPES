/*
	Dani's Portable Entertainment System
	====================================
	Created by Daniel Esteban, February 22, 2013.

	All is configured to run in a 8x8 LED Matrix with two 8-bit Shift Registers on an ATtiny85.
	As input, it uses the TiniWireM library to read data from a nunchuck.
	Edit Setup.h for Port/Pin Numbers & other configurations.
	(The number of row/columns of the matrix is hardcoded into libraries/LedMatrix/LedMatrix.h)
	(The code supports multiple nunchucks, but its commented out to save up memory.. probably i'll make it with defines in a near future)

	- For now I only have coded a racing game.

	Enjoy!
*/

#include "Setup.h"

void loop(void) {
	nunchucks.read();
	game->onFrame();
}

//Input Events
void onJoyChange(/*byte pin, */byte axis, int read) {
	game->onJoyChange(/*pin, */axis, read);
}

void onAccelChange(/*byte pin, */byte axis, int read) {
	game->onAccelChange(/*pin, */axis, read);
}

void onDown(/*byte pin, */byte button) {
	game->onDown(/*pin, */button);
}

void onUp(/*byte pin, */byte button) {
	switch(button) {
		case 0: //Zbutton
			if(nunchucks.get()->bCStatus) {
				game->restart();
				return;
			}		
	}
	game->onUp(/*pin, */button);
}

