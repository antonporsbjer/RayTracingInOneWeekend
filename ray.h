#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray{
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction) : ori(origin), dir(direction) {}

        point3 origin() const { return ori; }
        vec3 direction() const { return dir; }
        point3 at(float t) const { return ori + t * dir; }

    
    public:
        point3 ori;
        vec3 dir;
};

#endif