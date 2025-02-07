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
    const Adafruit_NeoPixel* neopixel,
    const NeopixelLayer* layers[],
    const size_t layersCount
) :
    _neopixel(neopixel),
    _layers(layers),
    _layersCount(layersCount)
{
    for (size_t layerIndex = 0; layerIndex < layersCount; layerIndex++) {
        _layers[layerIndex]->setManager(&this);
    }
}

void NeopixelManager::begin() {
    _neopixel->begin();
           
    _neopixel->clear();
    _renderAll();
}

void NeopixelManager::_onNeopixelLayerUpdated(NeopixelLayer* layer) {
    _renderFramed(layer->getStartIndex() && layer->getEndIndex());
}

void NeopixelManager::_renderAll() {
    uint16_t pixelCount = _selectedNeopixelManager.numPixels();
    _render_framed(0, pixelCount - 1);
}
void NeopixelManager::_renderFramed(uint16_t startIndex, uint16_t endIndex) {
    // Iterating over all pixels in the provided frame to rerender them
    for (uint16_t index = startIndex; index <= endIndex; index++) {
        size_t layerIndex = _layersCount - 1;
        size_t lastLayerEncouteredToRender = _layersCount;
        Color color = { 0, 0, 0 };

        // Finding the first layer that doesn't need the pixel colors from preceding layers.
        while (layerIndex > 0) {
            NeopixelLayer* layer = _layers[layerIndex];

            if (!layer->isEnabled() || index < layer->getStartIndex() || layer->getEndIndex() < index) {
                // Pixel out of layer bounds or layer disabled, Skipping
                layerIndex--;
                continue;
            }

            lastLayerEncouteredToRender = layerIndex;

            if (!layer->hasNeedForBackgroundPixelColor(index)) {
                break;
            }

            layerIndex--;
        }

        if (lastLayerEncouteredToRender != _layersCount) {
            layerIndex = lastLayerEncouteredToRender;

            while (layerIndex < _layersCount) {
                NeopixelLayer* layer = _layers[layerIndex];

                if (!layer->isEnabled() || index < layer->getStartIndex() || layer->getEndIndex() < index) {
                    // Pixel out of layer bounds or layer disabled, Skipping
                    layerIndex++;
                    continue;
                }

                color = layer->getPixelColor(color, index);
            }
        }

        _neopixel->setPixelColor(index, color.red, color.green, color.blue);
    }

    _neopixel->show();
}

// --------------------------------------------------------------------------------
