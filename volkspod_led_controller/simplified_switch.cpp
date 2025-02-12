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
#include "delta_time.h"

SimplifiedSwitch::SimplifiedSwitch(
    const uint8_t pin,
    const unsigned long debounceDurationMs,
    const bool pullup
) :
    _pin(pin),
    _debounceDurationMs(debounceDurationMs),
    _pullup(pullup)
{
    if (_pullup) {
        pinMode(
            _pin,
            INPUT_PULLUP
        );
    } else {
        pinMode(
            _pin,
            INPUT
        );
    }

    const unsigned long currentTimeMs = millis();

    _lastUpdateTimeMs = currentTimeMs;

    _updateState();
}

void SimplifiedSwitch::update(const unsigned long currentTimeMs) {
    const unsigned long deltaTimeMs = DeltaTime.calculate(currentTimeMs, _lastUpdateTimeMs);

    // Stop here if no subtential time has passed to reduce executed instruction count
    if (deltaTimeMs == 0) {
        return;
    }
    
    _lastUpdateTimeMs = currentTimeMs;

    // Update time left before an update without (underflow resistant)
    if (_timeLeftBeforeUpdateMs > deltaTimeMs) {
        _timeLeftBeforeUpdateMs = _timeLeftBeforeUpdateMs - deltaTimeMs;
    } else {
        _timeLeftBeforeUpdateMs = 0;
    }

    if (_timeLeftBeforeUpdateMs == 0) {
        _updateState();
    }
}

const bool SimplifiedSwitch::getState() const {
    return _state;
}

const bool SimplifiedSwitch::getStateUpdated() const {
    return _stateUpdated;
}
void SimplifiedSwitch::resetStateUpdated() {
    _stateUpdated = false;
}

void SimplifiedSwitch::_updateState() {
    const bool newState = digitalRead(_pin) == (_pullup ? LOW : HIGH);

    if (newState != _state) {
        _timeLeftBeforeUpdateMs = _debounceDurationMs;
        _stateUpdated = !_stateUpdated;
    }

    _state = newState;
}

// --------------------------------------------------------------------------------
