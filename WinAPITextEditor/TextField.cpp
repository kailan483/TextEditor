#include "TextField.h"

TextField::TextField(HINSTANCE hInst, int x, int y, int width, int height, HWND hParent)
{
	SetRect(x, y, width, height);
	setWidth(width);
	setHeight(height);
	minHeight = height;

	LoadLibrary("Msftedit.dll");	

	hWnd = CreateWindowExW(0, MSFTEDIT_CLASS, NULL, ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_VSCROLL | ES_NOHIDESEL | WS_CLIPCHILDREN, x, y, width, height, hParent, NULL, (HINSTANCE)GetWindowLongPtr(hParent, GWL_HINSTANCE), NULL);
	//Send EN_SELCHANGE notification to a main window
	SetProp(hWnd, "ObjPointer", this);
	SendMessage(hWnd, EM_EXLIMITTEXT, 0, (LPARAM)2000000000);
	/*SetWindowSubclass(hWnd, EditProc, 2, 0);*/
}

LRESULT TextField::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CONTEXTMENU:
	{
		HMENU hMenu = CreatePopupMenu();

		AppendMenu(hMenu, MFT_STRING, ID_CUT, "&Вырезать");
		AppendMenu(hMenu, MFT_STRING, ID_COPY, "&Копировать");
		AppendMenu(hMenu, MFT_STRING, ID_PASTE, "&Вставить");
		AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MFT_STRING, ID_CLOSE, "В&ыход");

		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN |
			TPM_LEFTALIGN,
			LOWORD(lParam),
			HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
	break;
	default:
		break;
	}
   return DefWindowProc(hWnd, msg, wParam, lParam);
}

void TextField::Perform(int messageId, WPARAM wParam, LPARAM lParam)
{
	SendMessage(hWnd, messageId, wParam, lParam);
}

void TextField::SetMargins(double rightMargin, double leftMargin, double topMargin, double bottomMargin)
{

	leftMargin = Converter::toPixels(leftMargin, hWnd);
	rightMargin = Converter::toPixels(rightMargin, hWnd);
	topMargin = Converter::toPixels(topMargin, hWnd);
	bottomMargin = Converter::toPixels(bottomMargin, hWnd);

	this->leftMargin = leftMargin;
	this->rightMargin = rightMargin;
	this->bottomMargin = bottomMargin;
	this->topMargin = topMargin;

	RECT rc;
	// Get the current control rectangle
	GetClientRect(hWnd, &rc);
	rc.left += leftMargin; 
	rc.top += topMargin; 
	rc.right -= rightMargin; 
	rc.bottom -= bottomMargin; 
	// Set the rectangle
	SendMessage(hWnd, EM_SETRECT, 0, (LPARAM)& rc);
	setWidth(getWidth() - (rightMargin + leftMargin));
	setHeight(getHeight() - (bottomMargin - topMargin));
}
void TextField::RescaleWidthWithMargins()
{
	setWidth(getWidth() - (rightMargin + leftMargin));
	setHeight(getHeight() - (bottomMargin - topMargin));
}
void TextField::SetSelection(int startPos, int endPos)
{
	SendMessage(hWnd, EM_SETSEL, (WPARAM)startPos, (LPARAM)endPos);
}
BOOL TextField::Hide()
{
	// TODO: Добавьте сюда код реализации.
	return ShowWindow(hWnd, SW_HIDE);
}
BOOL TextField::Show()
{
	// TODO: Добавьте сюда код реализации.
	return ShowWindow(hWnd, SW_SHOW);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	static LPNMHDR nmhdr;
	static LPRECT lprc;
	POINT pt;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);
		break;
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
	
}
