// +------------------------------------------------------------------------------+
// | filename: turn_signal_layer.h                                                |
// | filepath: volkspod_led_controller/turn_signal_layer.h                        |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Arduino Framework --------------------------------------------------------

#include <Arduino.h>

// ----- Front Turn Signals Layer Class -------------------------------------------

#ifndef _TURN_SIGNAL_LAYER_CLASS_H_
#define _TURN_SIGNAL_LAYER_CLASS_H_

#include "neopixel_layer.h"
#include "color.h"

class TurnSignalLayer : public NeopixelLayer {
    public:
        TurnSignalLayer(
            const uint16_t startIndex,
            const uint16_t endIndex,
            const uint16_t carretLength,
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
        const uint16_t _startIndex;
        const uint16_t _endIndex;
        const uint16_t _layerLength;
        const uint16_t _carretLength;
        const unsigned long _animationDurationMs;
        const Color _color;

        unsigned long _animationTimeMs = 0;
        uint16_t _animationPosition = 0;
        
        unsigned long _lastUpdateTimeMs = 0; // Last update time point
        bool _enabled = false;
};

#endif

// --------------------------------------------------------------------------------