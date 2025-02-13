// +------------------------------------------------------------------------------+
// | filename: front_led_on_off_layer.cpp                                         |
// | filepath: volkspod_led_controller/front_led_on_off_layer.cpp                 |
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

// ----- Front On Off Layer Class -------------------------------------------------

#include "front_led_on_off_layer.h"
#include "delta_time.h"

FrontLedOnOffLayer::FrontLedOnOffLayer(
    const uint16_t pixelCount,
    const unsigned long animationDurationMs,
    const Color color
) :
    NeopixelLayer(
        0,
        pixelCount - 1
    ),
    _animationDurationMs(animationDurationMs),
    _color(color)
{
    const unsigned long currentTimeMs = millis();
    _lastUpdateTimeMs = currentTimeMs;
}

void FrontLedOnOffLayer::update(unsigned long currentTimeMs) {
    const unsigned long deltaTimeMs = DeltaTime::calculate(currentTimeMs, _lastUpdateTimeMs);
    _lastUpdateTimeMs = currentTimeMs;

    bool animationTimeMsUpdated = false;
    if (_enabled) {
        if (_animationTimeMs != _animationDurationMs) {
            const unsigned long incrementedAnimationTimeMs = _animationTimeMs + deltaTimeMs;

            if (incrementedAnimationTimeMs < _animationTimeMs || incrementedAnimationTimeMs > _animationDurationMs) {
                _animationTimeMs = _animationDurationMs;
            } else {
                _animationTimeMs = incrementedAnimationTimeMs;
            }

            animationTimeMsUpdated = true;
        }
    } else {
        if (_animationTimeMs != 0) {
            if (_animationTimeMs > deltaTimeMs) {
                _animationTimeMs = _animationTimeMs - deltaTimeMs;
            } else {
                _animationTimeMs = 0;
            }

            animationTimeMsUpdated = true;
        }
    }

    if (animationTimeMsUpdated) {
        const uint16_t newAnimationPosition = _animationTimeMs * (getEndIndex() + 1) / _animationDurationMs;
        if (_animationPosition != newAnimationPosition) {
            _animationPosition = newAnimationPosition;
            _sendUpdateMessageToManager();
        }
    }
}

void FrontLedOnOffLayer::enable() {
    _enabled = true;
}
void FrontLedOnOffLayer::disable() {
    _enabled = false;
}

bool FrontLedOnOffLayer::isEnabled() const {
    return _animationTimeMs != 0;
}
Color FrontLedOnOffLayer::getPixelColor(const Color backgroundColor, const uint16_t index) const {
    if (index < _animationPosition) {
        return _color;
    }
    
    return Color {0, 0, 0};
}
bool FrontLedOnOffLayer::hasNeedForBackgroundPixelColor(const uint16_t index) const {
    return false;
}

// --------------------------------------------------------------------------------