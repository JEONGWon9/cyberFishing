#version 150

#if GL_ES
precision mediump float;
#endif

in vec2 texCoordV;
in vec4 colorV;

uniform sampler2D texBase;// back buffer
uniform sampler2D texBlend;



out vec4 fragColor;

void main()
{
	 vec4 c = texture(texBlend, texCoordV) * colorV;
   fragColor = c;


	
}