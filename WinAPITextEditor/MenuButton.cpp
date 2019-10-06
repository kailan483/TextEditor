#include "MenuButton.h"

////Вот такую структурку надо объявить
//typedef struct
//{
//	BITMAPINFOHEADER bmiHeader;
//	RGBQUAD bmiColors[259];
//} DIBINFO;
//
//BOOL CreateBMPFile(LPTSTR pszFile, HWND hWndScreen, DWORD dwComp, WORD wBitCount)
//{
//	HANDLE hf;
//	BITMAPFILEHEADER hdr;
//	LPBYTE lpBits;
//	DWORD dwTmp;
//
//	BITMAP ddbinfo = { 0 };
//	DIBINFO bInfo = { 0 };
//
//
//	RECT rc = { 0 };
//
//	if (!GetWindowRect(hWndScreen, &rc))             //если проблемы с окном -выход
//		return FALSE;
//
//
//	//Снимок экрана в диалоге
//	HDC hDC = GetWindowDC(hWndScreen);
//	HBITMAP hBmp = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
//
//	if (!hBmp)
//	{
//		ReleaseDC(hWndScreen, hDC);
//		return FALSE;
//	}
//
//	//Запись рисунка в память
//	HDC  hMemDC = CreateCompatibleDC(hDC);
//	HGDIOBJ hOld = SelectObject(hMemDC, hBmp);
//
//	BitBlt(hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hDC, 0, 0, SRCCOPY);
//
//	SelectObject(hMemDC, hOld);
//	DeleteDC(hMemDC);
//
//
//	//Получение данных по изображению
//	if (!GetObject(hBmp, sizeof(BITMAP), &ddbinfo))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		return FALSE;
//	}
//
//
//	bInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bInfo.bmiHeader.biWidth = ddbinfo.bmWidth;
//	bInfo.bmiHeader.biHeight = ddbinfo.bmHeight;
//
//	bInfo.bmiHeader.biPlanes = 1;
//	bInfo.bmiHeader.biBitCount = wBitCount;
//	bInfo.bmiHeader.biCompression = dwComp;
//
//	//Получение размера для сжатого рисунка из исходного
//	if (!GetDIBits(hDC, hBmp, 0, ddbinfo.bmHeight, NULL, (BITMAPINFO*)& bInfo, DIB_RGB_COLORS))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		return FALSE;
//	}
//	//Размер заголовка растра
//	DWORD dwInfSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 259;
//	//Весь растр
//	DWORD dwTotalSize = dwInfSize + bInfo.bmiHeader.biSizeImage;
//
//	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, dwTotalSize);
//
//	if (!lpBits)
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		return FALSE;
//	}
//
//	memset(lpBits, 0, dwTotalSize);
//	memcpy(lpBits, &bInfo, dwInfSize);
//
//	//Преобразование DDB в DIB
//	if (!GetDIBits(hDC, hBmp, 0, ddbinfo.bmHeight, lpBits + dwInfSize, (BITMAPINFO*)lpBits, DIB_RGB_COLORS))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		GlobalFree((HGLOBAL)lpBits);
//
//		return FALSE;
//	}
//
//
//	//Создание файла для рисунка
//	hf = CreateFile(pszFile,
//		GENERIC_READ | GENERIC_WRITE,
//		(DWORD)0,
//		NULL,
//		CREATE_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		(HANDLE)NULL);
//
//	if (hf == INVALID_HANDLE_VALUE)
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		GlobalFree((HGLOBAL)lpBits);
//		return FALSE;
//	}
//
//
//
//	//Структура заголовка файла
//	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
//	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + dwTotalSize);
//	hdr.bfReserved1 = 0;
//	hdr.bfReserved2 = 0;
//	hdr.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + dwInfSize);
//
//	//Запись заголовка
//	if (!WriteFile(hf, (LPVOID)& hdr, sizeof(BITMAPFILEHEADER),
//		(LPDWORD)& dwTmp, NULL))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		GlobalFree((HGLOBAL)lpBits);
//		CloseHandle(hf);
//		return FALSE;
//	}
//	//Запись описания растра
//	if (!WriteFile(hf, (BITMAPINFO*)& bInfo, dwInfSize, (LPDWORD)& dwTmp, NULL))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		GlobalFree((HGLOBAL)lpBits);
//		CloseHandle(hf);
//		return FALSE;
//	}
//
//	//Запись массива битов
//	if (!WriteFile(hf, (BYTE*)lpBits + dwInfSize, dwTotalSize - dwInfSize, (LPDWORD)& dwTmp, NULL))
//	{
//		DeleteObject(hBmp);
//		ReleaseDC(hWndScreen, hDC);
//		GlobalFree((HGLOBAL)lpBits);
//		CloseHandle(hf);
//		return FALSE;
//	}
//
//	DeleteObject(hBmp);
//	ReleaseDC(hWndScreen, hDC);
//	GlobalFree((HGLOBAL)lpBits);
//	CloseHandle(hf);
//
//	return TRUE;
//}

HBRUSH MenuButton::goldBrush = CreateSolidBrush(RGB(242, 206, 90));
HBRUSH MenuButton::standartBrush = CreateSolidBrush(RGB(225, 225, 225));
HPEN MenuButton::blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

MenuButton::MenuButton()
{
}

MenuButton::MenuButton(int x, int y, int width, int height, HWND hParent, HWND MainWindow, const WCHAR * imagePath, int ID, int Message_ID, string tooltipText, string tooltipTitle)
{
	SetRect(x, y, width, height);
	setWidth(width);
	setHeight(height);
	this->MainWindow = MainWindow;
	this->Message_ID = Message_ID;
	this->hParent = hParent;
	Object_ID = ID;
	wc.lpszClassName = "MyButton";
	wc.hbrBackground = CreateSolidBrush(RGB(225, 225, 225));
	wc.style = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hInstance = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
	wc.lpfnWndProc = sWndProc;

	RegisterClass(&wc);
	hWnd = CreateWindowEx(WS_EX_TRANSPARENT,"MyButton",
		"",
		WS_VISIBLE | WS_CHILD,
		x, y,
		width, height,
		hParent,
		(HMENU)ID,
		(HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE),
		NULL);

	Image = NULL;

	LoadPng(Image, imagePath);
	this->tooltipText = tooltipText;
	this->tooltipTitle = tooltipTitle;
	ToolTip::Create(hParent, hWnd, tooltipText, tooltipTitle);
}

MenuButton::~MenuButton()
{
}


LRESULT MenuButton::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	
	case CHANGESTATE:
		OnChangeState(wParam,lParam);	
		break;
	case WM_PAINT:
		OnPaint(wParam, lParam);		
		break;		
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
		break;		
	case WM_MOUSELEAVE:
		OnMouseLeave(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam);
		break;
	default:return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void MenuButton::OnChangeState(WPARAM wParam, LPARAM lParam)
{
	//Изменение состояния при наведении
	if (wParam == 0)
	{
		if (bool(lParam) != IsMouseInside)
		{
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		IsMouseInside = (bool)lParam;
	}
	else 
	{
		if ((bool)lParam != click)
		{
			click = (bool)lParam;
			cout << "Invalidate. Status:" << (bool)lParam << endl;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		else click = (bool)lParam;
	}

}

void MenuButton::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(hWnd, &ps), MemDCExercising;
	MemDCExercising = CreateCompatibleDC(hDc);
	if (click == true)
	{
		SelectObject(hDc, goldBrush);
		SelectObject(hDc, blackPen);
		Rectangle(hDc, 0, 0, getWidth(), getHeight());
		SelectObject(hDc, goldBrush);
	}
	else
	{
		if (IsMouseInside == true)
		{
			SelectObject(hDc, goldBrush);
			SelectObject(hDc, blackPen);
			Rectangle(hDc, 0, 0, getWidth(), getHeight());

			SelectObject(hDc, goldBrush);
		}
		else
		{
			SelectObject(hDc, standartBrush);
		}
	}
	auto brush = SelectObject(MemDCExercising, Image);

	BitBlt(hDc, 4, 4, 16, 16, MemDCExercising, 0, 0, MERGECOPY);
	SelectObject(MemDCExercising, brush);
	ReleaseDC(hWnd, MemDCExercising);
	ReleaseDC(hWnd, hDc);
	DeleteDC(MemDCExercising);
	EndPaint(hWnd, &ps);
	
}

void MenuButton::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	
	if (!IsMouseInside)
	{
		TRACKMOUSEEVENT tme;

		tme.cbSize = sizeof(tme);
		tme.hwndTrack = hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		TrackMouseEvent(&tme);
		IsMouseInside = true;
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
}

void MenuButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	IsMouseInside = false;
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}

void MenuButton::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	click = !click;
	SendMessage(MainWindow, Message_ID, NULL, LPARAM(click));
}
