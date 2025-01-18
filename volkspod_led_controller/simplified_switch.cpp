// +------------------------------------------------------------------------------+
// | filename: simplified_switch.cpp                                              |
// | filepath: volkspod_led_controller/simplified_switch.cpp                      |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: MIT                                                                 |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// |                                                                              |
// | Permission is hereby granted, free of charge, to any person obtaining a copy |
// | of this software and associated documentation files (the "Software"), to     |
// | deal in the Software without restriction, including without limitation the   |
// | rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  |
// | sell copies of the Software, and to permit persons to whom the Software is   |
// | furnished to do so, subject to the following conditions:                     |
// |                                                                              |
// | The above copyright notice and this permission notice shall be included in   |
// | all copies or substantial portions of the Software.                          |
// |                                                                              |
// | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   |
// | IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     |
// | FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  |
// | AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       |
// | LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      |
// | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS |
// | IN THE SOFTWARE.                                                             |
// +------------------------------------------------------------------------------+



// ----- Simplified Switch Class --------------------------------------------------

#include "simplified_switch.h"

SimplifiedSwitch::SimplifiedSwitch(uint8_t pin, unsigned long debounceDurationMs)
    : _pin(pin),
      _debounceDurationMs(debounceDurationMs)
{
    pinMode(
        _pin,
        INPUT
    );

    unsigned long currentTimeMs = millis();
    _updateState(currentTimeMs, true);
}

void SimplifiedSwitch::update(unsigned long currentTimeMs) {
    if (_updateAvailableTimeMs <= currentTimeMs) {
        _updateState(currentTimeMs);
    }
}

bool SimplifiedSwitch::getState() {
    return _state;
}

bool SimplifiedSwitch::getStateUpdated() {
    return _stateUpdated;
}
void SimplifiedSwitch::resetStateUpdated() {
    _stateUpdated = false;
}

void SimplifiedSwitch::_updateState(unsigned long currentTimeMs, bool dontUpdateStateUpdated) {
    _updateAvailableTimeMs = currentTimeMs + _debounceDurationMs;

    bool newState = digitalRead(_pin) == HIGH;

    if (newState != _state && !dontUpdateStateUpdated) {
        _stateUpdated = !_stateUpdated;
    }

    _state = newState;
}



// --------------------------------------------------------------------------------
