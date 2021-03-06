#include "Arduino.h"
#include "ShiftRegister74HC595.h"


// constructor
ShiftRegister74HC595::ShiftRegister74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin)
{
    // set attributes
    _numberOfShiftRegisters = numberOfShiftRegisters;
    
    _clockPin = clockPin;
    _serialDataPin = serialDataPin;
    _latchPin = latchPin;
    
    // define pins as outputs
    pinMode(clockPin, OUTPUT);
    pinMode(serialDataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    
    // set pins low
    digitalWrite(clockPin, LOW);
    digitalWrite(serialDataPin, LOW);
    digitalWrite(latchPin, HIGH);
    
    // allocates the specified number of bytes and initializes them to zero
    _digitalValues = (uint8_t *)malloc(numberOfShiftRegisters * sizeof(uint8_t));
    memset(_digitalValues, 0, numberOfShiftRegisters * sizeof(uint8_t));
    
    setAll(_digitalValues); // reset shift register
}


void ShiftRegister74HC595::setAll(uint8_t * digitalValues) {
    int byte;
    
    digitalWrite(_latchPin, LOW); 
    for (byte = _numberOfShiftRegisters - 1; byte >= 0; byte--) {
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, digitalValues[byte]);
    }
    digitalWrite(_latchPin, HIGH); 
    
    _digitalValues = digitalValues;    
}


uint8_t * ShiftRegister74HC595::getAll() {
    return _digitalValues; 
}


void ShiftRegister74HC595::set(int pin, uint8_t value) {
    setNoUpdate(pin, value);
    updateRegisters();
}

void ShiftRegister74HC595::updateRegisters()
{
	 setAll(_digitalValues);
}

void ShiftRegister74HC595::setNoUpdate(int pin, uint8_t value) {
    if (value == 1)
        _digitalValues[pin / 8] |= 1 << (pin % 8);
    else
        _digitalValues[pin / 8] &= ~(1 << (pin % 8));
}


uint8_t ShiftRegister74HC595::get(int pin) {
    return (_digitalValues[pin / 8] >> (pin % 8)) & 1;
}


void ShiftRegister74HC595::setAllHigh() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++)
        _digitalValues[i] = 255;
    setAll(_digitalValues); 
}


void ShiftRegister74HC595::setAllLow() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++)
        _digitalValues[i] = 0; 
    setAll(_digitalValues); 
}
