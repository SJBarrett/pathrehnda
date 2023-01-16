//
// Created by sjbar on 15/01/2023.
//

#pragma once

#include "Vec3.hpp"

namespace PathRehnda {

    class ImageBuffer {
    public:
        ImageBuffer(uint32_t width, uint32_t height);

        void write_color_sample(uint32_t x, uint32_t y, ColorRgb color);

        void add_buffer(const ImageBuffer& other_buffer);

        [[nodiscard]]
        ColorRgb get_color_sample(uint32_t x, uint32_t y, uint32_t num_samples) const;
    public:
        std::vector<ColorRgb> color_buffer;
        uint32_t image_width;
        uint32_t image_height;
    private:
        double perform_gamma_correction(double color_value) const;
    };

} // PathRehnda