#include "ExpandButton.h"



ExpandButton::ExpandButton(int x, int y, int width, int height, HWND hParent)
{

	SetRect(x, y, width, height);
	setWidth(width);
	setHeight(height);

	wc.lpfnWndProc = sWndProc;
	hWnd = CreateWindowEx(
		0,
		"BUTTON",
		"",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP | BS_PUSHLIKE,
		x, y,
		width, height,
		hParent,
		(HMENU)ID_EXPANDBUTTON,
		(HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE),
		NULL);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Load the image 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(L"images\\expand_2.png");

	HBITMAP expandButtonImage = NULL;

	image->GetHBITMAP(Color(192, 192, 192), &expandButtonImage);

	// delete the image when done 
	delete image; image = 0;

	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(gdiplusToken);
	auto error = GetLastError();

	SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)expandButtonImage);
}

ExpandButton::~ExpandButton()
{
}
