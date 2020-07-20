#version 150

#if GL_ES
precision mediump float;
#endif


uniform mat4 mProjection,mModelview;


in vec4 position;
in vec2 texCoord;
in vec4 color;

out vec2 texCoordV;
out vec4 colorV;



void main()
{

	//top left  0,1	   top right 1,1
	//bottom_left 0,0  bottom_right 1,0
	gl_Position = mProjection * mModelview * position;
	colorV = color/255.;
	texCoordV = texCoord;




}