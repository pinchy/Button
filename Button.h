#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_DEFAULT_DEBOUNCE 100
#define Sensor Button

#include <Arduino.h>

class Button
{
        typedef void (*CallBack_t)();

    private:
        int _pin = -1;
        bool _state;
        bool _previousState;
        bool _inversed;

        unsigned long _lastEdge;
        unsigned long _debounceTimeout = (unsigned long) BUTTON_DEFAULT_DEBOUNCE;
        CallBack_t _CallBackOnPress = nullptr;
        CallBack_t _CallBackOnRelease = nullptr;

        void _handleCallBacks(void);

    public:
        Button();
        Button(int p, bool i = false);
        ~Button();
        void begin(bool triggerCallbackOnFirstLoop = false);

        void setCallBack(CallBack_t cbp, CallBack_t cbr = nullptr);
        void setDebounceTimeout(unsigned long t) { this->_debounceTimeout = t; };

        void setPin(int p, bool i = false);
        int getPin() { return this->_pin; };
        bool isPinSet() { return this->_pin != -1; };
        bool pressed(void);
        bool read(void);
        void tick(void);
};

#endif