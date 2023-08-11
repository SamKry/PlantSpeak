#include <Arduino.h>
#include "actors/Led/Led.h"

Led::Led(int pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    turnOff();
}

void Led::turnOn()
{
    digitalWrite(_pin, LOW);
}

void Led::turnOff()
{
    digitalWrite(_pin, HIGH);
}

void Led::blinkOK()
{
    for (int i = 0; i < 4; i++)
    {
        turnOff();
        delay(100);
        turnOn();
        delay(100);
    }
}

void Led::blinkError()
{
    for (int i = 0; i < 3; i++)
    {
        turnOff();
        delay(500);
        turnOn();
        delay(500);
    }
}