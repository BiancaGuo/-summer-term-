#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
//��ҵ��Fork �ֿ⣬�ϴ���ppk����֤�顢puhygen��
//dif���߶Աȴ��벻ͬ
//API�����ĵ��鿴

//�ɻ������ࣺû�б߿�û����Ӧ��͸��
//ƴд��ALT+�Ҽ�ͷ
HINSTANCE hinst; /// HINSTANCE��������ʾ��������ʵ���ľ����ĳЩAPI������ʹ�õ����������
#define TRANS_BK_COLOR RGB(255,255,255)
// ȫ�ֱ���
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
		WS_EX_LAYERED|WS_EX_TOPMOST,//1��͸���� 2�������ڶ���
		"FlighterClass", // �����������������Ѿ�ע���˵Ĵ�����
		"",
		WS_POPUP|WS_VISIBLE, // ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,        // ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,        // ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,        // ���ڿ�� default width 
		CW_USEDEFAULT,        // ���ڸ߶� default height 
		(HWND)NULL,         // �����ھ�� no owner window 
		(HMENU)NULL,        // ���ڲ˵��ľ�� use class menu 
		hinst,           // Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);      // ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwndFlighter)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return FALSE;
	}


	if (SetLayeredWindowAttributes(hwndFlighter, TRANS_BK_COLOR, 255, LWA_COLORKEY | LWA_ALPHA))
	{
		GetLastError();
	}
	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwndFlighter, SW_SHOW);//SW_HIDE(����)

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwndFlighter);
	return TRUE;
}


BOOL  RegisterFighterClass()//ע�ᴰ��
{

		WNDCLASS wc;
		// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
		HWND hwnd;

		MSG msg;
		int fGotMessage;

		//hinst = hinstance;

		// Fill in the window class structure with parameters 
		// that describe the main window. 

		wc.style = CS_HREDRAW | CS_VREDRAW;  // ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
		wc.lpfnWndProc = FlighterWndProc;  // һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
		wc.cbClsExtra = 0;  // no extra class memory 
		wc.cbWndExtra = 0;  // no extra window memory 
		wc.hInstance = hinst;  // handle to instance 
		wc.hIcon = NULL;   // ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
		wc.hCursor = LoadCursor(NULL, // Cursor������꣬�������趨����������ʽ��
			IDC_HAND);   // ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
		wc.hbrBackground = (HBRUSH)GetStockObject( // GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
			WHITE_BRUSH);  // ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
		wc.lpszMenuName = NULL;  // ���ڵĲ˵�����Դ����
		wc.lpszClassName = "FlighterClass"; // ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�

										 // Register the window class. 

		if (!RegisterClass(&wc))
		{
			// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
			MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
			return FALSE;
		}
		// ����ע��ɹ����������С�

		return TRUE;

}



HBITMAP hmpFlighter;//�ɻ�λͼ���

LONG CALLBACK FlighterWndProc(
	HWND hwnd, //���ھ��
	UINT msg, // ��Ϣ

	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	HDC hdc, hdcMem;//������������Ϊ�ڴ滺��
	PAINTSTRUCT ps;
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
	case WM_CREATE://λͼ����
		MoveWindow(hwnd, 100, 100, 100, 100, TRUE);
		hmpFlighter = LoadImage(NULL, "�ɻ�2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//��ҵ����LoadImage�ĵ�																	 /**********************************************************************************************************/
																					 /*LoadImage:*/

		if (hmpFlighter == NULL)
		{
			MessageBox(hwnd, "λͼδ�ҵ���", "ERROE!", MB_OK | MB_ICONERROR);//��ҵ���ĵ�
			ExitProcess(0);

		}
		//ReSizeGameWnd(hwnd);
		break;

		// ��ϵͳ��Ϊ�����ϵ�GDI����Ӧ�ñ��ػ�ʱ�����򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ��ȻӦ�ó���Ҳ����ͨ������ UpateWindow�������򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ����ʹ��GDI�ڴ����ϻ���ͼ�εĳ��� �����롱 д�����
		// ���������WM_PAINT��Ϣ�Ĵ�������л���GDIͼ�Σ���ô�ڴ���ˢ��ʱ�ͻᱻ�±�Ĩ���͸���
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);//��ҵ��֪ͨ����ϵͳ��ͼ������DC����ָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽һ��PAINTSTRUCT�ṹ�С�
		hdcMem = CreateCompatibleDC(hdc);//�ڴ�DC,��ҵ���ĵ�
		SelectObject(hdcMem, hmpFlighter);//��λͼѡ��hdcMem��

		GetClientRect(hwnd, &rect);
		//BitBlt(hdc, 0, 0,1024,1024, hdcMem, 0, 0, SRCCOPY);//���ڴ��е����ݸ��Ƶ�hdc��,ͼƬ��������
		StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, 175, 160, SRCCOPY);//ͼƬ�ܹ�����


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
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{

	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wc.style = CS_HREDRAW | CS_VREDRAW;  // ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.lpfnWndProc = MainWndProc;  // һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.cbClsExtra = 0;  // no extra class memory 
	wc.cbWndExtra = 0;  // no extra window memory 
	wc.hInstance = hinstance;  // handle to instance 
	wc.hIcon = LoadIcon(NULL,  // hIcon��Ա����ָ�����ڵ�ͼ��
		IDI_APPLICATION);   // ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hCursor = LoadCursor(NULL, // Cursor������꣬�������趨����������ʽ��
		IDC_HAND);   // ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hbrBackground = (HBRUSH)GetStockObject( // GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
		WHITE_BRUSH);  // ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.lpszMenuName = "MainMenu";  // ���ڵĲ˵�����Դ����
	wc.lpszClassName = "MainWClass"; // ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�

									 // Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",        // �����������������Ѿ�ע���˵Ĵ�����
		"�ɻ���ս",    // title-bar string 
		WS_OVERLAPPEDWINDOW, // ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,        // ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,        // ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,        // ���ڿ�� default width 
		CW_USEDEFAULT,        // ���ڸ߶� default height 
		(HWND)NULL,         // �����ھ�� no owner window 
		(HMENU)NULL,        // ���ڲ˵��ľ�� use class menu 
		hinstance,           // Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);      // ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
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
	HWND hwnd, //���ھ��
	UINT msg, // ��Ϣ
	
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	HDC hdc, hdcMem;//������������Ϊ�ڴ滺��
	PAINTSTRUCT ps;
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
	case WM_CREATE://λͼ����
		hmpSky = LoadImage(NULL, "sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//��ҵ����LoadImage�ĵ�

/**********************************************************************************************************/
/*LoadImage:*/

		if (hmpSky == NULL)
		{
			MessageBox(hwnd, "λͼδ�ҵ���", "ERROE!", MB_OK | MB_ICONERROR);//��ҵ���ĵ�
			ExitProcess(0);

		}
		//ReSizeGameWnd(hwnd);

		if (RegisterFighterClass())
		{
			CreateFlighterWindows();
		}
		break;

		// ��ϵͳ��Ϊ�����ϵ�GDI����Ӧ�ñ��ػ�ʱ�����򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ��ȻӦ�ó���Ҳ����ͨ������ UpateWindow�������򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ����ʹ��GDI�ڴ����ϻ���ͼ�εĳ��� �����롱 д�����
		// ���������WM_PAINT��Ϣ�Ĵ�������л���GDIͼ�Σ���ô�ڴ���ˢ��ʱ�ͻᱻ�±�Ĩ���͸���
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);//��ҵ��֪ͨ����ϵͳ��ͼ������DC����ָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽һ��PAINTSTRUCT�ṹ�С�
		hdcMem = CreateCompatibleDC(hdc);//�ڴ�DC,��ҵ���ĵ�
		SelectObject(hdcMem, hmpSky);//��λͼѡ��hdcMem��

		GetClientRect(hwnd, &rect);
		//BitBlt(hdc, 0, 0,1024,1024, hdcMem, 0, 0, SRCCOPY);//���ڴ��е����ݸ��Ƶ�hdc��,ͼƬ��������
		StretchBlt(hdc,rect.left, rect.top,rect.right-rect.left, rect.bottom-rect.top, hdcMem, 0,0,600,450,SRCCOPY);//ͼƬ�ܹ�����


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
