// +------------------------------------------------------------------------------+
// | filename: back_brake_layer.h                                                 |
// | filepath: volkspod_led_controller/back_brake_layer.h                         |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Arduino Framework --------------------------------------------------------

#include <Arduino.h>

// ----- Back Brake Layer Class ---------------------------------------------------

#ifndef _BACK_BRAKE_LAYER_CLASS_H_
#define _BACK_BRAKE_LAYER_CLASS_H_

#include "neopixel_layer.h"
#include "color.h"

class BackBrakeLayer : public NeopixelLayer {
    public:
        BackBrakeLayer(
            const uint16_t startIndex,
            const uint16_t endIndex,
            const unsigned long animationSetupDurationMs,
            const Color color
        );

        void update(const unsigned long currentTimeMs);

        void enable();
        void disable();

        bool isEnabled() const override;
        Color getPixelColor(const Color backgroundColor, const uint16_t index) const override;
        bool hasNeedForBackgroundPixelColor(const uint16_t index) const override;

    private:
        const unsigned long _animationSetupDurationMs;
        const Color _color;
        
        unsigned long _animationSetupTimeMs = 0;

        float _animationFinalColorRatio = 0.0;
        
        unsigned long _lastUpdateTimeMs = 0; // Last update time point
        bool _enabled = false;
};

#endif

// --------------------------------------------------------------------------------
