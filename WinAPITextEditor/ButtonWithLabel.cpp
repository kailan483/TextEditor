#include "ButtonWithLabel.h"
HFONT ButtonWithLabel::labelFont = CreateFont(16, 8, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");
ButtonWithLabel::ButtonWithLabel(int x, int y, int width, int height, HWND hParent, HWND MainWindow, const WCHAR* imagePath, int ID, int Message_ID, string tooltipText, string tooltipTitle, string labelText):MenuButton2(x,y,width,height,hParent,MainWindow,imagePath,ID,Message_ID,tooltipText,tooltipTitle)
{
	setY(y);
	setX(x);
	setWidth(width);
	setHeight(height);
	auto i = this;
	Attach(hWnd, this);
	this->labelText = labelText;
	auto dimensions = CalculateDimensionsForFont(labelFont);
	pLabelPos.x = getWidth() + 4;
	pLabelPos.y = getHeight() / 2 - dimensions.cy / 2;
	SetPosition(getX(), getY());
}
LRESULT ButtonWithLabel::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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
	case WM_LBUTTONUP:
		OnLButtonUp(wParam, lParam);
		break;
	case WM_CAPTURECHANGED:
		OnLButtonUp(wParam, lParam);
		break;
	default:return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

SIZE ButtonWithLabel::CalculateDimensionsForFont(HFONT hFont)
{

		SIZE sz;
		HDC hdc = GetDC(hWnd);
		HFONT font = (HFONT)SelectObject(hdc, hFont);
		GetTextExtentPoint32(hdc, labelText.c_str(), labelText.length(), &sz);
		SelectObject(hdc, font);
		ReleaseDC(hWnd, hdc);
		DeleteObject(hFont);
		return sz;
}

void ButtonWithLabel::OnPaint(WPARAM wParam, LPARAM lParam)
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
		SetBkColor(hDc, RGB(242, 206, 90));
	}
	else
	{
		if (IsMouseInside == true)
		{
			SelectObject(hDc, goldBrush);
			SelectObject(hDc, blackPen);
			Rectangle(hDc, 0, 0, getWidth(), getHeight());
			SelectObject(hDc, goldBrush);
			SetBkColor(hDc, RGB(242, 206, 90));
		}
		else
		{
			SelectObject(hDc, standartBrush);
			SetBkColor(hDc, RGB(225, 225, 225));
		}
	}
	auto brush = SelectObject(MemDCExercising, Image);
	HFONT hFont = labelFont;
	HFONT prevFont = (HFONT)SelectObject(hDc, hFont);
	
	TextOut(hDc, pLabelPos.x, pLabelPos.y, labelText.c_str(), labelText.length());
	BitBlt(hDc, 4, 4, 16, 16, MemDCExercising, 0, 0, MERGECOPY);
	hFont = (HFONT)SelectObject(hDc, prevFont);
	SelectObject(MemDCExercising, brush);
	ReleaseDC(hWnd, MemDCExercising);
	ReleaseDC(hWnd, hDc);
	DeleteDC(MemDCExercising);
	DeleteObject(hFont);
	DeleteObject(brush);
	EndPaint(hWnd, &ps);
}
