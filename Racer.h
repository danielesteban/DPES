/*
	Racer.h - Racer game logic for DPES.
	Created by Daniel Esteban, February 22, 2013.
*/

#ifndef Racer_h
#define Racer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Game.h"

class Racer : public Game {
	public:
		Racer(int refreshRate, LedMatrix * ledMatrix);
		bool onFrame();
		void restart();
		void onJoyChange(/*byte pin, */byte axis, int read);
		void onAccelChange(/*byte pin, */byte axis, int read);
		void onDown(/*byte pin, */byte button);
		void onUp(/*byte pin, */byte button);
		void animateRoad();
	private:
		static const byte _carY = LedMatrix::numRows - 2;

		byte _roadFrameRateTick,
			_roadX,
			_roadMinW,
			_roadMaxW,
			_roadW,
			_roadSpeed,
			_roadData[LedMatrix::numRows][2];

		bool _crashed,
			_showingScore,
			_crashFade,
			_accelOn;

		float _carX,
			_carXSpeed,
			_score;

		unsigned int _nextRoadMinW;
};
 
#endif
