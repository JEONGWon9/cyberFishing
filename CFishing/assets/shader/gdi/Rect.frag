#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec2 center;
uniform float radius;
uniform vec4 color;



out vec4 fragColor;

float boxDist(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
  	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void main()
{	



	fragColor =color;

	
}

