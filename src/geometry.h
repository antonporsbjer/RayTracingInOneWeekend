#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "rt_math.h"
#include "object3d.h"

class sphere : public object3d {
    public:
        sphere() {}
        sphere(
            point3 sphere_center, 
            float sphere_radius, 
            shared_ptr<material> sphere_material
        ) : center(sphere_center), radius(sphere_radius), material_ptr(sphere_material) {};

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

class plane : public object3d {
    public:
        plane() {}
        plane(
            point3 plane_point, 
            vec3 plane_normal, 
            shared_ptr<material> plane_material
        ) : point(plane_point), normal(unit_vector(plane_normal)), material_ptr(plane_material) {};

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    public:
        point3 point;
        vec3 normal;
        shared_ptr<material> material_ptr;
};

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto denom = dot(normal, r.direction());
    if (abs(denom) > 1e-6) {
        auto dist = dot(point - r.origin(), normal) / denom;
        if (dist > t_min && dist < t_max) {
            rec.t = dist;
            rec.p = r.at(rec.t);
            rec.set_front_face_normal(r, normal);
            rec.material_ptr = material_ptr;
            return true;
        }
    }
    return false;
}

#endif