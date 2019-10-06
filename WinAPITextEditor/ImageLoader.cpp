#include "ImageLoader.h"

HBITMAP ImageLoader::loadImage(std::string name)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Load the image 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(std::to_wstring(name));
}
