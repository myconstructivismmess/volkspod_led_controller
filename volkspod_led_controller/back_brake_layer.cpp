// +------------------------------------------------------------------------------+
// | filename: back_brake_layer.cpp                                               |
// | filepath: volkspod_led_controller/back_brake_layer.cpp                       |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Back Brake Layer Class ---------------------------------------------------

#include "back_brake_layer.h"
#include "delta_time.h"
#include "easings.h"

BackBrakeLayer::BackBrakeLayer(
    const uint16_t startIndex,
    const uint16_t endIndex,
    const unsigned long animationSetupDurationMs,
    const Color color
) :
    NeopixelLayer(
        startIndex,
        endIndex
    ),
    _animationSetupDurationMs(animationSetupDurationMs),
    _color(color)
{
    const unsigned long currentTimeMs = millis();
    _lastUpdateTimeMs = currentTimeMs;
}

void BackBrakeLayer::update(unsigned long currentTimeMs) {
    const unsigned long deltaTimeMs = DeltaTime::calculate(currentTimeMs, _lastUpdateTimeMs);
    _lastUpdateTimeMs = currentTimeMs;

    bool animationSetupTimeMsUpdated = false;
    if (_enabled) {
        if (_animationSetupTimeMs != _animationSetupDurationMs) {
            const unsigned long incrementedSetupAnimationTimeMs = _animationSetupTimeMs + deltaTimeMs;

            if (_animationSetupDurationMs < incrementedSetupAnimationTimeMs || incrementedSetupAnimationTimeMs < _animationSetupTimeMs) {
                _animationSetupTimeMs = _animationSetupDurationMs;
            } else {
                _animationSetupTimeMs = incrementedSetupAnimationTimeMs;
            }

            animationSetupTimeMsUpdated = true;
        }
    } else {
        if (_animationSetupTimeMs != 0) {
            if (_animationSetupTimeMs > deltaTimeMs) {
                _animationSetupTimeMs = _animationSetupTimeMs - deltaTimeMs;
            } else {
                _animationSetupTimeMs = 0;
            }

            animationSetupTimeMsUpdated = true;
        }
    }

    if (animationSetupTimeMsUpdated) {
        const float easedSetupTime = Easings::easeInOutCubic((float)_animationSetupTimeMs / _animationSetupDurationMs);

        _animationFinalColorRatio = easedSetupTime;
        _sendUpdateMessageToManager();
    }
}

void BackBrakeLayer::enable() {
    _enabled = true;
}
void BackBrakeLayer::disable() {
    _enabled = false;
}

bool BackBrakeLayer::isEnabled() const {
    return _animationFinalColorRatio != 0.0;
}
Color BackBrakeLayer::getPixelColor(const Color backgroundColor, const uint16_t index) const {
    const uint8_t red = backgroundColor.red + _animationFinalColorRatio * ((int)(_color.red) - backgroundColor.red);
    const uint8_t green = backgroundColor.green + _animationFinalColorRatio * ((int)(_color.green) - backgroundColor.green);
    const uint8_t blue = backgroundColor.blue + _animationFinalColorRatio * ((int)(_color.blue) - backgroundColor.blue);

    return Color{red, green, blue};
}
bool BackBrakeLayer::hasNeedForBackgroundPixelColor(const uint16_t index) const {
    return true;
}

// --------------------------------------------------------------------------------
