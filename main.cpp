
#include "rt_math.h"
#include "camera.h"
#include "color.h"
#include "object3d_list.h"
#include "material.h"
#include "sphere.h"

#include <iostream>


color ray_color(const ray& r, const object3d& world, int depth){
    hit_record rec;
    if (depth <= 0) { return color(0.0f, 0.0f, 0.0f); } // If ray bounce limit is exceeded.
    if (world.hit(r, 0.001f, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0.0f, 0.0f, 0.0f);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5f * (unit_direction.y() + 1.0f);
    
    return (1.0f - t) * color(1.0f,1.0f,1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main(int argc, char **argv){

    // Image size
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height =static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    auto R = cos(pi / 4);
    object3d_list world;

    auto material_left = make_shared<lambertian>(color(0.0f, 0.0f, 1.0f));
    auto material_right = make_shared<lambertian>(color(1.0f, 0.0f, 0.0f));

    world.add(make_shared<sphere>(point3(-R, 0.0f, -1.0f), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0.0f, -1.0f), R, material_right));

    // Camera
    camera camera(90.0f, aspect_ratio);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j){
        std::cerr << "\rScaning remaining: " << j << " " << std::flush; // Render progress!
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0.0f, 0.0f, 0.0f);
            for(int s = 0; s < samples_per_pixel; ++s){
                auto u = (i + random_float()) / (image_width - 1);
                auto v = (j + random_float()) / (image_height - 1);
                ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}