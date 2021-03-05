#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

float hit_sphere(const point3& sphere_center, float radius, const ray& r){
    vec3 oc = r.origin() - sphere_center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0f * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4*a*c;
    if (discriminant < 0.0f){
        return -1.0f;
    } else {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }
}

color ray_color(const ray& r){
    auto t = hit_sphere(point3(0.0f, 0.0f, -1.0f), 0.5f, r);
    if (t > 0.0f){
        vec3 N = unit_vector(r.at(t) - vec3(0.0f, 0.0f, -1.0f));
        return 0.5f * color(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5f * (unit_direction.y() + 1.0f);
    
    return (1.0f - t) * color(1.0f,1.0f,1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main(int argc, char **argv){

    // Image size
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height =static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = point3(0.0f, 0.0f, 0.0f);
    auto horizontal = vec3(viewport_width, 0.0f, 0.0f);
    auto vertical = vec3(0.0f, viewport_height, 0.0f);
    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - vec3(0.0f, 0.0f, focal_length);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j){
        std::cerr << "\rScaning remaining: " << j << " " << std::flush;
        for(int i = 0; i < image_width; ++i){
            auto u = float(i) / (image_width -1);
            auto v = float(j) / (image_height -1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}