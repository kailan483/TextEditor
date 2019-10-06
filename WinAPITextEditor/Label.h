#pragma once
#include "WinAPIWindow.h"
class Label :
	public WinAPIWindow
{
private:
	string Text;
	SIZE CalculateDimensionsForFont(HFONT hFont);
public:
	void SetFont(HFONT& hFont);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Label(){}
	Label(int x, int y, HWND parent, string Text);
};

