//
// Created by Jack Armstrong on 3/26/24.
//

#include "Computation.h"

#include <iostream>
#include <gen/shader_quad_vert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImageData.h"

Computation::Computation(int width, int height, const unsigned char *source, const int len):
    width(width), height(height) {
    int ok;
    char infoLog[512];

    const unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    const auto vertSource = reinterpret_cast<const GLchar*>(shader_quad_vert);
    glShaderSource(vert, 1, &vertSource, &shader_quad_vert_len);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vert, 512, nullptr, infoLog);
        std::cout << "Vertex shader compiler error: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
    const unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    const auto fragSource = reinterpret_cast<const GLchar*>(source);
    glShaderSource(frag, 1, &fragSource, &len);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(frag, 512, nullptr, infoLog);
        std::cout << "Fragment shader compiler error: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "Program link error: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Computation::run(const ImageData *target, bool reset) {
    if (target == nullptr) {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        glViewport(0, 0, target->width, target->height);
        glBindFramebuffer(GL_FRAMEBUFFER, target->id);
    }
    glUseProgram(id);
    glUniform1f(glGetUniformLocation(id, "time"), static_cast<float>(glfwGetTime()));
    if (target == nullptr) {
        glUniform2f(glGetUniformLocation(id, "size"), width, height);
    } else {
        glUniform2f(glGetUniformLocation(id, "size"), target->width, target->height);
    }
    for(auto [ loc, value ] : u1i) {
        glUniform1i(loc, value);
    }
    for(auto [ loc, value ] : u1f) {
        glUniform1f(loc, value);
    }
    for (auto [ loc, v ] : u2f) {
        glUniform2f(loc, v.first, v.second);
    }
    for (auto [loc, tex ] : utex) {
        glUniform1i(loc, tex);
    }
    for (auto [loc, v] : u1fv) {
        glUniform1fv(loc, v.second, v.first);
    }
    for (auto [loc, v] : u2fv) {
        glUniform2fv(loc, v.second, v.first);
    }
    if (reset) {
        u1i.clear();
        u1f.clear();
        u2f.clear();
        utex.clear();
        quad.render();
    }
    if (target != nullptr) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Computation::uniform(const std::string &name, const float value) {
    u1f[glGetUniformLocation(id, name.c_str())] = value;
}

void Computation::uniform(const std::string &name, const int value) {
    u1i[glGetUniformLocation(id, name.c_str())] = value;
}

void Computation::uniform(const std::string &name, float x, float y) {
    u2f[glGetUniformLocation(id, name.c_str())] = { x, y };
}

void Computation::uniform(const std::string &name, const ImageData &value) {
    glActiveTexture(GL_TEXTURE0 + utex.size());
    glBindTexture(GL_TEXTURE_2D, value.color);
    utex[glGetUniformLocation(id, name.c_str())] = utex.size();
}

void Computation::uniform(const std::string &name, float *data, int len, int stride) {
    const int loc = glGetUniformLocation(id, name.c_str());
    if (stride == 1) {
        u1fv[loc] = { data, len };
    } else if (stride == 2) {
        u2fv[loc] = { data, len };
    } else {
        std::cout << "Cannot uniform data of stride " << stride << std::endl;
        exit(EXIT_FAILURE);
    }
}

