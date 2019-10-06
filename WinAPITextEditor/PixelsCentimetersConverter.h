#pragma once
#ifndef PIXELS_CENTIMETERS_CONVERTER
#define	PIXELS_CENTIMETERS_CONVERTER
#include <Windows.h>
class Converter
{
public:
	static double toPixels(double centimeters, HWND window);
	static double toCentimeters(double pixels, HWND window);
	static double MillimetersToPixels(double millimetres, HWND window);
	static const double CentimetersPerInch;
	static const double MillimetersPerInch;
private:
};

#endif // !PIXELS_CENTIMETERS_CONVERTER


