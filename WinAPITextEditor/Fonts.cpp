#include "Fonts.h"
ComboBox<HFONT>* Fonts::fcb = nullptr;
vector<const CHAR*> Fonts::unnecesaryFonts;
int Fonts::FontProc(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam)
{
	if ((SendMessage(fcb->hWnd, CB_FINDSTRINGEXACT, 1, (LPARAM)lpelfe->lfFaceName) == CB_ERR))
	{
		if (!isUnnecesary(lpelfe->lfFaceName))
		{
			HFONT hFont = NULL;
			hFont = CreateFont(18, 9, 0, 0, lpelfe->lfWeight, FALSE, FALSE, FALSE, lpelfe->lfCharSet, lpelfe->lfOutPrecision, lpelfe->lfClipPrecision, PROOF_QUALITY, FF_MODERN, lpelfe->lfFaceName);
			fcb->AddItem(hFont);
			auto index = SendMessage(fcb->hWnd, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)lpelfe->lfFaceName);
			SendMessage(fcb->hWnd, CB_SETITEMDATA, index, (LPARAM)index);
		}
	}
	return 1;
}

void Fonts::GetFonts(HDC hdc)
{
	unnecesaryFonts.push_back("Marlett");
	unnecesaryFonts.push_back("Webdings");
	unnecesaryFonts.push_back("Wingdings");
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0] = '\0';
	EnumFontFamiliesEx(hdc, &lf, FontProc, 0, 0);
}

bool Fonts::isUnnecesary(string fontName)
{
	for (auto& i : unnecesaryFonts)
	{
		if (strcmp(fontName.c_str(), i) == 0 || strstr(fontName.c_str(), i) != 0)
			return true;
	}
	return false;
}
