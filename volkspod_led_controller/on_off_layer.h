// +------------------------------------------------------------------------------+
// | filename: on_off_layer.h                                                     |
// | filepath: volkspod_led_controller/on_off_layer.h                             |
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

// ----- On Off Layer Class -------------------------------------------------------

#ifndef _ON_OFF_LAYER_CLASS_H_
#define _ON_OFF_LAYER_CLASS_H_

class OnOffLayer : public NeopixelLayer {
    public:
        OnOffLayer(
            uint16_t startIndex,
            uint16_t endIndex,
            Color color
        );

        void enable() {
            _enabled = true;
        }
        void disable() {
            _enabled = false;
        }
    protected:
        bool isEnabled() override;
        Color getPixelColor(Color backgroundColor, uint16_t index) override;
        bool hasNeedForBackgroundPixelColor(uint16_t index) override;
    private:
        const Color _color;
        bool _enabled = true;
};

#endif

// --------------------------------------------------------------------------------