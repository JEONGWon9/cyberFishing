#version 150

#if GL_ES
precision mediump float;
#endif

uniform vec2 center;
uniform float radius;
uniform vec4 color;



out vec4 fragColor;

void main()
{	

	float d = length(gl_FragCoord.xy - center);
	d= clamp(radius-d , 0.0 ,1.0);
	fragColor = vec4(color.rgb, color.a * d);

	//fragColor =color;

	
}

