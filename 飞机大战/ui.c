#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
//作业：Fork 仓库，上传（ppk个人证书、puhygen）
//dif工具对比代码不同
//API函数文档查看

//飞机窗口类：没有边框、没有响应、透明
//拼写：ALT+右箭头
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的句柄，某些API函数会使用到这个变量。
#define TRANS_BK_COLOR RGB(255,255,255)
// 全局变量
RECT rectBoundary;
RECT rect;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LONG CALLBACK FlighterWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CreateFlighterWindows();
BOOL RegisterFighterClass();




BOOL CreateFlighterWindows()
{
	// Create the main window. 
	HWND hwndFlighter;
	hwndFlighter = CreateWindowEx(
		WS_EX_LAYERED|WS_EX_TOPMOST,//1、透明的 2、永运在顶端
		"FlighterClass", // 窗口类名，必须是已经注册了的窗口类
		"",
		WS_POPUP|WS_VISIBLE, // 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,        // 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,        // 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,        // 窗口宽度 default width 
		CW_USEDEFAULT,        // 窗口高度 default height 
		(HWND)NULL,         // 父窗口句柄 no owner window 
		(HMENU)NULL,        // 窗口菜单的句柄 use class menu 
		hinst,           // 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);      // 指向附加数据的指针 no window-creation data 

	if (!hwndFlighter)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return FALSE;
	}


	if (SetLayeredWindowAttributes(hwndFlighter, TRANS_BK_COLOR, 255, LWA_COLORKEY | LWA_ALPHA))
	{
		GetLastError();
	}
	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwndFlighter, SW_SHOW);//SW_HIDE(隐藏)

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwndFlighter);
	return TRUE;
}


BOOL  RegisterFighterClass()//注册窗口
{

		WNDCLASS wc;
		// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
		HWND hwnd;

		MSG msg;
		int fGotMessage;

		//hinst = hinstance;

		// Fill in the window class structure with parameters 
		// that describe the main window. 

		wc.style = CS_HREDRAW | CS_VREDRAW;  // 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
		wc.lpfnWndProc = FlighterWndProc;  // 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
		wc.cbClsExtra = 0;  // no extra class memory 
		wc.cbWndExtra = 0;  // no extra window memory 
		wc.hInstance = hinst;  // handle to instance 
		wc.hIcon = NULL;   // 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
		wc.hCursor = LoadCursor(NULL, // Cursor是鼠标光标，这里是设定了鼠标光标的样式。
			IDC_HAND);   // 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
		wc.hbrBackground = (HBRUSH)GetStockObject( // GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
			WHITE_BRUSH);  // 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
		wc.lpszMenuName = NULL;  // 窗口的菜单的资源名。
		wc.lpszClassName = "FlighterClass"; // 给窗口类起一个名字，在创建窗口时需要这个名字。

										 // Register the window class. 

		if (!RegisterClass(&wc))
		{
			// 窗口注册失败，消息框提示，并退出。
			MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
			return FALSE;
		}
		// 窗口注册成功，继续运行。

		return TRUE;

}



HBITMAP hmpFlighter;//飞机位图句柄

LONG CALLBACK FlighterWndProc(
	HWND hwnd, //窗口句柄
	UINT msg, // 消息

	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	HDC hdc, hdcMem;//定义两个，作为内存缓存
	PAINTSTRUCT ps;
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE://位图加载
		MoveWindow(hwnd, 100, 100, 100, 100, TRUE);
		hmpFlighter = LoadImage(NULL, "飞机2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//作业：读LoadImage文档																	 /**********************************************************************************************************/
																					 /*LoadImage:*/

		if (hmpFlighter == NULL)
		{
			MessageBox(hwnd, "位图未找到！", "ERROE!", MB_OK | MB_ICONERROR);//作业：文档
			ExitProcess(0);

		}
		//ReSizeGameWnd(hwnd);
		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);//作业，通知操作系统画图，返回DC对象【指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到一个PAINTSTRUCT结构中】
		hdcMem = CreateCompatibleDC(hdc);//内存DC,作业；文档
		SelectObject(hdcMem, hmpFlighter);//将位图选择到hdcMem中

		GetClientRect(hwnd, &rect);
		//BitBlt(hdc, 0, 0,1024,1024, hdcMem, 0, 0, SRCCOPY);//将内存中的内容复制到hdc中,图片不能拉伸
		StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, 175, 160, SRCCOPY);//图片能够拉伸


		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);

		//GamePaint(hwnd);
		break;

	case WM_KEYDOWN:

		//OnKeyDown(wParam);
		//GamePaint(hwnd);
		break;

	case WM_TIMER:

		//OnTimer(hwnd);
		//GamePaint(hwnd);
		break;

	case WM_DESTROY:
		//ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}


int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{

	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wc.style = CS_HREDRAW | CS_VREDRAW;  // 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.lpfnWndProc = MainWndProc;  // 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.cbClsExtra = 0;  // no extra class memory 
	wc.cbWndExtra = 0;  // no extra window memory 
	wc.hInstance = hinstance;  // handle to instance 
	wc.hIcon = LoadIcon(NULL,  // hIcon成员用来指定窗口的图标
		IDI_APPLICATION);   // 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hCursor = LoadCursor(NULL, // Cursor是鼠标光标，这里是设定了鼠标光标的样式。
		IDC_HAND);   // 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hbrBackground = (HBRUSH)GetStockObject( // GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
		WHITE_BRUSH);  // 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.lpszMenuName = "MainMenu";  // 窗口的菜单的资源名。
	wc.lpszClassName = "MainWClass"; // 给窗口类起一个名字，在创建窗口时需要这个名字。

									 // Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",        // 窗口类名，必须是已经注册了的窗口类
		"飞机大战",    // title-bar string 
		WS_OVERLAPPEDWINDOW, // 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,        // 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,        // 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,        // 窗口宽度 default width 
		CW_USEDEFAULT,        // 窗口高度 default height 
		(HWND)NULL,         // 父窗口句柄 no owner window 
		(HMENU)NULL,        // 窗口菜单的句柄 use class menu 
		hinstance,           // 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);      // 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);



	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

HBITMAP hmpSky;


LONG CALLBACK MainWndProc(
	HWND hwnd, //窗口句柄
	UINT msg, // 消息
	
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	HDC hdc, hdcMem;//定义两个，作为内存缓存
	PAINTSTRUCT ps;
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE://位图加载
		hmpSky = LoadImage(NULL, "sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//作业：读LoadImage文档

/**********************************************************************************************************/
/*LoadImage:*/

		if (hmpSky == NULL)
		{
			MessageBox(hwnd, "位图未找到！", "ERROE!", MB_OK | MB_ICONERROR);//作业：文档
			ExitProcess(0);

		}
		//ReSizeGameWnd(hwnd);

		if (RegisterFighterClass())
		{
			CreateFlighterWindows();
		}
		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);//作业，通知操作系统画图，返回DC对象【指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到一个PAINTSTRUCT结构中】
		hdcMem = CreateCompatibleDC(hdc);//内存DC,作业；文档
		SelectObject(hdcMem, hmpSky);//将位图选择到hdcMem中

		GetClientRect(hwnd, &rect);
		//BitBlt(hdc, 0, 0,1024,1024, hdcMem, 0, 0, SRCCOPY);//将内存中的内容复制到hdc中,图片不能拉伸
		StretchBlt(hdc,rect.left, rect.top,rect.right-rect.left, rect.bottom-rect.top, hdcMem, 0,0,600,450,SRCCOPY);//图片能够拉伸


		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
		
		//GamePaint(hwnd);
		break;

	case WM_KEYDOWN:

		//OnKeyDown(wParam);
		//GamePaint(hwnd);
		break;

	case WM_TIMER:

		//OnTimer(hwnd);
		//GamePaint(hwnd);
		break;

	case WM_DESTROY:
		//ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}
