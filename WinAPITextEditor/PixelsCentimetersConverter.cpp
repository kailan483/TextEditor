#include "PixelsCentimetersConverter.h"

const double Converter::CentimetersPerInch = 2.54;
const double Converter::MillimetersPerInch = 25.4;

double Converter::toPixels(double centimeters, HWND window)
{
	HDC hdc = GetDC(window);
	int PPI = GetDeviceCaps(hdc, LOGPIXELSX);
	double pixels = centimeters / CentimetersPerInch * PPI;
	return pixels;
}

double Converter::toCentimeters(double pixels, HWND window)
{
	HDC hdc = GetDC(window);
	int PPI = GetDeviceCaps(hdc, LOGPIXELSX);
	double centimeters = pixels * CentimetersPerInch / PPI;
	return centimeters;
}

double Converter::MillimetersToPixels(double millimetres, HWND window)
{
	HDC hdc = GetDC(window);
	int PPI = GetDeviceCaps(hdc, LOGPIXELSX);
	double pixels = millimetres / MillimetersPerInch * PPI;
	return pixels;
}
