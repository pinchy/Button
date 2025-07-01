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
    this->_lastEdge = 0;
    this->_state = this->read();
    this->_longPressTriggered = false;
    this->_triggerCallBackOnFirstLoop = triggerCallbackOnFirstLoop;
}

void Button::_pressOnFirstLoop(void)
{
    if (this->_triggerCallBackOnFirstLoop)
    {
        this->_state = this->read();
        if (this->_state)
        {
            this->_triggerCallBack(CALLBACK_PRESS);
        }

        this->_triggerCallBackOnFirstLoop = false;
    }
}

bool Button::read(void) { return digitalRead(this->_pin) ^ this->_inversed; }

void Button::tick(void)
{
    if (this->isPinSet() == false) return;
    if (this->_triggerCallBackOnFirstLoop)
    {
        this->_pressOnFirstLoop();
        return;
    }

    bool isPressed = this->read();
    bool edge = (isPressed != this->_state);
    this->_state = isPressed;
    unsigned long currentTime = millis();
    bool longPress = (currentTime - this->_lastEdge > this->_longPressTimeout);
    bool shortPress = (currentTime - this->_lastEdge > this->_debounceTimeout);

    if (edge && shortPress)
    {
        if (isPressed == true)
        {
            // Serial.println(" ------- pressed");
            this->_triggerCallBack(CALLBACK_PRESS);
        }

        else
        {
            if (longPress)
            {
                // Serial.println(" ------- released (long)");
                this->_triggerCallBack(CALLBACK_LONGRELEASE);
            }
            else
            {
                // Serial.println(" ------- released (short)");
                this->_triggerCallBack(CALLBACK_RELEASE);
            }
        }

        this->_lastEdge = currentTime;
        this->_longPressTriggered = false;
    }

    else if (isPressed && longPress)
    {
        if (!this->_longPressTriggered)
        {
            // Serial.println(" ------- long pressed");
            this->_triggerCallBack(CALLBACK_LONGPRESS);
            this->_longPressTriggered = true;
        }
    }
}

void Button::_triggerCallBack(CallBackType_t type)
{
    if (this->_CallBacks[type] != nullptr) this->_CallBacks[type]();
}

bool Button::pressed(void)
{
    this->_state = this->read();
    return this->_state;
}
