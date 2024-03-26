#version 410 core

in vec2 uv;

uniform sampler2D tex;
uniform vec2 size;

out vec4 fc;

#define T(x,y) (texture(tex,uv+vec2(x,y)/size))

// https://www.shadertoy.com/view/MsS3Wc
vec3 hsv2rgb( in vec3 c )
{
    c.yz=clamp(c.yz,vec2(0),vec2(1));
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

    return c.z * mix( vec3(1.0), rgb, c.y);
}

void main(){
    #if 0
    vec4 c = T(0,0);
    c.z /= 3.0;
    //vec3 c2=vec3(0.68, 0.85, 0.90) * abs(c.w);
    //vec3 c2 = vec3(1.0,0.65,0.0)*c.z;
    vec3 c2 = vec3(1.0,0.65,0.0)*c.z + 1.*vec3(0.68, 0.85, 0.90) * abs(c.w);
    //vec3 c2 = 0.2*vec3(1.0,0.65,0.0)*c.z + 1.*vec3(0.68, 0.85, 0.90) * max(0.0, c.w) + 1.*vec3(0.56, 0.93, 0.56)*max(0.0,-c.w);
    fc=vec4(c2,1);
    #else
    vec4 p=T(0,0);
    vec2 v=p.xy;
    float ang=atan(v.y,v.x);
    float mag=length(v);
    #define PI 3.1415
    //    vec3 c = hsv2rgb(vec3(ang/(2.*PI)+.5, 1.0, p.z/3.0)) * mag;
    //    vec3 c = hsv2rgb(vec3(ang/(2.*PI)+.5, 1.0, p.z));
    //    vec3 c = hsv2rgb(vec3(ang/(2.*PI)+.5, mag, p.z/3.0));
    //    vec3 c = hsv2rgb(vec3(ang/(2.*PI)+.5, p.z/3.0, mag/2.0));
    vec3 c = hsv2rgb(vec3(ang/(2.*PI)+.5, 1.0, mag));
    //fc=vec4(c,1);
    fc=vec4(c,1)*abs(p.w)*12.0;
    #endif
}