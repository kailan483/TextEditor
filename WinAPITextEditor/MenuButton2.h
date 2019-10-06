#pragma once
#include "WinAPIWindow.h"
#include "ToolTip.h"
class MenuButton2 :
	public WinAPIWindow
{
public:
	MenuButton2();
	MenuButton2(int x, int y, int width, int height, HWND hParent, HWND MainWindow, const WCHAR* imagePath, int ID, int Message_ID, string tooltipText, string tooltipTitle);
	~MenuButton2();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HBRUSH goldBrush, standartBrush;
	static HPEN blackPen;
protected:
	void OnChangeState(WPARAM wParam, LPARAM lParam);
	void OnPaint(WPARAM wParam, LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(WPARAM wParam, LPARAM lParam);
	void OnLButtonUp(WPARAM wParam, LPARAM lParam);
	HBITMAP Image;
	HWND MainWindow, hParent;
	int Message_ID, Object_ID;
	bool IsMouseInside = false;
	bool click = false;
	string tooltipText, tooltipTitle;
};

