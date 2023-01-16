//
// Created by sjbar on 15/01/2023.
//

#pragma once

#include <iostream>
#include "ImageBuffer.hpp"

namespace PathRehnda {
    class ImageWriter {
    public:
        void write_image_buffer_to_stream(std::ostream &out, const ImageBuffer& image_buffer, uint32_t num_samples);
    };

} // PathRehnda