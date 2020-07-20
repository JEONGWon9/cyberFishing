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

	vec2 size = textureSize(texBase,0);
	vec2 uv =  gl_FragCoord.xy / size;
	vec4 base = texture(texBase,uv);

	vec4 blend = texture(texBlend,texCoordV);

	
	fragColor = vec4(base.rgb + blend.rgb * blend.a ,base.a);


	
}