#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt_math.h"


struct hit_record;

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material{
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override { 
            auto scatter_direction = rec.normal + random_unit_vector();
            
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered =ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

        public:
            color albedo;
};

class metal : public material {
    public:
        metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        float fuzz;
};

class dialectric : public material {
    public:
        dialectric(float refraction_index) : ref_index(refraction_index) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0f, 1.0f, 1.0f);
            float refraction_ratio = rec.front_face ? (1.0f / ref_index) : ref_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            float cos_theta = fmin(dot(unit_direction, rec.normal), 1.0f);
            float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
            vec3 direction;

            // TODO: Fix bug where the if-statement always reflect and don't refract!
            //if (cannot_refract || reflectance(cos_theta, ref_index) > random_float()) {
            //    direction = reflect(unit_direction, rec.normal);
            //} else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            //}

            scattered = ray(rec.p, direction);
            return true;
        }

        public:
            float ref_index;

        private:
            static float reflectance(float cosine, float ref_idx) {
                // Schlick's approximation
                auto r0 = (1 - 1.5f) / (1 + 1.5f);
                r0 = r0 * r0;
                return r0 + (1 - r0) * pow((1 - cosine), 5);
            }
};

#endif