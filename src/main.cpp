#include <iostream>
#include <spdlog/spdlog.h>
#include <fstream>
#include <thread>
#include <boost/program_options.hpp>

#include "Vec3.hpp"
#include "RehndaMath.hpp"
#include "materials/Material.hpp"
#include "hittable/Sphere.hpp"
#include "hittable/HittableList.hpp"
#include "Camera.hpp"
#include "materials/LambertianMaterial.hpp"
#include "materials/MetalMaterial.hpp"
#include "materials/DielectricMaterial.hpp"
#include "ImageBuffer.hpp"
#include "ImageWriter.hpp"
#include "Sampler.hpp"
#include "Aggregator.hpp"

using namespace PathRehnda;
namespace po = boost::program_options;

HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<LambertianMaterial>(ColorRgb(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -3; a < 3; a++) {
        for (int b = -3; b < 3; b++) {
            auto choose_mat = random_double();
            Point3 centre(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((centre - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = ColorRgb::random() * ColorRgb::random();
                    sphere_material = std::make_shared<LambertianMaterial>(albedo);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = ColorRgb::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<MetalMaterial>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<DielectricMaterial>(1.5);
                    world.add(std::make_shared<Sphere>(centre, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<DielectricMaterial>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<LambertianMaterial>(ColorRgb(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<MetalMaterial>(ColorRgb(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

struct PathRehndaOptions {
    uint32_t num_threads = 4;
    uint32_t image_width = 360;
    uint32_t num_samples_per_thread = 2;
    std::string output_file_name;
};

PathRehndaOptions parse_path_rehnda_options(int argc, char** argv) {
    po::options_description desc{"Options"};
    desc.add_options()
            ("help,h", "Help info")
            ("num_threads,n", po::value<uint32_t>()->default_value(std::jthread::hardware_concurrency()), "Number of threads")
            ("samples_per_thread,s", po::value<uint32_t>()->default_value(2), "Number of samplers per thread")
            ("image_width,w", po::value<uint32_t>()->default_value(480), "Number of pixels wide to render")
            ("output_file,o", po::value<std::string>()->default_value("image.ppm"), "File to output to");
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    if (variables_map.count("help")) {
        std::cerr << desc << '\n';
        exit(0);
    }

    PathRehndaOptions options;
    std::cerr << "Options\n";
    if (variables_map.count("num_threads")) {
        options.num_threads = variables_map["num_threads"].as<uint32_t>();
        std::cerr << "num_threads = " << options.num_threads << std::endl;
    }
    if (variables_map.count("image_width")) {
        options.image_width = variables_map["image_width"].as<uint32_t>();
        std::cerr << "image_width = " << options.image_width << std::endl;
    }
    if (variables_map.count("samples_per_thread")) {
        options.num_samples_per_thread = variables_map["samples_per_thread"].as<uint32_t>();
        std::cerr << "samples_per_thread = " << options.num_samples_per_thread << std::endl;
    }
    if (variables_map.count("output_file")) {
        options.output_file_name = variables_map["output_file"].as<std::string>();
        std::cerr << "output_file = " << options.output_file_name << std::endl;
    }
    return options;
}

// up to https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics

int main(int argc, char** argv) {
    const PathRehndaOptions options = parse_path_rehnda_options(argc, argv);

    // image properties
    const double aspect_ratio = 3.0 / 2.0;
    const uint32_t image_width = options.image_width;
    const auto image_height = static_cast<uint32_t>(image_width / aspect_ratio);
    const uint32_t samples_per_pixel_per_thread = options.num_samples_per_thread;
    const int max_depth = 10;
    const unsigned int num_threads = options.num_threads;
    std::cerr << "Using " << num_threads << " threads\n";

    // ppm image format header

    // world setup
    Point3 look_from(13, 2, 3);
    Point3 look_at(0, 0, 0);
    Vec3 up(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    const Camera camera(look_from, look_at, up, 20.0, aspect_ratio, aperture, dist_to_focus);
    const HittableList world = random_scene();
    const Sampler sampler(max_depth);
    const Aggregator aggregator;
    const SamplingConfig sampling_config{
        .sampler = sampler,
        .samples_per_pixel = samples_per_pixel_per_thread
    };
    const Scene scene{
        .camera = camera,
        .world = world
    };

    std::vector<ImageBuffer> image_buffers;
    for (size_t i = 0; i < num_threads; i++) {
        image_buffers.emplace_back(image_width, image_height);
    }

    // start parallel threads, leaving the main thread to run one instance itself
    std::vector<std::jthread> rendering_threads;
    for (size_t i = 0; i < num_threads - 1; i++) {
        rendering_threads.emplace_back([&aggregator, &sampling_config, &scene, &image_buffers, i](){
            aggregator.sample_pixels(sampling_config, scene, image_buffers[i + 1]);
        });
    }

    // run the main thread instance of sampling
    aggregator.sample_pixels(sampling_config, scene, image_buffers[0], true);
    std::cerr << "\nThread 1 done\n";

    // wait for all threads to complete
    for (auto &thread : rendering_threads) {
        thread.join();
    }
    std::cerr << "\nAll threads done\n";


    // sum the results of all sampling
    for (size_t i = 1; i < num_threads; i++) {
        image_buffers[0].add_buffer(image_buffers[i]);
    }

    // write out the image
    ImageWriter image_writer;
    std::ofstream output(options.output_file_name);
    image_writer.write_image_buffer_to_stream(output, image_buffers[0], samples_per_pixel_per_thread * num_threads);
    std::cerr << "\nDone.\n";
    return 0;
}
