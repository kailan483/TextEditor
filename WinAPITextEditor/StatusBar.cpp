#include "StatusBar.h"
StatusBar::StatusBar(HWND hParent)
{
	hWnd = CreateStatusWindow(WS_CHILD | WS_VISIBLE, "Text", hParent,ID_STATUSWINDOW);
}

LRESULT WINAPI  StatusBar::WindowProc()
{
	// TODO: �������� ���� ��� ����������.
	return LRESULT();
}

StatusBar::~StatusBar()
{
}
