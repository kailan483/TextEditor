#include "MenuButton.h"

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
