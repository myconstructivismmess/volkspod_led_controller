// +------------------------------------------------------------------------------+
// | filename: neopixel_layer.cpp                                                 |
// | filepath: volkspod_led_controller/neopixel_layer.cpp                         |
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

// ----- Neopixel Layer Class -----------------------------------------------------

#include "neopixel_layer.h"

NeopixelLayer::NeopixelLayer(
    const uint16_t startIndex,
    const uint16_t endIndex
) :
    _startIndex(startIndex),
    _endIndex(endIndex)
{}

const uint16_t NeopixelLayer::getStartIndex() const {
    return _startIndex;
}
const uint16_t NeopixelLayer::getEndIndex() const {
    return _endIndex;
}

void NeopixelLayer::_sendUpdateMessageToManager() const {
    if (_manager != nullptr) {
        _manager->_onNeopixelLayerUpdated(this);
    }
}

void NeopixelLayer::_setManager(const NeopixelManager* manager) {
    _manager = manager;
}

// --------------------------------------------------------------------------------
