#pragma once
#include "WinAPIWindow.h"
class ExpandButton :
	public WinAPIWindow
{
public:
	ExpandButton() {};
	ExpandButton(int x, int y, int width, int height, HWND hParent);
	~ExpandButton();
private:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd,msg,wParam,lParam);
		
	}
};

