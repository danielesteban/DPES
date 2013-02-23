/*
    Setup.h - Pin Declarations, library variables & setup function for DPES.
    Created by Daniel Esteban, February 22, 2013.
*/

#ifndef Setup_h
#define Setup_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
//#include <SoftwareSerial.h>
#include <LedMatrix.h>
#include <Nunchucks.h>
#include <TinyWireM.h>

//Pin Declarations
#define Register DDRB
#define Port PORTB
#define LatchPin 4
#define ClockPin 1
#define DataPin 3
#define SerialRxPin 4
#define SerialTxPin 3

//#define SerialSpeed 57600
//#define SerialSpeed 19200

const int refreshRate = 3000; // for the LedMatrix multiplex (in Hz).

LedMatrix ledMatrix(&Register, &Port, LatchPin, ClockPin, DataPin);

//Games
#include "Racer.h"

Game * game = new Racer(refreshRate, &ledMatrix); //Default Game

//STOP EDITING HERE...

//Library variables and setup function

void onJoyChange(/*byte pin, */byte axis, int read);
void onAccelChange(/*byte pin, */byte axis, int read);
void onDown(/*byte pin, */byte button);
void onUp(/*byte pin, */byte button);
Nunchucks nunchucks(onJoyChange, onAccelChange, onDown, onUp);

//SoftwareSerial Serial(SerialRxPin, SerialTxPin);

void setup() {
	Wire.begin();
	nunchucks.setup();

	randomSeed(analogRead(0)); //this should be an unused pin.. but there are none left ;P

	//LedMatrix multiplex timer
	cli(); //stop interrupts

	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0  = 0;
  	TCCR0A |= (1 << WGM01); //turn on CTC mode
	TCCR0B |= (1 << CS11) | (1 << CS10); //Set CS11 & CS10 bits for 64 prescaler
	OCR0A = (F_CPU / (refreshRate * 64)) - 1; //set compare match register to refreshRate
	TIMSK |= (1 << OCIE0A); //enable timer compare interrupt

	sei(); //allow interrupts

	//pinMode(SerialRxPin, INPUT);
	//pinMode(SerialTxPin, OUTPUT);
	//Serial.begin(SerialSpeed);
	//Serial.write(255);
}

//LedMatrix multiplex timer interrupt
ISR(TIM0_COMPA_vect) {
	game->onFrameTick();
	ledMatrix.update();
}

#endif
