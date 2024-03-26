#version 410 core

in vec2 uv;

uniform float t;
uniform float time;

out vec4 fc;

void main() {
    fc = vec4(uv*cos(t+time), 0, 1);
}