#include <windows.h>
#include <math.h>

/*
 To run it on VS code -->
    g++ main.cpp -o LineDDA.exe -lgdi32 -mwindows
    ./LineDDA.exe
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCircleBres(HDC hdc, int xc, int yc, int R, COLORREF c);

int xc, yc, R;
bool drawing = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("CircleDrawing");
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) return 0;

    HWND hwnd = CreateWindow(wc.lpszClassName, TEXT("Draw Circle"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    switch (msg) {
    case WM_LBUTTONDOWN:
        // To get center of the circle(xc, yc)
        xc = LOWORD(lParam);
        yc = HIWORD(lParam);
        drawing = true;
        break;

    case WM_LBUTTONUP:
        if (drawing) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            // Calculate R
            R = (int)sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));

            hdc = GetDC(hwnd);
            DrawCircleBres(hdc, xc, yc, R, RGB(255, 0, 0));
            ReleaseDC(hwnd, hdc);

            drawing = false;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc+x, yc+y, c);
    SetPixel(hdc, xc-x, yc+y, c);
    SetPixel(hdc, xc-x, yc-y, c);
    SetPixel(hdc, xc+x, yc-y, c);
    SetPixel(hdc, xc+y, yc+x, c);
    SetPixel(hdc, xc-y, yc+x, c);
    SetPixel(hdc, xc-y, yc-x, c);
    SetPixel(hdc, xc+y, yc-x, c);
}

// Bresenham's Circle Algorithm 
void DrawCircleBres(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0, y = R;
    int d = 1 - R;
    int d1 = 3;
    int d2 = 5 - (2*R);
    Draw8Points(hdc, xc, yc, x, y, c);

    while (x < y) {
        if (d < 0) {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        }
        else {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}
