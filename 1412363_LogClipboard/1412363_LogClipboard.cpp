// 1412363_LogClipboard.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412363_LogClipboard.h"
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdlib>
#include <locale>
#include <codecvt>
#include <vector>
#include <atlimage.h> 
#include <Gdiplusimaging.h>
using namespace std;
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
void WriteToFile(vector<wstring>textarray);
void _doInstallHook(HWND hWnd);

void getClipboardData(HWND hwnd);
char *textFilename = "textdata.txt";
TCHAR imageFolderName[255];
int numberOfImage = 0;
vector<wstring>textArray;
HHOOK hHook = NULL;
HINSTANCE hinstLib;
HWND g_hWnd;
bool isActive = false;

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
	
	wofstream f(textFilename, ios::out);


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1412363_LOGCLIPBOARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412363_LOGCLIPBOARD));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412363_LOGCLIPBOARD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1412363_LOGCLIPBOARD);
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
	g_hWnd = hWnd;
	switch (message)
	{
	case WM_CREATE:
	{
					  g_hWnd = hWnd;
					  AddClipboardFormatListener(hWnd);
					  
	}
		break;
	case WM_CLIPBOARDUPDATE:
		if (isActive)
		getClipboardData(g_hWnd);
		break;
	case WM_KEYDOWN:
		if (KeyPressed(VK_CONTROL) && KeyPressed(VK_MENU) && KeyPressed('C') && KeyPressed(VK_SHIFT)){
			isActive = true;
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			WriteToFile(textArray);
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
		WriteToFile(textArray);
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

void WriteToFile(vector<wstring>textarray){
	const locale utf8_locale = locale(locale(), new codecvt_utf8<wchar_t>());
	wofstream f(textFilename, ios::out);
	f.imbue(utf8_locale);

	for (int i = 0; i < textarray.size(); i++){
		f << textarray[i] << endl;
	}
	f.close();
}

void getClipboardData(HWND hWnd){

	if (OpenClipboard(hWnd))
	{
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
		{
			HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
			LPCWSTR buffer = (LPCWSTR)GlobalLock(hClipboardData);
			textArray.push_back((WCHAR*)(buffer));
			MessageBox(0, (WCHAR*)(buffer), L"Data", 0);
			GlobalUnlock(hClipboardData);
		}
		else if (IsClipboardFormatAvailable(CF_BITMAP))
		{
			numberOfImage++;
			wsprintf(imageFolderName, L"%d.bmp", numberOfImage);
			HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
			PBITMAPINFO pbi;

			BITMAP bitmap;
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);

			HDC hdc = GetDC(hWnd); // Tạo DC ảo chứa bitmap
			HDC memDC = CreateCompatibleDC(hdc);
			SelectObject(memDC, hBitmap);
			CImage hinh;
			hinh.Attach(hBitmap);
			hinh.Save(imageFolderName, Gdiplus::ImageFormatBMP);
			//// Sử dụng đoạn code comment bên dưới để zoom ảnh
			//RECT rect;
			//GetClientRect(hWnd, &rect);
			//int height = rect.bottom - rect.top;
			//int width = height * 16 / 9; // Duy trì tỉ lệ 16:9
			//StretchBlt(hdc, 0, 0, width, height,
			//	memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memDC, 0, 0, SRCCOPY);
			DeleteDC(memDC);
			ReleaseDC(hWnd, hdc);
		}

		CloseClipboard();
	}
}

