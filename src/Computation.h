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

    std::map<int, int> u1i;
    std::map<int, float> u1f;
    std::map<int, std::pair<float, float>> u2f;
    std::map<int, int> utex;
    std::map<int, std::pair<float*, int>> u1fv;
    std::map<int, std::pair<float*, int>> u2fv;

public:
    Computation(int width, int height, const unsigned char *source, int len);

    void uniform(const std::string &name, int value);
    void uniform(const std::string &name, float value);
    void uniform(const std::string &name, float x, float y);
    void uniform(const std::string &name, const ImageData &tex);
    void uniform(const std::string &name, float* data, int len, int stride);

    void run(const ImageData *target, bool reset=true);
};


#endif //COMPUTATION_H
