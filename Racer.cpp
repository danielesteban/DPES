/*
	Racer.cpp - Racer game logic for DPES.
	Created by Daniel Esteban, February 22, 2013.
*/

#include "Racer.h"

#define maxRoadSpeed 2
#define minRoadSpeed 20
#define roadBrightness 5

Racer::Racer(int refreshRate, LedMatrix * ledMatrix) : Game(refreshRate, ledMatrix) {
	restart();
}

bool Racer::onFrame() {
	if(!Game::onFrame()) return 0;
	if(!_crashed) {
		_carX += _carXSpeed;
		_carX = constrain(_carX, _roadData[_carY][0], _roadData[_carY - 1][0] + _roadData[_carY][1] - 1);
		animateRoad();
	} else if(!_showingScore) { //Crash animation
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
	_nextRoadMinW = 1;
	_roadMinW = _roadMaxW = _roadW = LedMatrix::numColums - 2;
	_roadX = 1;
	_roadSpeed = map(0, -125, 125, maxRoadSpeed, minRoadSpeed);
	_carX = _roadW / 2;
	_carXSpeed = _crashed = _crashFade = _showingScore = _score = 0;
	for(byte x=0; x<LedMatrix::numColums; x++) {
		_roadData[x][0] = 0;
		_roadData[x][1] = LedMatrix::numColums;
	}
}

void Racer::onJoyChange(/*byte pin, */byte axis, int read) {
	switch(axis) {
		case 0: //JoyX
			_carXSpeed = (float) read / 1023;
		break;
		case 1: //JoyY
			_roadSpeed = map(constrain(read, -125, 125), -125, 125, maxRoadSpeed, minRoadSpeed);
		break;
	}
}

void Racer::onDown(/*byte pin, */byte button) {
	switch(button) {
		case 0: //Zbutton
			if(_crashed) {
				if(_showingScore) restart();
				else {
					_ledMatrix->clear();
					_ledMatrix->printNumber(0, 1, _score / 10);
					_ledMatrix->printNumber(5, 1, (int) _score % 10);
					_showingScore = 1;
				}
			}
	}
}

void Racer::animateRoad() {
	_roadFrameRateTick++;

	if(_roadFrameRateTick < _roadSpeed) return;
	_roadFrameRateTick = 0;

	if((_roadMinW > 2 || _roadMaxW > 3) && _gameTime >= _nextRoadMinW) {
		if(_roadMinW > 2) _roadMinW--;
		else _roadMaxW--;
		_nextRoadMinW += 10;
	}

	if(_roadSpeed == minRoadSpeed) return;

	_roadW += random(-1, 2);
	_roadW = constrain(_roadW, _roadMinW, _roadMaxW);

	_roadX += random(-1, 2);
	_roadX = constrain(_roadX, 1, LedMatrix::numColums - _roadW - 1);
	
	byte x;

	if(_carX < _roadData[_carY - 1][0] || _carX >= _roadData[_carY - 1][0] + _roadData[_carY - 1][1]) { //Collision
		_crashed = 1;
		_score -= _gameTime;
		_score < 0 && (_score = 0);
		for(x=0; x<LedMatrix::numColums; x++) {
			_ledMatrix->setPixel(x, _carY, x < _roadData[_carY][0] || x >= _roadData[_carY][0] + _roadData[_carY][1] ? roadBrightness : 0);
		}
		return;
	}

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
	
	_score += 0.05;
}