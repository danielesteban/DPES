/*
	Game.cpp - Generic game class for DPES.
	Created by Daniel Esteban, February 22, 2013.
*/

#include "Game.h"

Game::Game(int refreshRate, LedMatrix * ledMatrix) {
	_refreshRate = refreshRate;
	_frameRateCount = refreshRate / 60;
	_ledMatrix = ledMatrix;
	_frameRateTick = _gameTimeTick = _gameTime = 0;
}

void Game::onFrameTick() {
	_frameRateTick++;
	_gameTimeTick++;
	if(_gameTimeTick >= _refreshRate) {
		_gameTimeTick = 0;
		_gameTime++;
	}
}

bool Game::onFrame() {
	if(_frameRateTick < _frameRateCount) return 0;
	_frameRateTick = 0;
	return 1;
}

void Game::restart() {
	for(byte x=0; x<LedMatrix::numColums * LedMatrix::numRows; x++) _ledMatrix->setPixel(x, 0);
	_gameTimeTick = _gameTime = 0;
}
