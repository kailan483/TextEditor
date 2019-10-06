#pragma once
#include "WinAPIWindow.h"
#include "resource.h"
#include <png.h>
#pragma comment(lib,"gdiplus.lib")
#include <gdiplus.h>
using namespace Gdiplus;
class CloseButton :
	public WinAPIWindow
{
public:
	CloseButton() {};
	CloseButton(int x, int y, int width, int height, HWND hParent);
	~CloseButton();
private:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
};

