﻿// pacman.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "pacman.h"

#define MAX_LOADSTRING 100
#define PI (3.1415926f) //定义圆周率的值

using namespace std;
//修改窗口的大小
#define WLENTH 700 //高
#define WHIGHT 740 //宽
#define STAGE_COUNT 3 //一共3关
static HWND g_hwnd; //游戏窗口句柄


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));

    MSG msg;

    // 主消息循环:  本来是有消息才返回，无消息时阻塞，现在改成可以用作游戏循环使用
    /*
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    */

    bool bRunning = true;
    while (bRunning) {
    //获取消息
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    
        //画玩家
        {
            static DWORD dwTime = GetTickCount();
            //当距离上次绘图的时间大于50毫秒，才进行本次绘制
            if (GetTickCount() - dwTime >= 50) {
                dwTime = GetTickCount();
            }
            else {
                continue;
            }
            //模拟当前的帧，本对象一共5帧，每一帧画不同的图形
            static int iFrame = 0;
            ++iFrame;
            if (iFrame >= 5) {
                iFrame = 0;
            }

            //代表对象的中心位置;
            int x = 300, y = 300;
            //对象的半径
            int r = 100;

            //dc对象句柄
            HDC hdc = ::GetDC(g_hwnd);
            std::shared_ptr<HDC__>dc(::GetDC(g_hwnd), [](HDC hdc)) {
                ::ReleaseDc(g_hwnd, hdc);
            });
            //获取窗口客户区大小
            RECT rc;
            GetClientRect(g_hwnd, &rc);

            //创建画刷
            std::shared_ptr<HBRUSH__>br(:CreateSolidBrush(RGB(255, 255, 255)), [](HBRUSH hbr) {
                ::DeleteObject(hbr)；
                });

            //画背景(清楚上一帧所画内容)
            FillRect(dc.get(), &rc, br.get());

            switch (iFrame) {
            case 0: {
                Ellipse(dc.get(), x - r, y - r, x + r, y + r);//画一个圆
                MoveToEx(dc.get(), x - r, y, NULL);//画一条横线
                LineTo(dc.get(), x, y);
                break;
            }
            case 1: {
            //画嘴(两条线与纵轴偏离PI\4)
                int x0, y0;
                int x1, y1;
                x0 = x - static_cast<int>(r* sin(PI * 0.75f));
                y0 = y + static_cast<int>(r* cos(PI * 0.75f));

                x1 = x + static_cast<int>(r* sin(PI * 1.25f));
                y1 = y - static_cast<int>(r* cos(PI * 1.25f));

                SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
                SetPixel(dc.get(), x1, y1, RGB(255, 0, 0));
                SetPixel(dc.get(), x, y, RGB(0, 0, 0));
                Arc(dc.get(), x - r, y - r, x + r, y + r, x1, y1, x0, y0); //画一个半圆+一条竖线

                MoveToEx(dc.get(), x0, y0, NULL);
                LineTo(dc.get(), x, y);

                MoveToEx(dc.get(), x1, y1, NULL);
                LineTo(dc.get(), x, y);
                break;
            }
            case 2: {
                Arc(dc.get(), x - r, y - r, x + r, y + r, x, y + r, x, y - r);//画一个半圆+一个竖线

                //画竖线
                MoveToEx(dc.get(), x, y - r, NULL);//从圆弧上面的点开始
                LineTo(dc.get(), x, y + r);//到圆弧下面的点结束
                break;
            }

            }

        }

    }





    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   /*
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
      */
   //限制窗口的大小，并且制定窗口初始位置在屏幕的左上角
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, WLENTH, WHIGHT, nullptr, nullptr, hInstance, nullptr);
   
   if (!hWnd)
   {
      return FALSE;
   }
   g_hwnd = hWnd; //保存游戏窗口句柄给全局变量

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
