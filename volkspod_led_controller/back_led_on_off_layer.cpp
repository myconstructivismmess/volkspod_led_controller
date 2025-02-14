// +------------------------------------------------------------------------------+
// | filename: back_led_on_off_layer.cpp                                          |
// | filepath: volkspod_led_controller/back_led_on_off_layer.cpp                  |
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

// ----- Back On Off Layer Class --------------------------------------------------

#include "back_led_on_off_layer.h"
#include "delta_time.h"
#include "easings.h"

BackLedOnOffLayer::BackLedOnOffLayer(
    const uint16_t pixelCount,
    const uint16_t ring2StartIndex,
    const uint16_t ring3StartIndex,
    const uint16_t ring4StartIndex,
    const unsigned long animationDurationMs,
    const Color color
) :
    NeopixelLayer(
        0,
        pixelCount - 1
    ),
    _ring2StartIndex(ring2StartIndex),
    _ring3StartIndex(ring3StartIndex),
    _ring4StartIndex(ring4StartIndex),
    _animationDurationMs(animationDurationMs),
    _color(color)
{
    const unsigned long currentTimeMs = millis();
    _lastUpdateTimeMs = currentTimeMs;
}

void BackLedOnOffLayer::update(unsigned long currentTimeMs) {
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
        const float easedTime = Easings::easeInOutCubic((float)_animationTimeMs / _animationDurationMs);

        bool animationPositionUpdated = false;

        const uint16_t newAnimationPositionRing1 = easedTime * _ring2StartIndex;
        const uint16_t newAnimationPositionRing2 = easedTime * (_ring3StartIndex - _ring2StartIndex);
        const uint16_t newAnimationPositionRing3 = easedTime * (_ring4StartIndex - _ring3StartIndex);
        const uint16_t newAnimationPositionRing4 = easedTime * (getEndIndex() - _ring4StartIndex + 1);

        if (_animationPositionRing1 != newAnimationPositionRing1) {
            _animationPositionRing1 = newAnimationPositionRing1;
            animationPositionUpdated = true;
        }

        if (_animationPositionRing2 != newAnimationPositionRing2) {
            _animationPositionRing2 = newAnimationPositionRing2;
            animationPositionUpdated = true;
        }

        if (_animationPositionRing3 != newAnimationPositionRing3) {
            _animationPositionRing3 = newAnimationPositionRing3;
            animationPositionUpdated = true;
        }

        if (_animationPositionRing4 != newAnimationPositionRing4) {
            _animationPositionRing4 = newAnimationPositionRing4;
            animationPositionUpdated = true;
        }

        if (animationPositionUpdated) {
            _sendUpdateMessageToManager();
        }
    }
}

void BackLedOnOffLayer::enable() {
    _enabled = true;
}
void BackLedOnOffLayer::disable() {
    _enabled = false;
}

bool BackLedOnOffLayer::isEnabled() const {
    return _animationTimeMs != 0;
}
Color BackLedOnOffLayer::getPixelColor(const Color backgroundColor, const uint16_t index) const {
    if (index < _ring2StartIndex) {
        if (index < _animationPositionRing1) {
            return _color;
        }
    } else if (index < _ring3StartIndex) {
        const uint16_t localIndex = index - _ring2StartIndex;

        if (localIndex < _animationPositionRing2) {
            return _color;
        }
    } else if (index < _ring4StartIndex) {
        const uint16_t localIndex = index - _ring3StartIndex;
    
        if (localIndex < _animationPositionRing3) {
            return _color;
        }
    } else {
        const uint16_t localIndex = index - _ring4StartIndex;
    
        if (localIndex < _animationPositionRing4) {
            return _color;
        }
    }
    
    return Color {0, 0, 0};
}
bool BackLedOnOffLayer::hasNeedForBackgroundPixelColor(const uint16_t index) const {
    return false;
}

// --------------------------------------------------------------------------------