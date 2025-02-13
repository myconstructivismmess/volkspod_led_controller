// +------------------------------------------------------------------------------+
// | filename: neopixel_manager.cpp                                               |
// | filepath: volkspod_led_controller/neopixel_manager.cpp                       |
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

// ----- Neopixel Manager Class ---------------------------------------------------

#include "neopixel_manager.h"

NeopixelManager::NeopixelManager(
    Adafruit_NeoPixel* neopixel,
    NeopixelLayer* const* layers,
    const size_t layersCount
) :
    _neopixel(neopixel),
    _layers(layers),
    _layersCount(layersCount)
{
    
}

void NeopixelManager::begin() const {
    // Initialize the Neopixel
    _neopixel->begin();

    // Initialize the link with the layers for messages purposes
    for (size_t layerIndex = 0; layerIndex < _layersCount; layerIndex++) {
        _layers[layerIndex]->_setManager(this);
    }

    // Initial Rendering of the Neopixel
    _renderAll();
}

void NeopixelManager::_onNeopixelLayerUpdated(const NeopixelLayer* layer) const {
    _renderFramed(layer->getStartIndex(), layer->getEndIndex());
}

void NeopixelManager::_renderAll() const {
    uint16_t pixelCount = _neopixel->numPixels();
    _renderFramed(0, pixelCount - 1);
}
void NeopixelManager::_renderFramed(const uint16_t startIndex, const uint16_t endIndex) const {
    if (_layersCount == 0) {
        for (uint16_t index = startIndex; index <= endIndex; index++) {
            _neopixel->setPixelColor(index, 0);
        }
        _neopixel->show();
        
        return;
    }

    // Iterating over all pixels in the provided frame to rerender them
    for (uint16_t index = startIndex; index <= endIndex; index++) {
        size_t layerIndex = _layersCount - 1;
        size_t lastLayerEncouteredToRender = _layersCount;
        Color color = { 0, 0, 0 };

        // Finding the first layer that doesn't need the pixel colors from preceding layers.
        while (true) {
            const NeopixelLayer* layer = _layers[layerIndex];

            if (!layer->isEnabled() || index < layer->getStartIndex() || layer->getEndIndex() < index) {
                // Pixel out of layer bounds or layer disabled, Skipping
                if (layerIndex > 0) {
                    layerIndex--;
                    continue;
                } else {
                    break;
                }
            }

            lastLayerEncouteredToRender = layerIndex;

            if (!layer->hasNeedForBackgroundPixelColor(index)) {
                break;
            }

            if (layerIndex > 0) {
                layerIndex--;
            } else {
                break;
            }
        }

        if (lastLayerEncouteredToRender != _layersCount) {
            layerIndex = lastLayerEncouteredToRender;

            while (layerIndex < _layersCount) {
                const NeopixelLayer* layer = _layers[layerIndex];

                if (!layer->isEnabled() || index < layer->getStartIndex() || layer->getEndIndex() < index) {
                    // Pixel out of layer bounds or layer disabled, Skipping
                    layerIndex++;
                    continue;
                }

                color = layer->getPixelColor(color, index);
                layerIndex++;
            }
        }

        _neopixel->setPixelColor(index, color.red, color.green, color.blue);
    }

    _neopixel->show();
}

// --------------------------------------------------------------------------------
