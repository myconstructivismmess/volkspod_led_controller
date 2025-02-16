// +------------------------------------------------------------------------------+
// | filename: turn_signal_layer.cpp                                              |
// | filepath: volkspod_led_controller/turn_signal_layer.cpp                      |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Turn Signal Layer Class --------------------------------------------------

#include "turn_signal_layer.h"
#include "delta_time.h"

TurnSignalLayer::TurnSignalLayer(
    const uint16_t startIndex,
    const uint16_t endIndex,
    const uint16_t carretLength,
    const unsigned long animationDurationMs,
    const Color color
) :
    NeopixelLayer(
        min(startIndex, endIndex),
        max(startIndex, endIndex)
    ),
    _startIndex(startIndex),
    _endIndex(endIndex),
    _layerLength(max(startIndex, endIndex) - min(startIndex, endIndex) + 1),
    _carretLength(carretLength),
    _animationDurationMs(animationDurationMs),
    _color(color)
{
    const unsigned long currentTimeMs = millis();
    _lastUpdateTimeMs = currentTimeMs;
}

void TurnSignalLayer::update(unsigned long currentTimeMs) {
    const unsigned long deltaTimeMs = DeltaTime::calculate(currentTimeMs, _lastUpdateTimeMs);
    _lastUpdateTimeMs = currentTimeMs;

    bool animationTimeMsUpdated = false;
    if (_enabled || _animationTimeMs != 0) {
        const unsigned long incrementedAnimationTimeMs = _animationTimeMs + deltaTimeMs;

        if (_animationDurationMs < incrementedAnimationTimeMs || incrementedAnimationTimeMs < _animationTimeMs) {
            _animationTimeMs = _enabled
                ? deltaTimeMs - (_animationDurationMs - _animationTimeMs)
                : 0;
        } else {
            _animationTimeMs = incrementedAnimationTimeMs;
        }

        animationTimeMsUpdated = true;
    }

    if (animationTimeMsUpdated) {
        const float time = (float)_animationTimeMs / _animationDurationMs;
        const uint16_t newAnimationPosition = time * (_layerLength + _carretLength);

        if (_animationPosition != newAnimationPosition) {
            _animationPosition = newAnimationPosition;
            _sendUpdateMessageToManager();
        }
    }
}

void TurnSignalLayer::enable() {
    _enabled = true;
}
void TurnSignalLayer::disable() {
    _enabled = false;
}

bool TurnSignalLayer::isEnabled() const {
    return _animationTimeMs != 0;
}
Color TurnSignalLayer::getPixelColor(const Color backgroundColor, const uint16_t index) const {
    const uint16_t localIndex = _startIndex < _endIndex
        ? index - _startIndex
        : _layerLength - 1 - (index - _endIndex);

    if ((max(_animationPosition, _carretLength) - _carretLength) <= localIndex && localIndex < _animationPosition) {
        return _color;
    }
    
    return backgroundColor;
}
bool TurnSignalLayer::hasNeedForBackgroundPixelColor(const uint16_t index) const {
    const uint16_t localIndex = _startIndex < _endIndex
        ? index - _startIndex
        : _layerLength - 1 - (index - _endIndex);

    return (localIndex < max(_animationPosition, _carretLength) - _carretLength) || (_animationPosition <= localIndex);
}

// --------------------------------------------------------------------------------