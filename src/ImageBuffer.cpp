//
// Created by sjbar on 15/01/2023.
//

#include "ImageBuffer.hpp"

namespace PathRehnda {
    ImageBuffer::ImageBuffer(uint32_t width, uint32_t height) : image_width(width), image_height(height) {
        color_buffer.resize(width * height, ColorRgb::zero());
    }

    void ImageBuffer::write_color_sample(uint32_t x, uint32_t y, ColorRgb color) {
        color_buffer[x + image_width * y] += color;
    }

    ColorRgb ImageBuffer::get_color_sample(uint32_t x, uint32_t y, int num_samples) const {
        const auto color = color_buffer[x + image_width * y];
        if (color.near_zero()) {
            return ColorRgb::zero();
        }

        auto r = color.x();
        auto g = color.y();
        auto b = color.z();

        auto scale = 1.0 / num_samples;
        // perform
        r = perform_gamma_correction(scale * r);
        g = perform_gamma_correction(scale * g);
        b = perform_gamma_correction(scale * b);
        return {r, g, b};
    }

    double ImageBuffer::perform_gamma_correction(double color_value) const {
        return sqrt(color_value);
    }
} // PathRehnda