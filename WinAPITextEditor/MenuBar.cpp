#include "MenuBar.h"


#pragma comment(lib,"gdiplus.lib")
#include <gdiplus.h>
using namespace Gdiplus;
MenuBar::MenuBar()
{
}

MenuBar::MenuBar(int x, int y, int width, int height, HWND hParent)
{
	wc.lpszClassName = "MenuBar";
	wc.hbrBackground = CreateSolidBrush(RGB(225, 225, 225));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = sWndProc;
	staticBrush = CreateSolidBrush(RGB(225, 225, 225));
	RegisterClass(&wc);
	hPen = CreatePen(PS_INSIDEFRAME, 1, RGB(192, 192, 192));
	hWnd = CreateWindowEx(0,"MenuBar", "Name", WS_CHILD, x, y, width, height, hParent, LoadMenu((HINSTANCE)GetWindowLongPtr(hParent, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_MENU1)), NULL, NULL);
	CategoryFont = CreateFont(16, 6, 0, 0, 1000, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	setWidth(width);
	setHeight(height);
	setX(x);
	setY(y);
	this->hParent = hParent;
}
LRESULT MenuBar::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT currentMousePos;
	switch (msg)
	{
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkColor(hdcStatic, RGB(225,225,225));
		return (INT_PTR)staticBrush;		
	}
		
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hPen);
		DrawSeparator(hdc, 110, 0, 110, height);
		DrawSeparator(hdc, 420, 0, 420, height);
		DrawSeparator(hdc, 584, 0, 584, height);
		SetBkColor(hdc, RGB(225, 225, 225));
		SelectObject(hdc, CategoryFont);
		SetTextColor(hdc, RGB(64, 64, 64));
		drawText(hdc, 225, 90, "Шрифт");
		drawText(hdc, 445, 90, "Форматирование");
		EndPaint(hWnd, &ps);
	}; break;
	case WM_MEASUREITEM:
	{
		LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight = 24;
		break;
	};
	case WM_DRAWITEM:
	{
		char achTemp[256];
		COLORREF clrBackground;
		COLORREF clrForeground;
		TEXTMETRIC tm;
		int x;
		int y;
		HRESULT hr;
		size_t cch;

		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
		
		if (lpdis->itemID == -1) // Empty item
			break;
		int index = (int)lpdis->itemData;
		clrForeground = SetTextColor(lpdis->hDC,
			GetSysColor(COLOR_BTNTEXT));
		clrBackground = SetBkColor(lpdis->hDC,
			GetSysColor(lpdis->itemState & ODS_SELECTED ?
				COLOR_3DLIGHT : COLOR_WINDOW));
		/*Calculate the verticaland horizontal position.*/
		GetTextMetrics(lpdis->hDC, &tm);
		lpdis->rcItem.bottom += 2;
		y = (lpdis->rcItem.bottom + lpdis->rcItem.top - tm.tmHeight) / 2;
		if (!(lpdis->itemState & ODS_COMBOBOXEDIT) && lpdis->CtlID == ID_FONTCOMBOBOX)
		{
			x = LOWORD(GetDialogBaseUnits()) / 4 /*+ 30*/;
		}
		else x = LOWORD(GetDialogBaseUnits()) / 4;
		/*Getand display the text for the list item.*/
		SendMessage(lpdis->hwndItem, CB_GETLBTEXT,
			lpdis->itemID, (LPARAM)achTemp);
		if (lpdis->CtlID == ID_FONTCOMBOBOX && !(lpdis->itemState & ODS_COMBOBOXEDIT))
			SelectObject(lpdis->hDC, fcb->GetItem(index));
		hr = StringCchLength(achTemp, 256, &cch);
		if (FAILED(hr))
		{
			/*TODO: Write error handler.*/
		}
		/*If the item has the focus, draw the focus rectangle.*/
		if (lpdis->itemState & ODS_FOCUS)
		{
			/*HPEN hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 255)), hPrevPen;
			hPrevPen = SelectPen(lpdis->hDC, hPen);
			Rectangle(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom);*/
			DrawFocusRect(lpdis->hDC, &lpdis->rcItem);
			/*hPen = SelectPen(lpdis->hDC, hPrevPen);
			FreeResource(hPen);*/
		}
		ExtTextOut(lpdis->hDC, x, y,
			ETO_CLIPPED | ETO_OPAQUE, &lpdis->rcItem,
			achTemp, (UINT)cch, NULL);
		/*Restore the previous colors.*/
		SetTextColor(lpdis->hDC, clrForeground);
		SetBkColor(lpdis->hDC, clrBackground);	
		break;
	};
	case WM_LBUTTONDOWN:
	{
		SendMessage(hParent, WM_RESTOREFOCUS, 0, 0);
	};
	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
		{

			HWND comboBox = (HWND)lParam;
			auto index = SendMessage(comboBox, CB_GETCURSEL, 0, 0);
			if (index == CB_ERR)
			{
				cout << "CB_GETCURSEL error!" << endl;
				return LRESULT(CB_ERR);
			}
			auto stringLen = SendMessage(comboBox, CB_GETLBTEXTLEN, (WPARAM)index, 0);
			if (stringLen == CB_ERR)
			{
				cout << "CB_GETLBTEXTLEN error!" << endl;
				return LRESULT(CB_ERR);
			}
			TCHAR* content = new TCHAR[stringLen+1];
			if (SendMessage(comboBox, CB_GETLBTEXT, index, (LPARAM)content) == CB_ERR)
			{
				cout << "CB_GETLBTEXT error!" << endl;
				delete[] content;
				return LRESULT(CB_ERR);
			}
			
			switch (LOWORD(wParam))
			{
			case ID_FONTCOMBOBOX:
			{
				SendMessage(hParent, ID_SETFONTNAME, NULL, (LPARAM)(LPCTSTR(content)));
				break;
			}
			case ID_FONTSIZECOMBOBOX:
			{
				SendMessage(hParent, ID_SETFONTSIZE, NULL, (LPARAM)stoi(content));
				break;
			}
			}
			delete[] content;
			break;
		};
		}
		switch (LOWORD(wParam))
		{
		case ID_PRINT:
			SendMessage(hParent, ID_PRINT, 0, 0);
			break;
		case ID_COPY:
			SendMessage(hParent, ID_COPY, 0, 0);
			break;
		case ID_SAVE:
			SendMessage(hParent, ID_SAVEFILE, 0, 0);
			break;
		case ID_OPEN:
			SendMessage(hParent, ID_OPEN, 0, 0);
			break;
		case ID_SAVEAS:
			SendMessage(hParent, ID_SAVEAS, 0, 0);
			break;
		case ID_CREATENEW:
			SendMessage(hParent, ID_CREATENEW, 0, 0);
			break;
		case ID_CLOSE:
			SendMessage(hParent, WM_DESTROY, 0, 0);
			break;
		case ID_UNDO:
			SendMessage(hParent, ID_UNDO, 0, 0);
			break;
		case ID_REDO:
			SendMessage(hParent, ID_REDO, 0, 0);
			break;
		case ID_CUT:
			SendMessage(hParent, ID_CUT, 0, 0);
			break;
		case ID_PASTE:
			SendMessage(hParent, ID_PASTE, 0, 0);
			break;
		case ID_DELETE:
			SendMessage(hParent, ID_DELETE, 0, 0);
			break;
		case ID_SELECTALL:
			SendMessage(hParent, ID_SELECTALL, 0, 0);
			break;
		default:
			break;
		}
	}; break;
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}; break;
	}
}

bool MenuBar::DrawSeparator(HDC hDc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hDc, x1, y1, NULL);
	return LineTo(hDc, x2, y2);
}
void MenuBar::drawText(HDC hdc, int x, int y, std::string str)
{
	const int length = str.length();
	TextOut(hdc, x, y, str.c_str(), length);
}
MenuBar::~MenuBar()
{
}
