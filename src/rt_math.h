#ifndef RT_MATH_H
#define RT_MATH_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926f;

// Utility functions
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}

inline float random_float(){
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}


inline float random_float(float min, float max){
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float_c(){
    return rand() / (RAND_MAX + 1.0f);
}

inline float random_float_c(float min, float max){
    return min + (max - min) * random_float_c();
}

inline float clamp(float x, float min, float max){
    if (x < min) { return min; }
    if (x > max) { return max; }
    return x;
}

#endif