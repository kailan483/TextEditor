#pragma once
#include "WinAPIWindow.h"
#include "ToolTip.h"
#include <windowsx.h>
class MenuButton :
	public WinAPIWindow
{
public:
	MenuButton();
	MenuButton(int x, int y, int width, int height, HWND hParent, HWND MainWindow, const WCHAR *imagePath,int ID,int Message_ID,string tooltipText, string tooltipTitle);
	~MenuButton();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	void OnChangeState(WPARAM wParam, LPARAM lParam);
	void OnPaint(WPARAM wParam, LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(WPARAM wParam, LPARAM lParam);
	static HPEN blackPen;
	static HBRUSH goldBrush, standartBrush;
	HBITMAP Image;
	HWND MainWindow, hParent;
	int Message_ID, Object_ID;
	bool IsMouseInside = false;
	bool click = false;
	string tooltipText, tooltipTitle;
	int currentState = 0;
};


