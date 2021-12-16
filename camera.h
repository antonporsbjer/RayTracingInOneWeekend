#ifndef CAMERA_H
#define CAMERA_H

#include "rt_math.h"
#include "ray.h"


class camera{
    public:
        camera(
            point3 look_from, 
            point3 look_at, 
            vec3 v_up, 
            float vert_fov, 
            float aspect_ratio,
            float aperture,
            float focus_dist
            ){
                
            auto theta = degrees_to_radians(vert_fov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0f * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(v_up, w));
            v = cross(w, u);

            origin = look_from;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;

            lens_radius = aperture / 2;
        }

        ray get_ray(float s, float t) const {
            vec3 rd = lens_radius * random_point_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                    origin + offset, 
                    lower_left_corner + s * horizontal + t * vertical - origin - offset
                    );
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
};

#endif