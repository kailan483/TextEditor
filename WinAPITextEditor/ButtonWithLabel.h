#pragma once
#include "MenuButton2.h"
#include "Label.h"
class ButtonWithLabel: public MenuButton2
{
private:
	string labelText;
	SIZE labelDimensions;
	POINT pLabelPos;
public:
	ButtonWithLabel()
	{

	}
	ButtonWithLabel(int x, int y, int width, int height, HWND hParent, HWND MainWindow, const WCHAR* imagePath, int ID, int Message_ID, string tooltipText, string tooltipTitle, string labelText);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HFONT labelFont;
	SIZE CalculateDimensionsForFont(HFONT hFont);
	/*Label* lbl = nullptr;*/
	void OnPaint(WPARAM wParam, LPARAM lParam);
};

