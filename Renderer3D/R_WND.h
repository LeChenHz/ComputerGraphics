#pragma once

//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================

#include <windows.h>
#include <tchar.h>


int screen_w, //屏幕宽
	screen_h, //屏幕高
	screen_exit = 0; //退出程序
int screen_keys[512];	// 当前键盘按下状态

static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP

unsigned char *screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

int screen_init(int w, int h, const TCHAR *title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(void);							// 显示 FrameBuffer

// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

// 初始化窗口并设置标题
int screen_init(int w, int h, const TCHAR *title) {
	//https://baike.baidu.com/item/WNDCLASS/6488732?fr=aladdin
	//储存某一类窗口的信息
	WNDCLASS wc = {
		CS_BYTEALIGNCLIENT, // 窗口类型
		(WNDPROC)screen_events, //窗口处理函数 lpfnWndProc
		0, //窗口扩展 cbClsExtra
		0, //窗口实例扩展 cbWndExtra
		0, //实例句柄 hInstance
		NULL,//窗口的最小化图标 hIcon
		NULL, //窗口鼠标光标 hCursor
		NULL, //窗口背景色 hbrBackground
		NULL, //窗口菜单 lpszMenuName
		_T("SCREEN3.1415926") //窗口类名 lpszClassName 
	};
	// https://docs.microsoft.com/zh-cn/windows/desktop/api/wingdi/ns-wingdi-tagbitmapinfo
	// 设备无关位图文件
	BITMAPINFO bi = {
		{ sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4, 0, 0, 0, 0 }
		//https://docs.microsoft.com/zh-cn/windows/desktop/api/wingdi/ns-wingdi-bitmapinfoheader
	};
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	// https://baike.baidu.com/item/HDC/16710
	/* HDC设备上下文是一种包含有关某个设备（如显示器或打印机）的绘制属性信息的 Windows 数据结构。
	所有绘制调用都通过设备上下文对象进行，这些对象封装了用于绘制线条、形状和文本的 Windows API */
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	//https://baike.baidu.com/item/CreateWindow
	screen_handle = CreateWindow(
		_T("SCREEN3.1415926"), //LPCTSTR lpClassName 窗口的类名
		title, //LPCTSTR lpWindowName 指向一个指定窗口名的空结束的字符串指针
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //DWORD dwStyle 指定创建窗口的风格
		0, //int x 指定窗口的初始水平位置
		0, //int y 指定窗口的初始垂直位置
		0, //int nWidth 以设备单元指明窗口的宽度
		0, //int nHeight 以设备单元指明窗口的高度
		NULL, //HWND hWndParent 指向被创建窗口的父窗口或所有者窗口的句柄
		NULL, //HMENU hMenu 菜单句柄
		wc.hInstance, //HANDLE hlnstance 与窗口相关联的模块实例的句柄
		NULL //LPVOID lpParam 指向一个值的指针，该值传递给窗口WM_CREATE消息
	);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * 4);

	return 0;
}

int screen_close(void) {
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//分发消息
void screen_dispatch(void) 
{
	MSG msg;
	while (1) 
	{

		// 该函数为一个消息检查线程消息队列，并将该消息（如果存在）放于指定的结构
		if (!PeekMessage(&msg, //接收消息信息的MSG结构指针
						NULL, //其消息被检查的窗口句柄
						0, //指定被检查的消息范围里的第一个消息
						0, //指定被检查的消息范围里的最后一个消息
						PM_NOREMOVE //确定消息如何被处理。 PM_NOREMOVE：PeekMessage处理后，消息不从队列里除掉。
						)
			) 
			break;

		// 从调用线程的消息队列里取得一个消息并将其放于指定的结构
		if (!GetMessage(&msg, //指向MSG结构的指针，该结构从线程的消息队列里接收消息信息
						NULL, //取得其消息的窗口的句柄
						0, //指定被检索的最小消息值的整数
						0 //指定被检索的最大消息值的整数
						)
			)
			break;
		// 该函数分发一个消息给窗口程序
		DispatchMessage(&msg);
	}
}


void screen_update(void) {
	HDC hDC = GetDC(screen_handle);
	BitBlt(
		hDC, // hDestDC：指向目标设备环境的句柄
		0, // x：指定目标矩形区域左上角的X轴逻辑坐标
		0, // y：指定目标矩形区域左上角的Y轴逻辑坐标
		screen_w, // nWidth：指定源在目标矩形区域的逻辑宽度
		screen_h, // nHeight：指定源在目标矩形区域的逻辑高度
		screen_dc, // hSrcDC：指向源设备环境的句柄
		0, // xSrc：指定源矩形区域左上角的X轴逻辑坐标
		0, // ySrc：指定源矩形区域左上角的Y轴逻辑坐标
		SRCCOPY // 指定光栅操作代码。这些代码将定义源矩形区域的颜色数据，如何与目标矩形区域的颜色数据组合以完成最后的颜色。
				// SRCCOPY：将源矩形区域直接拷贝到目标矩形区域
	);
	ReleaseDC(screen_handle, hDC); //ReleaseDC函数释放设备上下文环境（DC）供其他应用程序使用
	screen_dispatch();
}
