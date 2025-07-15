# Button Library

A comprehensive Arduino library for handling button inputs with debouncing, long press detection, and callback support.

## Features

- **Debouncing**: Prevents false triggers from mechanical switch noise
- **Long Press Detection**: Distinguishes between short and long button presses
- **Callback Support**: Register functions to execute on different button events
- **Inverted Logic**: Support for both active-high and active-low buttons
- **Edge Detection**: Reliable press and release detection
- **Configurable Timeouts**: Customizable debounce and long press durations

## Installation

1. Download the library files (`Button.h` and `Button.cpp`)
2. Place them in your Arduino libraries folder or project directory
3. Include the header in your sketch: `#include <Button.h>`

## Quick Start

```cpp
#include <Button.h>

Button myButton(2);  // Button connected to pin 2

void setup() {
  Serial.begin(9600);
  myButton.begin();
}

void loop() {
  myButton.tick();  // Call this in your main loop

  if (myButton.pressed()) {
    Serial.println("Button is pressed!");
  }
}
```

## Constructor Options

```cpp
Button();                    // Default constructor
Button(int pin);             // Pin number only
Button(int pin, bool inverted); // Pin number and logic inversion
```

## Configuration Methods

### Pin Setup

```cpp
void setPin(int pin, bool inverted = false);
```

- `pin`: Arduino pin number
- `inverted`: Set to `true` for active-low buttons (default: `false`)

### Timeout Configuration

```cpp
void setDebounceTimeout(unsigned long milliseconds);   // Default: 100ms
void setLongPressTimeout(unsigned long milliseconds);  // Default: 1000ms
```

### Initialization

```cpp
void begin(bool triggerCallbackOnFirstLoop = false);
```

- `triggerCallbackOnFirstLoop`: If `true`, triggers press callback if button is already pressed when `begin()` is called

## Callback System

Register functions to execute on button events:

```cpp
void setCallBack(CallBack_t callback, CallBackType_t type);
```

### Callback Types

- `CALLBACK_PRESS`: Triggered when button is pressed
- `CALLBACK_RELEASE`: Triggered on short button release
- `CALLBACK_LONGPRESS`: Triggered when button is held for long press duration
- `CALLBACK_LONGRELEASE`: Triggered when button is released after long press

### Example with Callbacks

```cpp
#include <Button.h>

Button myButton(2);

void onPress() {
  Serial.println("Button pressed!");
}

void onRelease() {
  Serial.println("Button released (short)");
}

void onLongPress() {
  Serial.println("Long press detected!");
}

void onLongRelease() {
  Serial.println("Button released (long)");
}

void setup() {
  Serial.begin(9600);

  // Register callbacks
  myButton.setCallBack(onPress, CALLBACK_PRESS);
  myButton.setCallBack(onRelease, CALLBACK_RELEASE);
  myButton.setCallBack(onLongPress, CALLBACK_LONGPRESS);
  myButton.setCallBack(onLongRelease, CALLBACK_LONGRELEASE);

  myButton.begin();
}

void loop() {
  myButton.tick();  // Essential: call this every loop
}
```

## Reading Button State

```cpp
bool pressed();  // Returns current button state
bool read();     // Raw pin reading (respects inversion)
```

## Utility Methods

```cpp
int getPin();        // Returns configured pin number
bool isPinSet();     // Returns true if pin has been configured
```

## Hardware Setup

### Standard Setup (Active-Low)

```
Arduino Pin -----> Button -----> GND
                     |
                 10kΩ Resistor
                     |
                    VCC
```

Or simply use the internal pull-up (recommended):

```
Arduino Pin -----> Button -----> GND
```

### Inverted Setup (Active-High)

```
VCC -----> Button -----> Arduino Pin
                            |
                        10kΩ Resistor
                            |
                           GND
```

## Complete Example

```cpp
#include <Button.h>

Button powerButton(2, false);  // Pin 2, active-low
Button menuButton(3, false);   // Pin 3, active-low

void powerPress() {
  Serial.println("Power button pressed");
}

void menuLongPress() {
  Serial.println("Menu button long pressed - entering config mode");
}

void setup() {
  Serial.begin(9600);

  // Configure power button
  powerButton.setCallBack(powerPress, CALLBACK_PRESS);
  powerButton.setDebounceTimeout(50);   // 50ms debounce
  powerButton.begin();

  // Configure menu button
  menuButton.setCallBack(menuLongPress, CALLBACK_LONGPRESS);
  menuButton.setLongPressTimeout(2000); // 2 second long press
  menuButton.begin();
}

void loop() {
  powerButton.tick();
  menuButton.tick();

  // Other code here...
}
```

## Important Notes

- **Always call `tick()`** in your main loop for proper operation
- The library uses `millis()` for timing, so it's non-blocking
- Internal pull-up resistors are automatically enabled when using `setPin()`
- Callbacks are executed immediately when conditions are met
- Long press detection only works when using the callback system or polling in the main loop

## Default Values

- Debounce timeout: 100ms
- Long press timeout: 1000ms
- Logic: Active-low (inverted = false)
- Pull-up: Enabled automatically

## Troubleshooting

- **Button not responding**: Check wiring and ensure `tick()` is called in loop
- **False triggers**: Increase debounce timeout with `setDebounceTimeout()`
- **Long press not working**: Verify long press timeout and ensure continuous polling
- **Inverted behavior**: Check the `inverted` parameter in constructor or `setPin()`
