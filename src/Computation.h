//
// Created by Jack Armstrong on 3/26/24.
//

#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <map>
#include <string>

#include "ImageData.h"
#include "Quad.h"

class Computation {
    int width;
    int height;

    unsigned int id{};
    Quad quad;

    std::map<int, float> u1f;
    std::map<int, std::pair<float, float>> u2f;
    std::map<int, int> utex;

public:
    Computation(int width, int height, const unsigned char *source, int len);

    void uniform(const std::string &name, float value);
    void uniform(const std::string &name, float x, float y);
    void uniform(const std::string &name, const ImageData &tex);

    void run(const ImageData *target);
};


#endif //COMPUTATION_H
