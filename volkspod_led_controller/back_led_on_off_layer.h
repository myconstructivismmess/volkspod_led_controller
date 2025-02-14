// +------------------------------------------------------------------------------+
// | filename: back_led_on_off_layer.h                                            |
// | filepath: volkspod_led_controller/back_led_on_off_layer.h                    |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
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