#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <thread>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
using namespace std;
void newLine(HWND hwnd, HDC hdc, Color color, float thickness, int x1, int y1, int x2, int y2) {
    Graphics graphics(hdc);
    Pen pen(color, thickness);
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}
void updateSize(HWND hwnd, int* pHeight, int* pWidth) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    *pWidth = rect.right - rect.left;
    *pHeight = rect.bottom - rect.top;
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
    //------------------------------

    newLine(hwnd, hdc, Color(255, 0, 0, 0), 1.5, 100, 100, 100, 200);
    newLine(hwnd, hdc, Color(255, 0, 0, 0), 1.5, 100, 200, 200, 200);
    newLine(hwnd, hdc, Color(255, 0, 0, 0), 1.5, 100, 100, 200, 100);
    newLine(hwnd, hdc, Color(255, 0, 0, 0), 1.5, 200, 100, 200, 200);
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
        MessageBox(NULL, TEXT("Registering window class failed! Please try again."), TEXT("An error has occured!"), NULL);
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
        MessageBox(NULL, TEXT("Window does not exist!"), TEXT("An error has occured!"), NULL);
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