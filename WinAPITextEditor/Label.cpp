#include "Label.h"

SIZE Label::CalculateDimensionsForFont(HFONT hFont)
{
	SIZE sz;
	HDC hdc = GetDC(hWnd);
	HFONT font = (HFONT)SelectObject(hdc, hFont);
	GetTextExtentPoint32(hdc, Text.c_str(), Text.length(), &sz);
	SelectObject(hdc, font);
	ReleaseDC(hWnd, hdc);
	DeleteObject(hFont);
	return sz;
	/*return SIZE();*/
}

void Label::SetFont(HFONT& hFont)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
}

LRESULT Label::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc;
	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkColor(hdc, RGB(225, 225, 225));
		EndPaint(hWnd, &ps);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

Label::Label(int x, int y, HWND parent, string Text)
{
	HFONT hFont = CreateFont(18, 7, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");
	this->Text = Text;
	hWnd = CreateWindow("static", Text.c_str(),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		x, y, 0, 0,
		parent, (HMENU)(ID_STATIC1),
		(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), NULL);
	auto dimensions = CalculateDimensionsForFont(hFont);
	if (hWnd != NULL)
	{

		y -= dimensions.cy / 2;
		SetWindowText(hWnd, Text.c_str());
		SetWindowPos(hWnd, NULL, x, y, dimensions.cx, dimensions.cy, 0);
		setX(x);
		setY(y);
		setWidth(dimensions.cx);
		setHeight(dimensions.cy);
		UpdateWindow(hWnd);
	}
	else cout << "Label creation failed!" << endl;
	DeleteObject(hFont);
}