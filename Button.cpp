#include <Button.h>

Button::Button(void) {}
Button::~Button(void) {}
Button::Button(int p, bool i) { this->setPin(p, i); }

void Button::setPin(int p, bool i)
{
    this->_pin = p;
    this->_inversed = i;
    pinMode(p, INPUT_PULLUP);
}

void Button::setCallBack(CallBack_t cb, CallBackType_t type)
{
    if (type >= 0 && type < CALLBACK_NUM)
    {
        this->_CallBacks[type] = cb;
    }
}

void Button::begin(bool triggerCallbackOnFirstLoop)
{
    this->_lastEdge = millis();
    this->_state = this->read();
    this->_previousLongPressState = this->_state; // prevent the long press from triggering

    if (triggerCallbackOnFirstLoop && this->pressed()) // only trigger the ACTIVE state
        this->_previousShortPressState = !this->_state;
    else
        this->_previousShortPressState = this->_state;
}

bool Button::read(void) { return digitalRead(this->_pin) ^ this->_inversed; }

void Button::tick(void)
{
    if (this->isPinSet() == false)
        return;

    bool isPressed = this->read();
    bool edge = (isPressed != this->_state);

    if (edge)
    {
        this->_state = isPressed;
        if (isPressed == false)
        {
            if (millis() - this->_lastEdge > this->_longPressTimeout)
            {
                this->_triggerCallBack(CALLBACK_LONGRELEASE);
                this->_previousShortPressState = this->_state;
                this->_previousLongPressState = this->_state;
            }
            else if (millis() - this->_lastEdge > this->_debounceTimeout)
            {
                this->_previousShortPressState = this->_state;
                this->_triggerCallBack(CALLBACK_RELEASE);
            }
        }

        this->_lastEdge = millis();
    }

    if (isPressed)
    {
        if (millis() - this->_lastEdge > this->_longPressTimeout)
        {
            if (this->_state != this->_previousLongPressState)
            {
                this->_previousLongPressState = this->_state;
                this->_triggerCallBack(CALLBACK_LONGPRESS);
            }
        }
        else if (millis() - this->_lastEdge > this->_debounceTimeout)
        {
            if (this->_state != this->_previousShortPressState)
            {
                this->_previousShortPressState = this->_state;
                this->_triggerCallBack(CALLBACK_PRESS);
            }
        }
    }
}

void Button::_triggerCallBack(CallBackType_t type)
{
    if (this->_CallBacks[type] != nullptr)
        this->_CallBacks[type]();
}

bool Button::pressed(void) { return this->_state; }
