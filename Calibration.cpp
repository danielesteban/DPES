/*
	Calibration.cpp - Nunchucks calibration class for DPES.
	Created by Daniel Esteban, February 23, 2013.
*/

#include "Calibration.h"

Calibration::Calibration(int refreshRate, LedMatrix * ledMatrix, Nunchucks * nunchucks) : Game(refreshRate, ledMatrix) {
	_nunchucks = nunchucks;
    restart();
}

bool Calibration::onFrame() {
	if(!Game::onFrame()) return 0;
	
    //debug
    byte x = 0,
        y = 0;

    nunchuck * n = _nunchucks->get(/*pin*/);    
    n->xZero != 0 && (x = (n->xRead - n->xZero) / 50);

    if(_x == x && _y == y) return 0;
    renderSquare(_x, _y, false);
    _x = x;
    _y = y;
    renderSquare(_x, _y);
}

void Calibration::restart() {
	Game::restart();
	_x = _y = 255;
}

void Calibration::onDown(/*byte pin, */byte button) {
	switch(button) {
		case 0: //Zbutton
            nunchuck * n = _nunchucks->get(/*pin*/);
            n->xZero = 0;
            //n->yZero = 0;
            //n->zZero = 0;
		break;
	}
}

void Calibration::onUp(/*byte pin, */byte button) {
    switch(button) {
        case 0: //Zbutton
            nunchuck * n = _nunchucks->get(/*pin*/);
            n->xZero = n->xRead;
            //n->yZero = n->yRead;
            //n->zZero = n->zRead;

            //Save into eeprom...
            byte eepromBit = 0;
            EEPROM.write(eepromBit, lowByte(n->xZero));
            EEPROM.write(eepromBit + 1, highByte(n->xZero));
            //EEPROM.write(eepromBit + 2, lowByte(n->yZero));
            //EEPROM.write(eepromBit + 3, highByte(n->yZero));
            //EEPROM.write(eepromBit + 4, lowByte(n->zZero));
            //EEPROM.write(eepromBit + 5, highByte(n->zZero));
        break;
    }
}

void Calibration::renderSquare(byte x, byte y, bool on) {
    _ledMatrix->setPixel(3 + x, 3 + y, on ? 255 : 0);
    _ledMatrix->setPixel(4 + x, 3 + y, on ? 255 : 0);
    _ledMatrix->setPixel(3 + x, 4 + y, on ? 255 : 0);
    _ledMatrix->setPixel(4 + x, 4 + y, on ? 255 : 0);
}
