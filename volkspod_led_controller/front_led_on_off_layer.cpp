// +------------------------------------------------------------------------------+
// | filename: front_led_on_off_layer.cpp                                         |
// | filepath: volkspod_led_controller/front_led_on_off_layer.cpp                 |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Front On Off Layer Class -------------------------------------------------

#include "front_led_on_off_layer.h"
#include "delta_time.h"
#include "easings.h"

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
        const float easedTime = Easings::easeInOutCubic((float)_animationTimeMs / _animationDurationMs);
        const uint16_t newAnimationPosition = easedTime * (getEndIndex() + 1);
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