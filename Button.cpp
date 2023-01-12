#include <Button.h>

Button::Button(void) {}
Button::~Button(void) {}
Button::Button(uint8_t p, bool i) { this->setPin(p, i); }

void Button::setPin(uint8_t p, bool i)
{
    this->_pin = p;
    this->_inversed = i;
    pinMode(p, INPUT_PULLUP);
}

void Button::setCallBack(CallBack_t cbp, CallBack_t cbr)
{
    this->_CallBackOnPress = cbp;
    this->_CallBackOnRelease = cbr;
}

void Button::begin(bool triggerCallbackOnFirstLoop)
{
    this->_lastEdge = millis();
    this->_state = this->read();

    if (triggerCallbackOnFirstLoop && this->pressed())   // only trigger the ACTIVE state
        this->_previousState = !this->_state;
    else
        this->_previousState = this->_state;
}

bool Button::read(void) { return digitalRead(this->_pin) ^ this->_inversed; }

void Button::tick(void)
{
    // debounce
    if (this->read() != this->_state)
    {
        this->_lastEdge = millis();   // if an edge is detected, reset the last edge detect
        this->_state = this->read();
    }

    if (millis() - this->_lastEdge > this->_debounceTimeout)   // button held down for at least 50ms
    {
        this->_state = this->read();
        this->_handleCallBacks();
    }
}

void Button::_handleCallBacks(void)
{
    if (this->_state != this->_previousState)   // button edge
    {
        this->_previousState = this->_state;

        if (this->pressed())
        {
            if (this->_CallBackOnPress != nullptr) this->_CallBackOnPress();
        }
        else
        {
            if (this->_CallBackOnRelease != nullptr) this->_CallBackOnRelease();
        }
    }
}

bool Button::pressed(void) { return this->_state; }
