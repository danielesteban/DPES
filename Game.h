/*
	Game.h - Generic game class for DPES.
	Created by Daniel Esteban, February 22, 2013.
*/

#ifndef Game_h
#define Game_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <LedMatrix.h>

class Game {
	public:
		Game(int refreshRate, LedMatrix * ledMatrix);
		void onFrameTick();
		virtual bool onFrame();
		virtual void restart();
		virtual void onJoyChange(/*byte pin, */byte axis, int read) {};
		virtual void onAccelChange(/*byte pin, */byte axis, int read) {};
		virtual void onDown(/*byte pin, */byte button) {};
		virtual void onUp(/*byte pin, */byte button) {};
	protected:
		LedMatrix * _ledMatrix;
		unsigned int _gameTime;
	private:
		byte _frameRateCount,
			_frameRateTick;

		int _refreshRate,
			_gameTimeCount,
			_gameTimeTick;
};
 
#endif
