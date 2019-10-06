#include "MinimizeButton.h"



MinimizeButton::MinimizeButton()
{
}

MinimizeButton::MinimizeButton(int x, int y, int width, int height, HWND hParent)
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
		(HMENU)ID_MINIMIZEBUTTON,
		(HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE),
		NULL);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Load the image 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(L"images\\minimize_2.png");
	HBITMAP expandButtonImage = NULL;

	image->GetHBITMAP(Color(192,192,192), &expandButtonImage);

	// do something with your image 
	// ... 

	// delete the image when done 
	delete image; image = 0;

	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(gdiplusToken);

	//LoadBitmapFromBMPFile(szFileName, &image, &hPalette);

	//image = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PNG1));

	auto error = GetLastError();

	SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)expandButtonImage);
}


MinimizeButton::~MinimizeButton()
{
}
