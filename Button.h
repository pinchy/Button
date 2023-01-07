#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_DEFAULT_DEBOUNCE 100

#include <Arduino.h>

class Button
{
        typedef void (*CallBack_t)();

    private:
        uint8_t _pin;
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
        Button(uint8_t p, bool i = false);
        ~Button();
        void begin(void);

        void setCallBack(CallBack_t cbp, CallBack_t cbr = nullptr);
        void setDebounceTimeout(unsigned long t) { this->_debounceTimeout = t; };

        void setPin(uint8_t p, bool i = false);
        bool pressed(void);
        bool read(void);
        void tick(void);
};

#endif