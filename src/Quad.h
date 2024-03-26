//
// Created by Jack Armstrong on 3/26/24.
//

#ifndef QUAD_H
#define QUAD_H

class Quad {

private:
    unsigned int id{};
    unsigned int ebo{};
    unsigned int vbo{};

public:
    Quad();

    void render() const;

};



#endif //QUAD_H
