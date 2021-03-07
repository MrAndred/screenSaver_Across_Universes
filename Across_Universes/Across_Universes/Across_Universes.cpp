// Across_Universes.cpp : Определяет точку входа для приложения.
//


#include "framework.h"
#include "Across_Universes.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:

struct Star  // Структура описывающая точку(звезду) в пространстве
{
    float X;
    float Y;
    float Z;
};

const int Speed = 5;           //  Скорость полета точек(звезд)
const int StarCount = 1000;  //  Кол-во генерируемых точек(звезд)
int D = 0;
int oX = 0;
int oY = 0;
int ClientWidth;
int ClientHeight;
Star star[StarCount];
PAINTSTRUCT ps;
HDC hdc;
HDC compdc;
HBITMAP buffer;

HWND MainWnd;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void InitProgram()
{
    ClientWidth = GetSystemMetrics(SM_CXSCREEN);
    ClientHeight = GetSystemMetrics(SM_CYSCREEN);

    D = (ClientWidth + ClientHeight) / 2;

    // Вычисляем центр экрана
    oX = ClientWidth / 2;
    oY = ClientHeight / 2;

    // Генерация трехмерных точек

    for (int i = 0; i < StarCount; i++)
    {
        star[i].X = (rand() % ClientWidth) - oX;
        star[i].Y = (rand() % ClientHeight) - oY;
        star[i].Z = (rand() % ClientWidth) - oX;
    }

}

void MoveStars()
{
    int X, Y;
    float k, rPoint;

    for (int i = 0; i < StarCount; i++)
    {
        star[i].Z = star[i].Z - Speed;

        if (star[i].Z <= -D)  star[i].Z = 0;

        if ((star[i].Z + D) == 0) star[i].Z = 0.1;

        k = D / (star[i].Z + D);

        X = oX + floor(star[i].X * k);
        Y = oY + floor(star[i].Y * k);

        rPoint = star[i].Z * 0.005;

        Ellipse(compdc, X - rPoint, Y - rPoint, X + rPoint, Y + rPoint);

    }

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ACROSSUNIVERSES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACROSSUNIVERSES));

    SetTimer(MainWnd, 0, 1, 0);
    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    KillTimer(MainWnd, 0);
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm = 0;

    return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
   InitProgram();
   MainWnd = CreateWindowEx(
       WS_EX_TOPMOST,
       szWindowClass,
       szWindowClass,
       WS_POPUP | WS_VISIBLE,
       0,
       0,
       ClientWidth,
       ClientHeight,
       NULL,
       NULL,
       hInstance,
       NULL);

   if (!MainWnd)
   {
       return FALSE;
   }

   ShowWindow(MainWnd, nCmdShow);
   UpdateWindow(MainWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HPEN hpen;
    HBRUSH hbrush;
    switch (message)
    {

    case WM_CREATE:
    {
        hdc = GetDC(hWnd);
        compdc = CreateCompatibleDC(hdc);
        buffer = CreateCompatibleBitmap(hdc, ClientWidth, ClientHeight);
        SelectObject(compdc, buffer);
        ReleaseDC(hWnd, hdc);
    }	break;

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)  SendMessage(hWnd, WM_CLOSE, 0, 0);
    }	break;

    case WM_TIMER: InvalidateRect(hWnd, 0, FALSE);

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        // Стираем буфер рисования 
        hpen = (HPEN)GetStockObject(BLACK_PEN);
        hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        SelectObject(compdc, hpen);
        SelectObject(compdc, hbrush);
        Rectangle(compdc, 0, 0, ClientWidth, ClientHeight);

        // Рисуем звезды белым цветом 
        hpen = (HPEN)GetStockObject(WHITE_PEN);
        hbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        SelectObject(compdc, hpen);
        SelectObject(compdc, hbrush);
        MoveStars();

        // Скидываем буфер рисования на экран 
        BitBlt(hdc, 0, 0, ClientWidth, ClientHeight, compdc, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
    }	break;

    case WM_DESTROY:
    {
        DeleteObject(compdc);
        DeleteObject(buffer);
        PostQuitMessage(0);
    }	break;

    default: return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return 0;
}


