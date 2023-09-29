#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <thread>
#define PI 3.14159
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
using namespace std;
typedef struct { HWND hwnd; HDC hdc; int size; int x; int y; int thickness; Color color; } squareProperty;
typedef struct { int x; int y; } pos;
typedef struct { int x1; int x2; int y1; int y2; } posEx;
pos center;
void newLine(HWND hwnd, HDC hdc, Color color, float thickness, int x1, int y1, int x2, int y2) {
    Graphics graphics(hdc);
    Pen pen(color, thickness);
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}
/////////////////////////////////////////////////
void rotateSquare(squareProperty sqPrpty, int rotation) {
    HWND hwnd = sqPrpty.hwnd;
    HDC hdc = sqPrpty.hdc;
    Color color = sqPrpty.color;
    int thickness = sqPrpty.thickness;
    newLine(hwnd, hdc, color, thickness, 100, 100, 100, 200);
    newLine(hwnd, hdc, color, thickness, 100, 200, 200, 200);
    newLine(hwnd, hdc, color, thickness, 100, 100, 200, 100);
    newLine(hwnd, hdc, color, thickness, 200, 100, 200, 200);
}
void updateSize(HWND hwnd, int* pHeight, int* pWidth) {
    RECT clientRect;
    RECT windowRect;
    GetClientRect(hwnd, &clientRect);
    GetWindowRect(hwnd, &windowRect);
    *pWidth = (clientRect.right - clientRect.left) - ((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left));
    //*pWidth = windowRect.right - windowRect.left;
    *pHeight = clientRect.bottom - clientRect.top;
}
void updateCenter(HWND hwnd) {
    int height{ 0 }, width{ 0 };
    updateSize(hwnd, &height, &width);
    center.x = width / 2;
    center.y = height / 2;
}
void newSquare(squareProperty sqPrpty, posEx pos1, posEx pos2, posEx pos3, posEx pos4) {
    int size = sqPrpty.size;
    HWND hwnd = sqPrpty.hwnd;
    HDC hdc = sqPrpty.hdc;
    Color color = sqPrpty.color;
    int thickness = sqPrpty.thickness;
    int x = sqPrpty.x;
    int y = sqPrpty.y;
    size /= 2;
    if (!pos1.x1 == NULL) {
        newLine(hwnd, hdc, color, thickness, pos1.x1, pos1.y1, pos1.x2, pos1.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y - size, x - size, y + size);
    }

    if (!pos2.x1 == NULL) {
        newLine(hwnd, hdc, color, thickness, pos2.x1, pos2.y1, pos2.x2, pos2.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y + size, x + size, y + size);
    }

    if (!pos3.x1 == NULL) {
        newLine(hwnd, hdc, color, thickness, pos3.x1, pos3.y1, pos3.x2, pos3.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y - size, x + size, y - size);
    }

    if (!pos4.x1 == NULL) {
        newLine(hwnd, hdc, color, thickness, pos4.x1, pos4.y1, pos4.x2, pos4.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x + size, y - size, x + size, y + size);
    }
}


void coverScreen(HWND hwnd, HDC hdc, int brushColor){
    int width{ 0 }, height{ 0 };
    updateSize(hwnd, &height, &width);
    SelectObject(hdc, GetStockObject(brushColor));
    Rectangle(hdc, 0, 0, width, height);
}
void customDraw(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    updateCenter(hwnd);
    cout << center.x << "\n" << center.y;
    squareProperty sqPrpty1{ hwnd, hdc, 100, center.x, center.y, 1, Color(255, 255, 0, 0) };
    posEx pos1, pos2, pos3, pos4;
    pos1 = { NULL,NULL,NULL,NULL };
    pos2 = { NULL,NULL,NULL,NULL };
    pos3 = { NULL,NULL,NULL,NULL };
    pos4 = { NULL,NULL,NULL,NULL };
    //------------------------------
    
    //newLine(hwnd, hdc, Color(255, 255, 0, 0), 10, center.x, center.y, center.x + 1, center.y + 1);
    newSquare(sqPrpty1,pos1, pos2, pos3, pos4);
    //Sleep(1000);
    //coverScreen(hwnd, hdc, WHITE_BRUSH);

    //---------------------------------
    EndPaint(hwnd, &ps);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp) {
    switch (uint) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uint, wp, lp);
    }
}

int WINAPI newWindow(string title, int sizeX, int sizeY, bool scrollingAndButtonsEnabled) {
    HINSTANCE hin{ 0 };
    HINSTANCE hprevin{ 0 };
    LPSTR lpcmdl = NULL;
    int nshowcmd = SW_SHOW;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    ULONG_PTR token;
    GdiplusStartupInput gdipsi;
    GdiplusStartup(&token,&gdipsi,NULL);

    int option = 0;
    if (scrollingAndButtonsEnabled == false) {
        option = WS_OVERLAPPED;
    }
    else if (scrollingAndButtonsEnabled == true) {
        option = WS_OVERLAPPEDWINDOW;
    }
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hin;
    wc.hCursor = LoadCursor(hin, IDC_ARROW);
    wc.hIcon = LoadIcon(hin, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"main";
    wc.hIconSm = LoadIcon(hin, IDI_APPLICATION);
    wc.lpfnWndProc = WindowProc;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Registering window class failed!"), TEXT("An error has occured!"), NULL);
        return 1;
    }

    hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        L"main",
        (L"" + wstring(title.begin(), title.end())).c_str(),
        option,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        sizeX,
        sizeY,
        NULL,
        NULL,
        hin,
        NULL
    );

    if (!hwnd) {
        MessageBox(NULL, TEXT("Window could not be created!"), TEXT("An error has occured!"), NULL);
        return 1;
    }

    ShowWindow(hwnd, nshowcmd);
    //SendMessage(hwnd, WM_PAINT, 0, 0);
    thread thread1(customDraw, hwnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    thread1.join();
    GdiplusShutdown(token);

    return 0;
}
int main()
{
    newWindow("square!!", 400, 400, false);
}
