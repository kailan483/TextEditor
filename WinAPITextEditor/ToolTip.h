#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
using namespace std;
class ToolTip
{
public:
	static HWND Create(int toolID, HWND hDlg, string pszText);
	static HWND Create(HWND hwndOwner,HWND hWndTool, string pszText, string title);
	static HWND CreateTrackingToolTip(int toolID, HWND parent, string pszText);
};

