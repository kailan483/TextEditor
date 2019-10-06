#include "WinAPIWindow.h"

WndMap WinAPIWindow::WindowsMap;

LRESULT WinAPIWindow::sWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WndMap::iterator it = WindowsMap.find(hWnd);
	if (it != WindowsMap.end())
	{

		auto w = it->second;
		return w->WindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void WinAPIWindow::Attach(HWND h, WinAPIWindow* w)
{
	WindowsMap[h] = w;
}
