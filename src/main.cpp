#include "rt_math.h"
#include "camera.h"
#include "color.h"
#include "object3d_list.h"
#include "material.h"
#include "geometry.h"

#include <iostream>

/* Find ray color */
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

/* Creates a scene with a random number of spheres. */
object3d_list random_scene(){
    object3d_list world;

    auto ground_material = make_shared<lambertian>(color(0.5f, 0.5f, 0.5f));
    //world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_material = random_float();
            point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                shared_ptr<material> sphere_material;

                if (choose_material < 0.8f) {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
                // TODO: Find what causes a bug when creating metal speres.
                /*
                else if (choose_material < 0.95f) {
                    // Metal
                    auto albedo = color::random(0.5f, 1.0f);
                    auto fuzz = random_float(0.0f, 0.05f);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                } 
                */
                else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5f);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    return world;
}

int main(int argc, char **argv){
    /* Image size */
    const auto aspect_ratio = 3.0f / 2.0f;                                  // Change for diffrent aspect ratios. Widescreen: 16.0f / 9.0f;
    const int image_width = 500;                                            // Change to get different resolution. Default: 400
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;                                       // Change to get more detailed image. Default: 100;
    const int max_depth = 100;                                               // Change for more/less ray bounces. Default: 50;

    /* Custom colors */
    const color color_default(0.4f, 0.2f, 0.1f);                            
    const color color_ground(0.5f, 0.5f, 0.5f);
    const color color_red(0.8f, 0.2f, 0.2f);
    const color color_green(0.2f, 0.8f, 0.2f);
    const color color_blue(0.2f, 0.2f, 0.8f);
    const color color_yellow(0.8f, 0.8f, 0.2f);
    const color color_white(1.0f, 1.0f, 1.0f);
    
    // Metalic colors
    const color color_gold(0.9f, 0.7f, 0.2f);
    const color color_silver(0.8f, 0.8f, 0.8f);

    /* Custom material */
    // Diffuse
    auto material_ground = make_shared<lambertian>(color_ground);
    auto material_lamb_default = make_shared<lambertian>(color_default);
    auto material_lamb_red = make_shared<lambertian>(color_red);
    auto material_lamb_blue = make_shared<lambertian>(color_blue);
    auto material_lamb_green = make_shared<lambertian>(color_blue);

    // Dielectric
    auto material_glass = make_shared<dielectric>(1.5f);
    auto material_diamond = make_shared<dielectric>(2.417f);
    auto material_silicon = make_shared<dielectric>(3.45f);

    // Metal
    auto material_metal_default = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
    auto material_metal_default_ruff = make_shared<metal>(color(0.4f, 0.3f, 0.6f), 0.25f);
    auto material_metal_gold = make_shared<metal>(color_gold, 0.1f);
    auto material_metal_silver = make_shared<metal>(color_silver, 0.1f);
    auto material_metal_mirror = make_shared<metal>(color_white, 0.0f);

    // World
    auto world = random_scene();
    //auto world = object3d_list();
 
    /* Add geometry to world */
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0f, material_glass));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0f, material_lamb_default));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0f, material_metal_default));

    world.add(make_shared<plane>(point3(0,0,0), vec3(0, 1, 0), material_ground));

    /* Camera */
    point3 look_from(13,2,5);
    point3 look_at(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0f;
    auto aperture = 0.1f; //0.125f; //0.5f;

    camera camera(look_from, look_at, vup, 20.0f, aspect_ratio, aperture, dist_to_focus);

    /* Render loop */
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