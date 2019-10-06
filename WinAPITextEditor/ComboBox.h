#pragma once
#include "WinAPIWindow.h"
#include "resource.h"
#include "ToolTip.h"
#include <CommCtrl.h>
#include <iostream>
#include <vector>
#include <string>
#include <windowsx.h>
#include <tchar.h>
#include <wctype.h>
#include <algorithm>
static const int DEFAULT_TXT_LIM = 30000;
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define Combo_SetEditSel(hwndCtl, ichStart, ichEnd)  \
    (ComboBox_SetEditSel(hwndCtl,ichStart,ichEnd),0)
LRESULT CALLBACK ComboProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData
);
static int Combo_FindString(HWND hwndCtl, INT indexStart, LPTSTR lpszFind);
bool isPositiveInteger(const std::string& s);
static void DoAutoComplete(HWND hwnd, HWND hEdit, TCHAR ch);
#pragma comment (lib, "comctl32")
template<typename T>
class ComboBox :
	public WinAPIWindow
{
private:
	vector<T> items;
	void NextItem();
	void PrevItem();
public:
	ComboBox();
	ComboBox(int x, int y, int width, int height, HWND hParent, int ID);
	~ComboBox();
	void AddItem(T item);
	T GetItem(int index);
	LRESULT CALLBACK sWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SetFont(HFONT font);
	//Для шаблона с типом int
	void Init();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int lastSelectedValueIndex;
	void MakeAutoComplete();
	static BOOL IsExtended(HWND hwndCtl);
	void SetCurSel(int index);
};

template<typename T>
inline ComboBox<T>::ComboBox()
{

}

template<typename T>
inline ComboBox<T>::ComboBox(int x, int y, int width, int height, HWND hParent, int ID)
{
	this->lastSelectedValueIndex = 0;
	SetRect(x, y, width, height);
	setWidth(width);
	setHeight(height);

	if (typeid(T) == typeid(HFONT))
	{
		hWnd = CreateWindow(WC_COMBOBOX, TEXT(""),
			WS_BORDER | CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_CHILD
			,
			x, y, width, height, hParent, (HMENU)ID, NULL,
			NULL);
		auto err = SendMessage(hWnd, CB_SETDROPPEDWIDTH, 450, 0);
		if (err == CB_ERR) MessageBox(hParent, "CB_ERR", "CB_ERR", MB_ICONERROR);
	}
	else if (typeid(T) == typeid(int))
		hWnd = CreateWindow(WC_COMBOBOX, TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL | CBS_OWNERDRAWFIXED | WS_TABSTOP | WS_CHILD
			,
			x, y, width, height, hParent, (HMENU)ID, NULL,
			NULL);
	SetProp(hWnd, "ID", (HANDLE)ID);
	MakeAutoComplete();
}
template<typename T>
inline ComboBox<T>::~ComboBox()
{
	items.clear();
}

template<typename T>
inline void ComboBox<T>::AddItem(T item)
{
	items.push_back(item);
}

template<typename T>
inline T ComboBox<T>::GetItem(int index)
{
	if (!items.empty())
		return items.at(index);
}

template<typename T>
inline LRESULT ComboBox<T>::sWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

template<typename T>
inline void ComboBox<T>::SetFont(HFONT font)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
}

template<>
inline void ComboBox<int>::Init()
{
	for (int i = 8; i < 32; i++)
		SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)std::to_string(i).c_str());
}

template<typename T>
inline LRESULT ComboBox<T>::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case CHANGESTATE:
	{
		if (wParam == ID_FONTCOMBOBOX)
		{
			LPCTSTR lpszString = (LPCTSTR)lParam;
			int index = SendMessage(hWnd, CB_FINDSTRINGEXACT, 1, (LPARAM)lpszString);
			int curIndex = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
			if (index == CB_ERR)
			{
				cout << "CB_FINDSTRINGEXACT error!" << endl;
				return CB_ERR;
			}
			if (curIndex != index)
			{
				if (SendMessage(hWnd, CB_SETCURSEL, (WPARAM)index, 0) == CB_ERR)
				{
					cout << "CB_SETCURSEL error" << endl;
					return CB_ERR;
				}
			}
		}
		else if (wParam == ID_FONTSIZECOMBOBOX)
		{
			double yHeight = round((double)lParam / 20);
			int index = SendMessage(hWnd, CB_FINDSTRINGEXACT, 1, (LPARAM)to_string((int)yHeight).c_str());

			int curIndex = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
			if (index == CB_ERR)
			{
				cout << "CB_FINDSTRINGEXACT error!" << endl;
				return CB_ERR;
			}
			if (curIndex != index)
			{
				if (SendMessage(hWnd, CB_SETCURSEL, (WPARAM)index, 0) == CB_ERR)
				{
					cout << "CB_SETCURSEL error" << endl;
					return CB_ERR;
				}
			}
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}
template<typename T>
BOOL ComboBox<T>::IsExtended(HWND hwndCtl)
{
	// TODO: Добавьте сюда код реализации.
	static TCHAR buf[MAX_PATH];
	GetClassName(hwndCtl, buf, MAX_PATH);
	return 0 == _tcsicmp(buf, WC_COMBOBOXEX);
}

template<typename T>
inline void ComboBox<T>::SetCurSel(int index)
{
	ComboBox_SetCurSel(hWnd, index);
}

template<typename T>
void ComboBox<T>::MakeAutoComplete()
{
	// SubClass the combo's Edit control
	HWND hEdit = FindWindowEx(hWnd, NULL, WC_EDIT, NULL);
	SetWindowSubclass(hEdit, ComboProc, 1, 0);
}
template<typename T>
void ComboBox<T>::NextItem()
{
	int index = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (index != CB_ERR)
	{
		int itemCount = SendMessage(hWnd, CB_GETCOUNT, 0, 0);
		if (itemCount != CB_ERR)
		{
			if (index < itemCount) index ++;			
			SendMessage(hWnd, CB_SETCURSEL, index, 0);			
		}
	}
	return;
}
template<typename T>
void ComboBox<T>::PrevItem()
{
	int index = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (index != CB_ERR)
	{
		if (index > 0) index--;
		SendMessage(hWnd, CB_SETCURSEL, index, 0);
	}
	return;
}
