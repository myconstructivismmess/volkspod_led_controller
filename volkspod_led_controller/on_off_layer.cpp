// +------------------------------------------------------------------------------+
// | filename: on_off_layer.cpp                                                   |
// | filepath: volkspod_led_controller/on_off_layer.cpp                           |
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

#include "on_off_layer.h"

OnOffLayer::OnOffLayer(
    const uint16_t startIndex,
    const uint16_t endIndex,
    const Color color
) :
    NeopixelLayer(
        startIndex,
        endIndex
    ),
    _color(color)
{}

void OnOffLayer::enable() {
    if (!_enabled) {
        _enabled = true;
        _sendUpdateMessageToManager();
    }
}
void OnOffLayer::disable() {
    if (_enabled) {
        _enabled = false;
        _sendUpdateMessageToManager();
    }
}

bool OnOffLayer::isEnabled() const {
    return _enabled;
}
Color OnOffLayer::getPixelColor(Color backgroundColor, uint16_t index) const {
    return _color;
}
bool OnOffLayer::hasNeedForBackgroundPixelColor(uint16_t index) const {
    return false;
}

// --------------------------------------------------------------------------------