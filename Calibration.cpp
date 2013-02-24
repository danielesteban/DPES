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
	
    byte x = 0,
        y = 0;

    nunchuck * n = _nunchucks->get(/*pin*/);    
    n->xZero != 0 && (x = (n->xRead - n->xZero) / 50);

    for(byte j=0; j<2; j++) {
        for(byte c=0; c<4; c++) {
            //This is super ugly.. but it's the only way to make it fit into the attiny memory
            _ledMatrix->setPixel(3 + _x + (c > 1 ? c - 2 : c), 3 + _y + (c > 1 ? 1 : 0), j == 1 ? 255 : 0);
        }
        _x = x;
        _y = y;
    }
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
            byte eepromByte = 0;
            EEPROM.write(eepromByte, lowByte(n->xZero));
            EEPROM.write(eepromByte + 1, highByte(n->xZero));
            //EEPROM.write(eepromBit + 2, lowByte(n->yZero));
            //EEPROM.write(eepromBit + 3, highByte(n->yZero));
            //EEPROM.write(eepromBit + 4, lowByte(n->zZero));
            //EEPROM.write(eepromBit + 5, highByte(n->zZero));
        break;
    }
}
