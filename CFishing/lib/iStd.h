#pragma once

#include "iGraphics.h"
#include "iOpenGL.h"
#include "iType.h"

#include "iImage.h"
#include "iPopup.h"
#include "iStrTex.h"

#include "iSort.h"
#include "iShortestPath.h"

#include "iFPS.h"

#include "iOpenAL.h"
#include "iVideo.h"
#include"iShaderToy.h"


#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define xprintf printf
#else
#define xprintf(...) (void)0
#endif

extern int monitorSizeW, monitorSizeH;
extern iSize devSize;
#define DEVSIZE_WIDTH 960
#define DEVSIZE_HEIGHT 480
extern iRect viewport;

void loadLib(HDC hDC);
void freeLib();
typedef void (*Method_Paint)(float dt);
void drawLib(Method_Paint method);
void keyLib(iKeyState stat, int c);
uint32 getKeyDown();
bool getKeyDown(uint32 key);
uint32 getKeyStat();
bool getKeyStat(uint32 key);
void resizeLib(int width, int height);
void zoomLib(iPoint point, float rate);
void shakeLib(int range, float duration=0.2f);
extern GLuint vertexObject, vertexBuffer;





struct iVertex
{
    float p[4];//alway [2] = 0 , [3] == 1
    iPoint uv;
    iColor4b c;

};

struct iQuad
{
    iVertex tl, tr, bl, br;
    
};

class iVBO
{

public:
    iVBO(int qNum = 1000);
    virtual ~iVBO();

    //add...ex)position..
    void paint(float dt);



public:
    iQuad* q;
    int qNum, _qNum;
    short* indices;

    Texture* tex;
    GLuint vbo;
    GLuint programID;// GLenum blendSrc, blendDst;


};


class iFBO
{
public:
    iFBO(int width, int height);
    virtual ~iFBO();

    void clear(float r, float g, float b, float a);
    void bind();
    void bind(Texture* tex);
    void unbind();

    Texture* getTexture();
    uint32 bindingTexID();

public:
    GLuint fbo;

    GLuint depthBuffer;

    Texture* tex;// colorBuffer

    GLint prevFbo;// multiple fbo 고려
    Texture** listTex;
    int listNum;
};
extern iFBO* fbo;




uint8 float2uint8(float f);
void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);

void setLineWidth(float lineWidth);
void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(float x, float y, float width, float height, float radius=0.0f);
void drawRect(iRect rt, float radius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius=0.0f);
void fillRect(iRect rt, float radius=0.0f);

void drawCircle(iPoint p, float radius, float linewidth);
void drawCircle(float x, float y, float radius, float linewidth);

void fillCircle(iPoint p, float radius);
void fillCircle(float x, float y, float radius);

void drawEllipse(float x, float y, float radius0,float radius1, float linewidth, float degree);
void drawEllipse(iPoint p, float radius0, float radius1, float linewidth, float degree);

void fillEllipse(float x, float y, float radius0, float radius1, float degree);
void fillEllipse(iPoint p, float radius0, float radius1, float degree);

void shaderdrawline(iPoint sp, iPoint ep);

void saveImageFromRGBA(const char* path, uint8* rgba, int width, int height);
uint8* bmp2rgba(Bitmap* bmp, int& width, int& height);

Texture* createTexture(int width, int height, bool rgba32f=false);
void setTexture(TextureWrap wrap, TextureFilter filter);

#ifdef _DEBUG
extern int texNum;
#endif
Texture* createImage(const char* szFormat, ...);
Texture* createGreyImage(const char* szFormat, ...);
Texture* createReflectImage(float rateY, const char* szFormat, ...);
Texture** createDivideImage(int wNum, int hNum, const char* szFormat, ...);
void freeImage(Texture* tex);

void drawImageLikeCircle(Texture* tex, int x, int y, float rate);
void drawImage(Texture* tex, int x, int y, int anc);
void drawImage(Texture* tex, int x, int y,
    int ix, int iy, int iw, int ih, int anc,
    float ratX, float ratY, int xyz, float degree,
    int reverse=REVERSE_NONE);

void setClip(int x, int y, int width, int height);
void setClip(iRect rect);



void setStringName(const char* name);
const char* getStringName();
void setStringSize(float size);
float getStringSize();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorder(float size);
float getStringBorder();
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
void drawString(int x, int y, int anc, const char* szFormat, ...);
iSize sizeOfString(const char* szFormat, ...);

uint32 random();
float _sin(float degree);
float _cos(float degree);
float linear(float r, float a, float b);
iPoint linear(float r, iPoint a, iPoint b);
float easeIn(float r, float a, float b);
iPoint easeIn(float r, iPoint a, iPoint b);
float easeOut(float r, float a, float b);
iPoint easeOut(float r, iPoint a, iPoint b);
float easeInOut(float r, float a, float b);
iPoint easeInOut(float r, iPoint a, iPoint b);

bool containPoint(iPoint p, iRect rt);
bool containRect(iRect src, iRect dst);

char* loadFile(const char* filePath, int& length);
void saveFile(const char* filePath, char* buf, int bufLength);

