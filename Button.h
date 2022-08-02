#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_DEFAULT_DEBOUNCE 100

#include <Arduino.h>

class Button
{
    typedef void (*CallBack_t)();

private:
    int _pin;
    bool _state;
    bool _previousState;
    bool _inversed;

    uint32_t _lastEdge;
    uint32_t _debounceTimeout = (uint32_t)BUTTON_DEFAULT_DEBOUNCE;
    CallBack_t _CallBackOnPress = nullptr;
    CallBack_t _CallBackOnRelease = nullptr;

    void _handleCallBacks(void);

public:
    Button();
    Button(int p, bool i = false);
    ~Button();
    void begin(void);

    void setCallBack(CallBack_t cbp, CallBack_t cbr = nullptr);

    void setPin(int p, bool i = false);
    bool pressed(void);
    bool read(void);
    void tick(void);
};

#endif