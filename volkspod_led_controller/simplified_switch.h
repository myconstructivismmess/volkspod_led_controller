// +------------------------------------------------------------------------------+
// | filename: simplified_switch.h                                                |
// | filepath: volkspod_led_controller/simplified_switch.h                        |
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



// ----- Switch Class --------------------------------------------------------

#ifndef _SIMPLIFIED_SWITCH_CLASS_H_
#define _SIMPLIFIED_SWITCH_CLASS_H_

#include <Arduino.h>

class SimplifiedSwitch {
    public:
        SimplifiedSwitch(
            uint8_t pin,
            unsigned long debounceDurationMs = 50
        );

        void update(unsigned long currentTimeMs);

        bool getState();

        bool getStateUpdated();
        void resetStateUpdated();

  private:
    const uint8_t _pin;
    const unsigned long _debounceDurationMs;

    unsigned long _updateAvailableTimeMs;
    bool _state;
    bool _stateUpdated;

    void _updateState(unsigned long currentTimeMs, bool dontUpdateStateUpdated = false);
};

#endif



// -----------------------------------------------------------------------------
