#ifndef LED_H
#define LED_H

class Led
{
public:
    Led(int pin);
    void turnOn();
    void turnOff();
    void blinkOK();
    void blinkError();

private:
    int _pin;
};

#endif // LED_H
