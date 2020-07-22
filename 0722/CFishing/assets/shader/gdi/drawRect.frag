#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec4 rect;
uniform float radius;
uniform vec4 color;
uniform float linewidth;


out vec4 fragColor;

float boxDist(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
  	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

void main()
{	
	vec2 c = vec2(rect.x + rect.z/2 , rect.y - rect.w/2);
	float d = boxDist(gl_FragCoord.xy -rect.xy,rect.zw/2,radius);
	float lw = linewidth/2.0;
	d =  1.0-clamp(abs(-d/lw),0.0 ,1.0);

	fragColor = vec4(color.xyz,color.a*d);
	//fragColor = vec4(1.);
}
//vec2 c = vec2(rect.x + rect.z/2 , rect.y - rect.w/2);
//
//	float d = boxDist(gl_FragCoord.xy -c,rect.zw/2,radius);