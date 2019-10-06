#pragma once
#include "WinAPIWindow.h"
class ClientArea :
	public WinAPIWindow
{
public:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ClientArea();
	ClientArea(int x, int y, int width, int height, HWND hParent);
private:
	HWND parent;
};

