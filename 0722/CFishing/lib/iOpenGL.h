#pragma once
#include <windows.h>

// GLEW_STATIC 전처리기에 정의.
#include <GL/glew.h>
#include <GL/wglew.h>
//#include <GL/GL.h>
//#include <GL/GLU.h>

// glew.c 파일을 프로젝트 추가.

#pragma comment(lib, "opengl32.lib")
#include "iMatrix.h"
extern iMatrix* mProjection;
extern iMatrix* mModelview;

void setupOpenGL(bool setup, HDC hDC);
bool startGLEW();
void initOpenGL();
void reshapeOpenGL(int width, int height);

GLuint createShader(const char* str, GLuint frag);
void destroyShader(GLuint id);
GLuint createProgramID(GLuint vertID, GLuint fragID);

enum iBlend
{
    iBlendAlpha = 0,
    iBlendGrey,
    iBlendAdd,



    iBlendMax,

};
void setGLBlend(iBlend blend);
GLuint getProgramID();
void destroyProgramID(GLuint programID);

void setAntiAliasParameters(bool anti);


GLuint nextPOT(GLuint x);
struct Texture;
Texture* createImageWithRGBA(GLubyte* rgba, GLuint width, GLuint height);

/*
	라이브러리 접근방법
	1. compile : header file include (*.obj, *.o)
	2. link : .lib include (archive *.obj, make lib or exe)
*/
