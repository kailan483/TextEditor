#include "FontComboBox.h"



FontComboBox::FontComboBox()
{

}

FontComboBox::FontComboBox(int x, int y, int width, int height, HWND hParent, int ID)
{
	SetRect(x, y, width, height);
	setWidth(width);
	setHeight(height);
	
	hWnd = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT | WS_VSCROLL | CBS_OWNERDRAWFIXED | WS_TABSTOP | WS_CHILD
		,
		x, y, width, height, hParent, NULL , NULL,
		NULL);

	auto index = FindItem("Times New Roman");

	SendMessage(hWnd, CB_SETCURSEL, index, 0);
	
}

void FontComboBox::AddFont(HFONT hfont)
{
	fonts.push_back(hfont);
}

HFONT FontComboBox::GetFont(int index)
{
	return fonts[index];
}


FontComboBox::~FontComboBox()
{
}

LRESULT FontComboBox::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void FontComboBox::SetFont(HFONT font)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE,0));
}


// Поиск элемента по имени и возврат его индекса
int FontComboBox::FindItem(string Name)
{
	int index = SendMessage(hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)Name.c_str());
	return index;
}
