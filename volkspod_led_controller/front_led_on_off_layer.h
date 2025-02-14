// +------------------------------------------------------------------------------+
// | filename: front_led_on_off_layer.h                                           |
// | filepath: volkspod_led_controller/front_led_on_off_layer.h                   |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- Arduino Framework --------------------------------------------------------

#include <Arduino.h>

// ----- Front LED On Off Layer Class ---------------------------------------------

#ifndef _FRONT_LED_ON_OFF_LAYER_CLASS_H_
#define _FRONT_LED_ON_OFF_LAYER_CLASS_H_

#include "neopixel_layer.h"
#include "color.h"

class FrontLedOnOffLayer : public NeopixelLayer {
    public:
        FrontLedOnOffLayer(
            const uint16_t pixelCount,
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
        const unsigned long _animationDurationMs;
        const Color _color;

        unsigned long _animationTimeMs = 0;
        uint16_t _animationPosition = 0;
        unsigned long _lastUpdateTimeMs = 0; // Last update time point
        bool _enabled = false;
};

#endif

// --------------------------------------------------------------------------------