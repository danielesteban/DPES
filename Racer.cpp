/*
	Racer.cpp - Racer game logic for DPES.
	Created by Daniel Esteban, February 22, 2013.
*/

#include "Racer.h"

#define maxRoadSpeed 3
#define minRoadSpeed 27
#define roadBrightness 5

Racer::Racer(int refreshRate, LedMatrix * ledMatrix) : Game(refreshRate, ledMatrix) {
	restart();
}

bool Racer::onFrame() {
	if(!Game::onFrame()) return 0;
	if(!_crashed) animateRoad();
	else {
		byte brightness = _ledMatrix->getPixel(_carX, _carY);
		if(_crashFade == 0) {
			if(brightness > 0) brightness -= 15;
			else _crashFade = 1;
		} else {
			if(brightness < 255) brightness += 15;
			else _crashFade = 0;
		}
		_ledMatrix->setPixel(_carX, _carY, brightness);
	}
}

void Racer::restart() {
	Game::restart();
	_nextRoadMinW = 2;
	_roadMinW = _roadMaxW = _roadW = LedMatrix::numColums - 2;
	_roadX = 1;
	_roadSpeed = map(0, -125, 125, maxRoadSpeed, minRoadSpeed);
	_carX = _roadW / 2;
	_carXSpeed = _crashed = _crashFade = 0;
	for(byte x=0; x<LedMatrix::numColums; x++) {
		_roadData[x][0] = 0;
		_roadData[x][1] = LedMatrix::numColums;
	}
}

void Racer::onJoyChange(/*byte pin, */byte axis, int read) {
	switch(axis) {
		case 0: //JoyX
			_carXSpeed = (float) read / 510;
		break;
		case 1: //JoyY
			_roadSpeed = map(constrain(read, -125, 125), -125, 125, maxRoadSpeed, minRoadSpeed);
		break;
	}
}

void Racer::animateRoad() {
	_roadFrameRateTick++;

	_carX += _carXSpeed;
	_carX = constrain(_carX, _roadData[_carY][0], _roadData[_carY - 1][0] + _roadData[_carY][1] - 1);

	if(_roadFrameRateTick < _roadSpeed) return;
	_roadFrameRateTick = 0;

	if(_roadSpeed == minRoadSpeed) return;
	if((_roadMinW > 2 || _roadMaxW > 3) && _gameTime >= _nextRoadMinW) {
		if(_roadMinW > 2) _roadMinW--;
		else _roadMaxW--;
		_nextRoadMinW += 10;
	}

	_roadW += random(-1, 2);
	_roadW = constrain(_roadW, _roadMinW, _roadMaxW);

	_roadX += random(-1, 2);
	_roadX = constrain(_roadX, 1, LedMatrix::numColums - _roadW - 1);
	
	if(_carX < _roadData[_carY - 1][0] || _carX >= _roadData[_carY - 1][0] + _roadData[_carY - 1][1]) { //Collision
		_crashed = 1;
		return;
	}

	byte x;
	for(byte y=LedMatrix::numRows - 1; y>= 1; y--) {
		_roadData[y][0] = _roadData[y - 1][0];
		_roadData[y][1] = _roadData[y - 1][1];
		for(x=0; x<LedMatrix::numColums; x++) {
			_ledMatrix->setPixel(x, y, x < _roadData[y][0] || x >= _roadData[y][0] + _roadData[y][1] ? roadBrightness : 0);
		}
		if(y == _carY) _ledMatrix->setPixel(_carX, _carY, 255);
	}		
	for(x=0; x<LedMatrix::numColums; x++) {
		_ledMatrix->setPixel(x, x < _roadX || x >= _roadX + _roadW ? roadBrightness : 0);
	}
	_roadData[0][0] = _roadX;
	_roadData[0][1] = _roadW;
}