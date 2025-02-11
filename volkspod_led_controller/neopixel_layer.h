// +------------------------------------------------------------------------------+
// | filename: neopixel_layer.h                                                   |
// | filepath: volkspod_led_controller/neopixel_layer.h                           |
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

// ----- External Libraries -------------------------------------------------------

#include <Adafruit_NeoPixel.h>

// ----- Arduino Framework --------------------------------------------------------

#include <Arduino.h>

// ----- Neopixel Layer Class -----------------------------------------------------

#ifndef _NEOPIXEL_LAYER_CLASS_H_
#define _NEOPIXEL_LAYER_CLASS_H_

#include "color.h"
#include "neopixel_manager.h"

// Forward Declaration of NeopixelManager class
class NeopixelManager;

class NeopixelLayer {
    friend class NeopixelManager;

    public:
        NeopixelLayer(const uint16_t startIndex, const uint16_t endIndex);

        const uint16_t getStartIndex() const;
        const uint16_t getEndIndex() const;

        // Methods to setup in class that inherits from NeopixelLayer
        virtual bool isEnabled() const = 0;
        virtual Color getPixelColor(const Color backgroundColor, const uint16_t index) const = 0;
        virtual bool hasNeedForBackgroundPixelColor(const uint16_t index) const = 0;
        
    protected:
        void _sendUpdateMessageToManager() const;

    private:
        const uint16_t _startIndex;
        const uint16_t _endIndex;

        const NeopixelManager* _manager = nullptr;

        // Should only be accessed by NeopixelManager begin method
        void _setManager(const NeopixelManager* manager);
};

#endif

// --------------------------------------------------------------------------------
