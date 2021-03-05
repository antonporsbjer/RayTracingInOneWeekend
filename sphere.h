#ifndef SPHERE_H
#define SPHERE_H

#include "rt_math.h"
#include "object3d.h"


class sphere : public object3d {
    public:
        sphere() {}
        sphere(point3 sphere_center, float sphere_radius, shared_ptr<material> sphere_material) : center(sphere_center), radius(sphere_radius), material_ptr(sphere_material) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        public:
            point3 center;
            float radius;
            shared_ptr<material> material_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    
    if (discriminant < 0.0f){
        return false;
    }

    auto sqrtd = sqrt(discriminant);
    
    // Find nearest root
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max){
        root = (-half_b  + sqrtd) / a;
        if (root < t_min || root > t_max){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outwards_normal = (rec.p - center) / radius;
    rec.set_front_face_normal(r, outwards_normal);
    rec.material_ptr = material_ptr;

    return true;
}

#endif