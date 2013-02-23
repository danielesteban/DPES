/*
	Calibration.h - Nunchucks calibration class for DPES.
	Created by Daniel Esteban, February 23, 2013.
*/

#ifndef Calibration_h
#define Calibration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <Nunchucks.h>
#include "Game.h"

class Calibration : public Game {
	public:
		Calibration(int refreshRate, LedMatrix * ledMatrix, Nunchucks * nunchucks);
		void onFrameTick();
		bool onFrame();
		void restart();
		void onDown(/*byte pin, */byte button);
		void onUp(/*byte pin, */byte button);
		void renderSquare(byte x, byte y, bool on = true);
	private:
		byte _x, _y;
		Nunchucks * _nunchucks;
};
 
#endif
