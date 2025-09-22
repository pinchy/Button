#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_DEFAULT_DEBOUNCE 100
#define BUTTON_DEFAULT_LONGPRESS 1000
#define Sensor Button

#include <Arduino.h>

enum CallBackType_t
{
    CALLBACK_PRESS = 0,
    CALLBACK_RELEASE = 1,
    CALLBACK_LONGPRESS = 2,
    CALLBACK_LONGRELEASE = 3,
    CALLBACK_NUM = 4
};


enum ButtonEdgeType_t
{
    BUTTON_EDGE_RISING,
    BUTTON_EDGE_FALLING
};


class Button
{
        typedef void (*CallBack_t)();

    private:
        int _pin = -1;
        bool _state = false;
        bool _longPressTriggered = false;
        bool _triggerCallBackOnFirstLoop = false;
        bool _inversed = false;

        unsigned long _lastEdge = 0;
        unsigned long _debounceTimeout = (unsigned long) BUTTON_DEFAULT_DEBOUNCE;
        unsigned long _longPressTimeout = (unsigned long) BUTTON_DEFAULT_LONGPRESS;


        CallBack_t _CallBacks[CALLBACK_NUM] = {nullptr};

        void _triggerCallBack(CallBackType_t type);
        void _pressOnFirstLoop(void);

    public:
        Button();
        Button(int p, bool i = false);
        ~Button();
        void begin(bool triggerCallbackOnFirstLoop = false);

        void setCallBack(CallBack_t cb, CallBackType_t type);
        void setDebounceTimeout(unsigned long t) { this->_debounceTimeout = t; };
        void setLongPressTimeout(unsigned long t) { this->_longPressTimeout = t; };
        void checkPressedAndTrigger(void);

        void setPin(int p, bool i = false);
        int getPin() { return this->_pin; };
        bool isPinSet() { return this->_pin != -1; };
        bool pressed(void);
        bool read(void);
        void tick(void);
};

#endif