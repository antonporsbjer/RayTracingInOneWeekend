#include <iostream>
#include "vec3.h"
#include "color.h"

int main(int argc, char **argv){

    // Image size
    const int image_width = 512;//256;
    const int image_height = 512;//256;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j){
        std::cerr << "\rScaning remaining: " << j << " " << std::flush;
        for(int i = 0; i < image_width; ++i){
            color pixel_color(float(i) / (image_width-1), float(j) / (image_height-1), 0.25);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}