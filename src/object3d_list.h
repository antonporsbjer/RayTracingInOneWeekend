#ifndef OBJECT3D_LIST_H
#define OBJECT3D_LIST_H

#include "rt_math.h"
#include "object3d.h"

#include <memory>
#include <vector>


using std::shared_ptr;
using std::make_shared;
using std::vector;

class object3d_list : public object3d{
    public:
        object3d_list() {}
        object3d_list(shared_ptr<object3d> obj) { add(obj); }

        void clear() { obj_list.clear(); }
        void add(shared_ptr<object3d> obj) { obj_list.push_back(obj); }

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        public:
            vector<shared_ptr<object3d>> obj_list;
};

bool object3d_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_hit = t_max;

    for (const auto& obj : obj_list) {
        if (obj->hit(r, t_min, closest_hit, temp_rec)) {
            hit_anything = true;
            closest_hit = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif