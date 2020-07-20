#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec2 center;
uniform float a;
uniform float b;
uniform float radian;
uniform vec4 color;

out vec4 fragColor;

vec2 rotate(vec2 p, float radian)
{
    // ccw
    mat2 m = mat2(cos(radian), -sin(radian), sin(radian), cos(radian));
    // cw
    //mat2 m = mat2(cos(radian), -sin(radian), sin(radian), cos(radian));
    return p * m;
}

float ellipse(vec2 p, float a, float b)
{
    a = 1.0/a;
    b = 1.0/b;
    return (length( p*vec2(a,b) )-1.0)/(length(p*vec2(a*a,b*b)));
}

void main()
{
    vec2 p = rotate(gl_FragCoord.xy - center, radian);
    float d = ellipse(p, a, b);
    fragColor = vec4(color.rgb, color.a * clamp(-d, 0.0, 1.0));
}
