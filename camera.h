#ifndef CAMERA_H
#define CAMERA_H

#include "rt_math.h"
#include "ray.h"


class camera{
    public:
        camera(float vert_fov, float aspect_ratio){
            auto theta = degrees_to_radians(vert_fov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0f;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0f;

            origin = point3(0.0f, 0.0f, 0.0f);
            horizontal = vec3(viewport_width, 0.0f, 0.0f);
            vertical = vec3(0.0f, viewport_height, 0.0f);
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - vec3(0.0f, 0.0f, focal_length);
        }

        ray get_ray(float u, float v) const {
            return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif