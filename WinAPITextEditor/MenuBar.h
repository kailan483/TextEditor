#pragma once
#include <strsafe.h>
#include "WinAPIWindow.h"
//#include "FontComboBox.h"
#include "ComboBox.h"
#include "resource.h"
#include "resource1.h"
#include <vector>
using namespace std;
class MenuBar :
	public WinAPIWindow
{
public:

	MenuBar();
	MenuBar(int x, int y, int width, int height, HWND hParent);
	HWND hParent;
	~MenuBar();
	ComboBox<HFONT>* fcb = nullptr;
private:
	bool DrawSeparator(HDC hDc, int x1, int y1, int x2, int y2);
	void drawText(HDC hdc, int x, int y, std::string str);
	HPEN hPen;
	HFONT CategoryFont;
	HBRUSH staticBrush;
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};