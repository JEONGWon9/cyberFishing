#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec2 sp,ep;
uniform vec4 color;
uniform float linewidth;


out vec4 fragColor;
float drawLine(vec2 p, vec2 start, vec2 end, float width)
{
	vec2 n = end -start;
	float len = length(n);
	n /= len;

	vec2 m = p - start;
	vec2 proj = clamp(dot(m,n), 0.0, len) * n;
	return length(m-proj) - width / 2.0;

}

void main()
{	

	float d = drawLine(gl_FragCoord.xy,sp,ep,linewidth);
	fragColor = vec4(color.xyz,color.a* clamp(-d,0.0,1.0));
//	d = d/(linewidth/2.0);
//	float a = clamp(-d,0.0,1.0);
//	a = a * a * a * a;
//	fragColor = vec4(color.xyz,color.w* a);// light

	
}
