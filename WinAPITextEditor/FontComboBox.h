#pragma once
#include <vector>
#include "WinAPIWindow.h"
#include "resource.h"


class FontComboBox :
	public WinAPIWindow
{
public:
	FontComboBox();
	FontComboBox(int x, int y, int width, int height, HWND hParent, int ID);
	void AddFont(HFONT hfont);
	HFONT GetFont(int index);
	void SetFont(HFONT font);
	~FontComboBox();
private:
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	vector<HFONT> fonts;
public:
	// Поиск элемента по имени и возврат его индекса
	int FindItem(string Name);
};

