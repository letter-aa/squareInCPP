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
typedef struct { int x1; int y1; int x2; int y2; } posEx;
pos center;
void newLine(HWND hwnd, HDC hdc, Color color, float thickness, int x1, int y1, int x2, int y2) {
    Graphics graphics(hdc);
    Pen pen(color, thickness);
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}
/////////////////////////////////////////////////
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
void newSquare(squareProperty sqPrpty, posEx left, posEx bottom, posEx top, posEx right) {
    int size = sqPrpty.size;
    HWND hwnd = sqPrpty.hwnd;
    HDC hdc = sqPrpty.hdc;
    Color color = sqPrpty.color;
    int thickness = sqPrpty.thickness;
    int x = sqPrpty.x;
    int y = sqPrpty.y;
    size /= 2;
    if (!(left.x1 == NULL && left.x2 == NULL && left.y1 == NULL && left.y2 == NULL)) {
        newLine(hwnd, hdc, color, thickness, left.x1, left.y1, left.x2, left.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y - size, x - size, y + size);
    }

    if (!(bottom.x1 == NULL && bottom.x2 == NULL && bottom.y1 == NULL && bottom.y2 == NULL)) {
        newLine(hwnd, hdc, color, thickness, bottom.x1, bottom.y1, bottom.x2, bottom.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y + size, x + size, y + size);
    }

    if (!(top.x1 == NULL && top.x2 == NULL && top.y1 == NULL && top.y2 == NULL)) {
        newLine(hwnd, hdc, color, thickness, top.x1, top.y1, top.x2, top.y2);
    }
    else {
        newLine(hwnd, hdc, color, thickness, x - size, y - size, x + size, y - size);
    }

    if (!(right.x1 == NULL && right.x2 == NULL && right.y1 == NULL && right.y2 == NULL)) {
        newLine(hwnd, hdc, color, thickness, right.x1, right.y1, right.x2, right.y2);
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
void rotateSquare(HWND hwnd, HDC hdc, squareProperty sqPrpty, int rotation) {
    coverScreen(hwnd, hdc, WHITE_BRUSH);
    if ((rotation / 180) >= 1) {
        rotation = rotation - 180 * floor(rotation / 180);
    }
    else if ((rotation / -180) >= -1) {
        rotation = rotation - -180 * floor(rotation / -180);
    }
    Color color = sqPrpty.color;
    int thickness = sqPrpty.thickness;
    if (rotation < 0) {
        newLine(hwnd, hdc, color, thickness, 100, 100, 100, 200);
        newLine(hwnd, hdc, color, thickness, 100, 200, 200, 200);
        newLine(hwnd, hdc, color, thickness, 100, 100, 200, 100);
        newLine(hwnd, hdc, color, thickness, 200, 100, 200, 200);
    }
}
void customDraw(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    updateCenter(hwnd);
    cout << center.x << "\n" << center.y;
    squareProperty sqPrpty1{ hwnd, hdc, 100, center.x, center.y, 1, Color(255, 255, 0, 0) };
    posEx left, bottom, top, right;
    left = { NULL,NULL,NULL,NULL };
    bottom = { NULL,NULL,NULL,NULL };
    top = { NULL,NULL,NULL,NULL };
    right = { NULL,NULL,NULL,NULL };
    //------------------------------
    
    //newLine(hwnd, hdc, Color(255, 255, 0, 0), 10, center.x, center.y, center.x + 1, center.y + 1);
    newSquare(sqPrpty1,left, bottom, top, right);
    //Sleep(1000);
    //coverScreen(hwnd, hdc, WHITE_BRUSH);

    //---------------------------------
    EndPaint(hwnd, &ps);
}
auto newPixel(int x, int y, int size, int color) {
    /*
    2584
    2588
    2580
    ////
    254u
    25AE
    */
    char pixel = 254u;
    HANDLE Con = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < y; i++) {
        cout << "\n";
    }
    //SetConsoleTextAttribute(Con, 0);
    for (int i = 0; i < x; i++) {
        cout << " ";
    }
    SetConsoleTextAttribute(Con, color);
    for (int i = 0; i < size; i++) {
        cout << "\n";
        for (int i = 0; i < x; i++) {
            cout << "  ";
        }
        for (int i = 0; i < size; i++) {
            cout << pixel;
        }
    }
    SetConsoleTextAttribute(Con, 7);
    struct pos { int x; int y; };
    return pos{ x,y };
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
