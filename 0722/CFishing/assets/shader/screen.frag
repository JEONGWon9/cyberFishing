#version 150

#if GL_ES
precision mediump float;
#endif

in vec2 texCoordV;
in vec4 colorV;

uniform sampler2D texID;
uniform vec2 resolution;


out vec4 fragColor;

void main()
{
	vec4 c = texture(texID,texCoordV) * colorV/255.;
	//float grey = 0.299*c.r + 0.587*c.g + 0.114*c.b;
	//fragColor = vec4(grey,grey,grey,c.a);

	fragColor = c.rgba;


	
}