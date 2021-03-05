#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.h"

struct hit_record {
    point3 p;
    vec3 normal;
    float t;
    bool front_face;

    inline void set_front_face_normal(const ray& r, const vec3& outwards_normal){
        front_face = dot(r.direction(), outwards_normal) < 0.0f;
        normal = front_face ? outwards_normal : -outwards_normal;
    }
};

class object3d {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const = 0;
};

#endif