// +------------------------------------------------------------------------------+
// | filename: back_led_on_off_layer.h                                            |
// | filepath: volkspod_led_controller/back_led_on_off_layer.h                    |
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

// ----- Arduino Framework --------------------------------------------------------

#include <Arduino.h>

// ----- Back LED On Off Layer Class ----------------------------------------------

#ifndef _BACK_LED_ON_OFF_LAYER_CLASS_H_
#define _BACK_LED_ON_OFF_LAYER_CLASS_H_

#include "neopixel_layer.h"
#include "color.h"

class BackLedOnOffLayer : public NeopixelLayer {
    public:
        BackLedOnOffLayer(
            const uint16_t pixelCount,
            const uint16_t ring2StartIndex,
            const uint16_t ring3StartIndex,
            const uint16_t ring4StartIndex,
            const unsigned long animationDurationMs,
            const Color color
        );

        void update(const unsigned long currentTimeMs);

        void enable();
        void disable();

        bool isEnabled() const override;
        Color getPixelColor(const Color backgroundColor, const uint16_t index) const override;
        bool hasNeedForBackgroundPixelColor(const uint16_t index) const override;

    private:
        const uint16_t _ring2StartIndex;
        const uint16_t _ring3StartIndex;
        const uint16_t _ring4StartIndex;
        const unsigned long _animationDurationMs;
        const Color _color;

        unsigned long _animationTimeMs = 0;

        uint16_t _animationPositionRing1 = 0;
        uint16_t _animationPositionRing2 = 0;
        uint16_t _animationPositionRing3 = 0;
        uint16_t _animationPositionRing4 = 0;
        
        unsigned long _lastUpdateTimeMs = 0; // Last update time point
        bool _enabled = false;
};

#endif

// --------------------------------------------------------------------------------