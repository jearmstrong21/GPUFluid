//
// Created by Jack Armstrong on 3/26/24.
//

#ifndef IMAGEDATA_H
#define IMAGEDATA_H



class ImageData {

public:
    unsigned int id{};
    unsigned int color{};

    int width;
    int height;

    explicit ImageData(int width, int height, bool wrap);

};



#endif //IMAGEDATA_H
