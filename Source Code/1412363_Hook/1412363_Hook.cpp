// 1412363_Hook.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412363_Hook.h"
#include <Windowsx.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void _doInstallHook(HWND hWnd);
void _doRemoveHook(HWND hWnd);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


HHOOK hHook = NULL;
HINSTANCE hinstLib;
KBDLLHOOKSTRUCT *key;
HWND staticResult, edtTime;
TCHAR result[256];
int timestart = 0, timeend = 0;
int count = 0;
HHOOK hhkLowLevelKybd;
int isButtonClicked = 0;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1412363_HOOK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412363_HOOK));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412363_HOOK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1412363_HOOK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

bool KeyPressed(int a)
{
	if (GetAsyncKeyState(a))
	{
		return true;
	}
	else
	{
		return false;
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
								DWORD CtrlID = GetDlgCtrlID((HWND)lParam); //Window Control ID
							  if (CtrlID == IDC_STATIC1) //If desired control
							  {
								  

								  HDC hdcStatic = (HDC)wParam;
								  SetTextColor(hdcStatic, RGB(0, 0, 0));
								  SetBkColor(hdcStatic, RGB(102, 178, 255));
								  return (INT_PTR)CreateSolidBrush(RGB(102, 178, 255));
							  }
							  if (CtrlID == IDC_STATIC2) //If desired control
							  {


								  HDC hdcStatic = (HDC)wParam;
								  SetTextColor(hdcStatic, RGB(0, 0, 0));
								  SetBkColor(hdcStatic, RGB(255, 204, 204));
								  return (INT_PTR)CreateSolidBrush(RGB(255, 204, 204));
							  }
	}

		break;
	case WM_CTLCOLORBTN:
	{
						   HDC hdcStatic = (HDC)wParam;
						   SetTextColor(hdcStatic, RGB(0, 0, 0));
						   SetBkColor(hdcStatic, RGB(102, 178, 255));
						   return (INT_PTR)CreateSolidBrush(RGB(102, 178, 255));
	}
	case WM_KEYDOWN:
		if (KeyPressed(VK_CONTROL) && KeyPressed(VK_MENU) && KeyPressed(VK_ESCAPE) && KeyPressed(VK_SHIFT)){
			_doInstallHook(hWnd);
		}

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTONOK:
		{
							 isButtonClicked = 1;
							 EnableWindow(edtTime, FALSE);
		}
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

char getSpecialChar(char c)
{
	switch (c)
	{
	case '1':
		return  '!';
		break;
	case '2':
		return '@';
		break;
	case '3':
		return '#';
		break;
	case '4':
		return '$';
		break;
	case '5':
		return '%';
		break;
	case '6':
		return '^';
		break;
	case '7':
		return '&';
		break;
	case '8':
		return '*';
		break;
	case '9':
		return '(';
		break;
	case '0':
		return ')';
		break;
	default:
		break;
	}

}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	count++;
	
	if (nCode == HC_ACTION && GetWindowTextLength(edtTime) != 0){
		
		int t;
		WCHAR buff[100];
		GetWindowText(edtTime, buff, 100);
		t = _wtoi(buff);
		switch (wParam)
		{
		
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
							if (count == 1 && isButtonClicked ==1)
							{
								PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
								timeend = p->time;
								if (timeend - timestart > 1000 * t){
									TCHAR buff[255];
									wsprintf(buff, L"\n", p->vkCode);
									wcscat_s(result, buff);

								}
								timestart = timeend;
								TCHAR buff[255];
								
								switch (p->vkCode)
								{
								case VK_CAPITAL:
									wsprintf(buff, L"<CAPS LOCK>");
									break;
								case VK_LSHIFT:
									wsprintf(buff, L"<LSHIFT>");
									break;
								case VK_RSHIFT:
									wsprintf(buff, L"<RSHIFT>");
									break;
								case VK_LCONTROL:
									wsprintf(buff, L"<LCONTROL>");
									break;
								case VK_RCONTROL:
									wsprintf(buff, L"<RCONTROL>");
									break;
								case VK_LMENU:
									wsprintf(buff, L"<LALT>");
									break;
								case VK_RMENU:
									wsprintf(buff, L"<RALT>");
									break;									
								case VK_SHIFT:
									wsprintf(buff, L"<SHIFT>");
									break;
								case VK_INSERT:
									wsprintf(buff, L"<INSERT>");
									break;
								case VK_HOME:
									wsprintf(buff, L"<HOME>");
									break;
								case VK_END:
									wsprintf(buff, L"<END>");
									break;
								case VK_PRINT:
									wsprintf(buff, L"<PRINT>");
									break;
								case VK_DELETE:
									wsprintf(buff, L"<DELETE>");
									break;
								case VK_BACK:
									wsprintf(buff, L"<BACK>");
									break;
								case VK_TAB:
									wsprintf(buff, L"<TAB>");
									break;
								case VK_SNAPSHOT:
									wsprintf(buff, L"<SNAPSHOT>");
									break;
								case VK_LWIN:
									wsprintf(buff, L"<LWIN>");
									break;
								case VK_RWIN:
									wsprintf(buff, L"<RWIN>");
									break;
								case VK_LEFT:
									wsprintf(buff, L"<LEFT>");
									break;
								case VK_RIGHT:
									wsprintf(buff, L"<RIGHT>");
									break;
								case VK_UP:
									wsprintf(buff, L"<UP>");
									break;
								case VK_DOWN:
									wsprintf(buff, L"<DOWN>");
									break;

								default:
								{
										   char c = char(p->vkCode);
										   if (GetAsyncKeyState(VK_SHIFT) < 0)
										   {
											   if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
											   {
												   //Both shift and caps are on
												   wsprintf(buff, L"%c", char(tolower(c)));
											   }
											   else
											   {
												   //Only shift is on
												   if (c >= '0' && c <= '9')
												   {
													   //if c is digit
													   getSpecialChar(c);
												   }
												   else
												   {
														   wsprintf(buff, L"%c",char( p->vkCode));
												   }
											   }
										   }
										   else
										   {
											   if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
											   {
												   //Only caps is on
												   wsprintf(buff, L"%c", char(p->vkCode));
											   }
											   else
											   {
												   //Both caps and shift are off
												   wsprintf(buff, L"%c", char(tolower(c)));

												 
											   }
										   }										  

								}
									break;
								}

								wcscat_s(result, buff);
								SetWindowTextW(staticResult, result);
							}
							else{
								count = 0;
							}
						
		}

			break;

		}
	}


	return CallNextHookEx(hhkLowLevelKybd, nCode, wParam, lParam);
}



void _doInstallHook(HWND hWnd)
{
	if (hHook != NULL) return;
	hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, hinstLib, 0);
	if (hhkLowLevelKybd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		HFONT hFont = CreateFont(19, 7, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"italic");
		staticResult = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE , width / 20, height  / 5, width * 9 / 10, height * 4 / 5, hWnd, (HMENU)IDC_STATIC2, NULL, NULL);
		SendMessage(staticResult, WM_SETFONT, (WPARAM)hFont, TRUE);
		HWND stt1 = CreateWindowEx(0, L"STATIC", L"Type the number of time you want to create new line automatically: (s)", WS_CHILD | WS_VISIBLE, width / 20, height / 20, width *13/ 20, height / 10, hWnd, (HMENU)IDC_STATIC1, NULL, NULL);
		SendMessage(stt1, WM_SETFONT, (WPARAM)hFont, TRUE);

		edtTime = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE |WS_BORDER | ES_NUMBER, width*11 / 20, height / 20, width * 9 / 40, height / 10, hWnd, NULL, NULL, NULL);
		SendMessage(edtTime, WM_SETFONT, (WPARAM)hFont, TRUE);

		HWND stt2 = CreateWindowEx(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | WS_BORDER, width * 25 / 32 , height / 20, width / 6, height / 10, hWnd, (HMENU)IDC_BUTTONOK, NULL, NULL);
		SendMessage(stt2, WM_SETFONT, (WPARAM)hFont, TRUE);

		MessageBox(hWnd, L"Setup hook successfully ", L"Result", MB_OK);
	}

}

void _doRemoveHook(HWND hWnd)
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	MessageBox(hWnd, L"Remove hook successfully", L"Result", MB_OK);
}