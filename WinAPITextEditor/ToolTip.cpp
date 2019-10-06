#include "ToolTip.h"
#include <CommCtrl.h>
HWND ToolTip::Create(int toolID, HWND hDlg, string pszText)
{
	if (!toolID || !hDlg || pszText.empty())
	{
		return FALSE;
	}
	// Get the window of the tool.
	HWND hwndTool = GetDlgItem(hDlg, toolID);

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL,
		(HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), NULL);
	HFONT hfont = CreateFont(20, 8, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");
	
	SendMessage(hwndTip, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
	if (!hwndTool || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = _strdup(pszText.c_str());
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)& toolInfo);
	SendMessage(hwndTip, TTM_SETTIPBKCOLOR, (WPARAM)RGB(204, 255, 255), 0);
	FreeResource(hfont);
	return hwndTip;
}

HWND ToolTip::Create(HWND hwndOwner,HWND hWnd, string pszText, string title)
{

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP ,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hWnd, NULL,
		(HINSTANCE)GetWindowLong(hwndOwner, GWL_HINSTANCE), NULL);
	HFONT hfont = CreateFont(16, 7, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");

	SendMessage(hwndTip, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
	if (!hWnd || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	SendMessage(hWnd, TTM_GETTOOLINFO, 0, (LPARAM)& toolInfo);
	toolInfo.hwnd = hwndOwner;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hWnd;
	toolInfo.lpszText = _strdup(pszText.c_str());
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)& toolInfo);
	SendMessage(hwndTip, TTM_SETTIPBKCOLOR, (WPARAM)RGB(204, 255, 255), 0);
	SendMessage(hwndTip, TTM_SETTITLE, (WPARAM)TTI_NONE, (LPARAM)title.c_str());
	FreeResource(hfont);
}

HWND ToolTip::CreateTrackingToolTip(int toolID, HWND owner, string pszText)
{
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		owner, NULL,
		(HINSTANCE)GetWindowLong(owner, GWL_HINSTANCE), NULL);
	HFONT hfont = CreateFont(20, 8, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");
	SendMessage(hwndTip, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
	if (!owner || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = owner;
	toolInfo.uFlags = TTF_IDISHWND | TTF_TRACK;
	toolInfo.uId = (UINT_PTR)owner;
	toolInfo.lpszText = _strdup(pszText.c_str());
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)& toolInfo);
	SendMessage(hwndTip, TTM_SETTIPBKCOLOR, (WPARAM)RGB(204, 255, 255), 0);	
	FreeResource(hfont);
	return hwndTip;
}
