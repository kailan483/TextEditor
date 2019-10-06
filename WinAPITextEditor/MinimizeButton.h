#pragma once
#include "WinAPIWindow.h"
class MinimizeButton :
	public WinAPIWindow
{
public:
	MinimizeButton();
	MinimizeButton(int x, int y, int width, int height, HWND hParent);
	~MinimizeButton();
private:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}
};

