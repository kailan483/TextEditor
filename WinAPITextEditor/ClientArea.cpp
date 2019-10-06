#include "ClientArea.h"
LRESULT ClientArea::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static LPNMHDR nmhdr;
	static REQRESIZE* rqrsz;
	static PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_CONTEXTMENU:
	{
		HMENU hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MFT_STRING, ID_CUT, "&Вырезать");
		AppendMenu(hMenu, MFT_STRING, ID_COPY, "&Копировать");
		AppendMenu(hMenu, MFT_STRING, ID_PASTE, "&Вставить");
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON |
			TPM_TOPALIGN |
			TPM_LEFTALIGN,
			LOWORD(lParam),
			HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}break;
	case WM_ERASEBKGND:
		return 1;
	case WM_COMMAND:
	{
		
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
		{
			SendMessage(parent, msg, wParam, lParam);
			break;
		}
		case 0:
		{
			SendMessage(parent, LOWORD(wParam), wParam, lParam);
			break;
		}
		}
		break;
	}
	case WM_NOTIFY:
	{
		nmhdr = (LPNMHDR)lParam;
		rqrsz = (REQRESIZE*)lParam;
		switch (nmhdr->code)
		{
			//Handle caret moving	
		case EN_SELCHANGE:
		{
			DWORD v1, v2;
			CHARRANGE cr;
			SendMessage(nmhdr->hwndFrom, EM_GETSEL, (WPARAM)& v1, (LPARAM)& v2);
			SendMessage(nmhdr->hwndFrom, EM_EXGETSEL, 0, (LPARAM)& cr);
			SendMessage(parent, msg, wParam, lParam);
		}; break;
		default:break;
		}
		switch (rqrsz->nmhdr.code)
		{
		case EN_REQUESTRESIZE:
		{
			SendMessage(parent, msg, wParam, lParam);
		}; break;
		}
	}; break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return FALSE;
}
ClientArea::ClientArea()
{

}
ClientArea::ClientArea(int x, int y, int width, int height, HWND hParent)
{
	parent = hParent;
	setX(x);
	setY(y);
	setWidth(width);
	setHeight(height);

	wc.lpszClassName = "ClientArea";
	wc.hbrBackground = CreateSolidBrush(RGB(166, 144, 240));
	wc.style = CS_DBLCLKS;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = 0;
	wc.hInstance = (HINSTANCE)GetWindowLong(hParent,GWL_HINSTANCE);
	wc.lpfnWndProc = sWndProc;

	RegisterClass(&wc);

	hWnd = CreateWindowEx(WS_EX_COMPOSITED,"ClientArea", 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, getX(), getY(), getWidth(), getHeight(), hParent, NULL, (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE), NULL);
}