//
// Created by sjbar on 16/01/2023.
//

#pragma once

#include "Camera.hpp"
#include "hittable/Hittable.hpp"
#include "ImageBuffer.hpp"
#include "Sampler.hpp"

namespace PathRehnda {
    struct Scene {
        const Camera& camera;
        const Hittable& world;
    };
    struct SamplingConfig {
        const Sampler& sampler;
        uint32_t samples_per_pixel;
    };

    class Aggregator {
    public:
        void sample_pixels(const SamplingConfig& sampling_config, const Scene& scene, ImageBuffer& out_image_buffer, bool print_progress = false) const;
    };
}
