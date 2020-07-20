#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float linewidth;


out vec4 fragColor;

void main()
{	

	float d = length(gl_FragCoord.xy - center);
	float lw = linewidth/2.0;
	d = 1.0 - clamp(abs(radius-d),0.0 ,lw)/lw;
	fragColor = vec4(color.rgb, color.a * d);

	
}

