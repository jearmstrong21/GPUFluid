#version 410 core

in vec2 uv;

uniform sampler2D tex;

uniform vec2 size;
uniform float time;

out vec4 col;

#define K 0.2
#define dt 0.15
//#define VISC 0.55

#define T(x, y) (texture(tex, uv + vec2(x, y) / size))

uniform float viscosity;
uniform float vorticityConfinement;

#define MAX_SOURCE_COUNT 10
uniform int sourceCount;
uniform vec2 sourceLoc[MAX_SOURCE_COUNT];
uniform vec2 sourceDir[MAX_SOURCE_COUNT];
uniform float sourceSize[MAX_SOURCE_COUNT];
uniform float sourceStrength[MAX_SOURCE_COUNT];

void makeSource(inout vec4 fc, inout vec2 extf, vec2 loc, vec2 dir, float size, float strength) {
    float dist=length(uv-loc);
    float f=0.00001*strength/4./(pow(dist, size)+0.0001);
    float a=0. + 0.*cos(time*2.0)*0.5;
    extf+=f*dir;
}

void main(){
    vec2 cscale = vec2(0.5);
    float S = K / dt;

    vec4 fc = T(0, 0);
    vec4 fr = T(1, 0);
    vec4 fl = T(-1, 0);
    vec4 ft = T(0, 1);
    vec4 fd = T(0, -1);

    vec3 udx = vec3(fr.xyz - fl.xyz) * 0.5;
    vec3 udy = vec3(ft.xyz - fd.xyz) * 0.5;

    float udiv = udx.x + udy.y;
    vec2 ddx = vec2(udx.z, udy.z);

    fc.z -= dt * dot(vec3(ddx, udiv), fc.xyz); // density

    fc.z = clamp(fc.z, 0.0, 3.0); // stability?

    vec2 pdx = S * ddx;
    vec2 laplacian = fr.xy+fl.xy+ft.xy+fd.xy - 4.0 * fc.xy;
    vec2 viscf = viscosity * laplacian;

    vec2 was = uv - dt * fc.xy / size.xy;
    fc.xyw = texture(tex, was).xyw;

    vec2 extf = vec2(0);

    for(int i = 0; i < sourceCount; i++) {
        vec2 d = sourceDir[i];
        makeSource(fc, extf, sourceLoc[i], d.x*vec2(cos(d.y),sin(d.y)), sourceSize[i], sourceStrength[i]);
    }

    fc.xy += dt * (viscf - K/dt*pdx + extf); // forces
    fc.xy = max(vec2(0), abs(fc.xy) - 1e-4) * sign(fc.xy);

    #if 1
    fc.w = (fr.y - fl.y + fd.x - ft.x);
    vec2 vorticity = vec2(abs(ft.w) - abs(fd.w), abs(fl.w) - abs(fr.w));
    vorticity *= vorticityConfinement / length(vorticity + 1e-9) * fc.w;
    // recommend 0.11, not 0.01
    //    if (uv.x<0.02||uv.y<0.02||uv.x>=0.98||uv.y>=0.98)vorticity*=0.;
    vorticity *= smoothstep(.5,.48,abs(uv.x-0.5));
    vorticity *= smoothstep(.5,.48,abs(uv.y-0.5));
    fc.xy += dt * vorticity;
    #endif

    // TODO: boundary
    fc.x *= smoothstep(.5,.48, abs(uv.x-0.5));
    fc.y *= smoothstep(.5,.48, abs(uv.y-0.5));
    fc=clamp(fc,vec4(-10,-10,.5,-10),vec4(10,10,3,10));

    //fc.z *= 0.999;
    fc.xy*=0.99999;

    col = vec4(fc);

}