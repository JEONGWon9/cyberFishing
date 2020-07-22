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
	(texID,texCoordV) * colorV/255.;


	fragColor = (texID,texCoordV) * colorV/255.;


	
}