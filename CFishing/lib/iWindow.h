#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32")

#include <gl/glew.h>
#include <gl/wglew.h>
//#include <gl/GL.h>
//#include <gl/GLU.h>
#pragma comment(lib, "opengl32")

enum WndStyle {
    WndStyle_static = 0,

    WndStyle_button,
    WndStyle_checkBox,
    WndStyle_radio,

    WndStyle_comboBox,

    WndStyle_listBox,
    WndStyle_editBox,

    WndStyle_trackbar,

    WndStyle_opengl,
};

typedef void (*WndDrag)(const char* path);
typedef LRESULT(*WndCtrlColor)(HDC hdc, HWND hwnd);
typedef void (*WndCtrlUpdate)(HWND hwnd);

struct WndCtrl
{
    HWND hwnd;
    WndStyle style;
    WndCtrlColor color;
    WndCtrlUpdate update;
};

class WndCtrlSystem
{
public:
    WndCtrlSystem(HWND hwnd);
    virtual ~WndCtrlSystem();

    void dragAcceptFiles(WndDrag drag);
    void dropFiles(WPARAM wParam, LPARAM lParam);

    void add(HWND, WndStyle, WndCtrlColor, WndCtrlUpdate);
    LRESULT color(WPARAM wParam, LPARAM lParam);
    void update(WPARAM wParam, LPARAM lParam);
    void scroll(WPARAM wParam, LPARAM lParam);

public:
    WndDrag methodDrag;
    HWND hwndParent;
    HINSTANCE hinstance;
    WndCtrl* wc;
    int wcNum;
};

void initWndCtrlSystem();
void setWndCtrlStyem(WndCtrlSystem* wcs);

//
// Static
//
HWND createWndStatic(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

//
// Button
//
HWND createWndButton(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

HWND createWndCheckBox(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);
bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);

HWND createWndRadio(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

//
// Group
//
HWND createWndGroup(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

//
// ComboBox
//
HWND createWndComboBox(int x, int y, int width, int height,
    const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update);
void addWndComboBox(HWND hwnd, int index, const char* str);
void addWndComboBox(HWND hwnd, const char** line, int lineNum);
void removeWndComboBox(HWND hwnd, int index);
int countWndComboBox(HWND hwnd);
int indexWndComboBox(HWND hwnd);
char* getWndComboBox(HWND hwnd, int index);
void setWndComboBox(HWND hwnd, int index);

//
// ListBox
//
HWND createWndListBox(int x, int y, int width, int height,
    const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** line, int lineNum);
void removeWndListBox(HWND hwnd, int index);
int countWndListBox(HWND hwnd);
int indexWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);

//
// EditBox
//
enum WndEditBoxStyle {
    WndEditBoxStyle_all = 0,
    WndEditBoxStyle_int,
    WndEditBoxStyle_float,
};

void setWndEditBoxLength(int maxLength);
HWND createWndEditBox(int x, int y, int width, int height,
    const char* str, WndEditBoxStyle style, WndCtrlColor color, WndCtrlUpdate update);
HWND createWndEditBoxMultiline(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);



HWND createWndTrackBar(int x,int y, int width, int height,
    const char* str,WndCtrlColor color, WndCtrlUpdate update, bool displayTicks);
HWND* addWndTrackBarBuddy(HWND hwnd, const char* str);
void setWndTrackBarRange(HWND hwnd,int MIN, int MAX);
void setWndTrackBarPos(HWND hwnd, int POS);
int getWndTrackBarPos(HWND hwnd);



void enableWnd(HWND hwnd, bool enable);
void setWndText(HWND hwnd, const char* szFormat, ...);
char* getWndText(HWND hwnd);
int getWndInt(HWND hwnd);
float getWndFloat(HWND hwnd);

HWND createWndDlg(int x, int y, int width, int height,
    const char* strTitle, WNDPROC wndProc);

typedef void (*methodChooseColor)(int r, int g, int b);
void showChooseColor(methodChooseColor method);

//const char* openFileDlg(bool open, const char* filter);
const char* openFileDlg(bool open, LPCWSTR filter);

HANDLE startOnlyRun(const wchar_t* className);
void endOnlyRun(HANDLE handle);

class iCriticalSection
{
private:
    iCriticalSection();
public:
    static iCriticalSection* instance();
    virtual ~iCriticalSection();

    void start();
    void end();

private:
    CRITICAL_SECTION cs;
};

class iOpenGL
{
public:
    // for Games
    iOpenGL(int x, int y, int width, int height, int bits, const char* name, bool fullscreen);
    // for Ctrl
    iOpenGL(int x, int y, int width, int height, int bits, bool visible = true);
    virtual ~iOpenGL();

    void setMakeCurrent();
    void swapBuffer();

private:
    void dispose();

public:
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
};

#include "iStd.h"

typedef void (*MethodWndGLUpdate)(float dt);
struct WndGL
{
    iOpenGL* gl;
    GLuint vao;
    MethodWndGLUpdate method;
    iSize devSize;
   
    
};

extern iArray* arrayGL;

void setupOpenGL(bool setup);
WndGL* createOpenGL(int x, int y, int width, int height,
    MethodWndGLUpdate m, int devWidth, int devHeight);
void updateOpenGL(float dt);

bool checkOpenGLPosition(HWND hwndParent, HWND hwndOpengl, iPoint& point);
