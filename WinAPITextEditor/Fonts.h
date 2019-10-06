#pragma once
#include <vector>
#include <string>
#include "ComboBox.h"
using namespace std;

class Fonts
{
public:
	static int CALLBACK FontProc(
		const LOGFONT* lpelfe,
		const TEXTMETRIC* lpntme,
		DWORD      FontType,
		LPARAM     lParam
	);
	static void GetFonts(HDC hdc);
	static ComboBox<HFONT>* fcb;
	static vector<const CHAR*> unnecesaryFonts;
	static bool isUnnecesary(string fontName);
};

