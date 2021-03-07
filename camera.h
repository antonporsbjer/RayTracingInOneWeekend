#ifndef CAMERA_H
#define CAMERA_H

#include "rt_math.h"
#include "ray.h"


class camera{
    public:
        camera(point3 look_from, point3 look_at, vec3 v_up, float vert_fov, float aspect_ratio){
            auto theta = degrees_to_radians(vert_fov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0f * h;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0f;

            auto look_dir = unit_vector(look_from - look_at);
            auto u = unit_vector(cross(v_up, look_dir));
            auto v = cross(look_dir, u);

            origin = look_from;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - look_dir;
        }

        ray get_ray(float s, float t) const {
            return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif