//
// Created by sjbar on 15/01/2023.
//

#include "ImageWriter.hpp"
#include "Color.hpp"

namespace PathRehnda {
    void ImageWriter::write_image_buffer_to_stream(std::ostream &out, const ImageBuffer &image_buffer, int num_samples) {
        out << "P3\n" << image_buffer.image_width << ' ' << image_buffer.image_height << "\n255\n";
        for (uint32_t y = image_buffer.image_height - 1; y != (uint32_t) -1; y--) {
            for (uint32_t x = 0; x < image_buffer.image_width; x++) {
                auto color_24_bit = unit_interval_to_24_bit_rgb(image_buffer.get_color_sample(x, y, num_samples));
                out << static_cast<int>(color_24_bit.r) << ' ' << static_cast<int>(color_24_bit.g) << ' ' << static_cast<int>(color_24_bit.b) << '\n';
            }
        }
    }
} // PathRehnda