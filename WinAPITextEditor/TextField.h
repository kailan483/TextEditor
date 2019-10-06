#pragma once
#include "WinAPIWindow.h"
#include "PixelsCentimetersConverter.h"
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
class TextField :
	public WinAPIWindow
{
public:
	TextField(){}
	TextField(HINSTANCE hInst,int x, int y, int width, int height, HWND hParent);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//int MinWidth, MaxWidth;
	int minHeight;
	void Perform(int messageId, WPARAM, LPARAM);
	void SetMargins(double rightMargin, double leftMargin, double topMargin, double bottomMargin);
	void RescaleWidthWithMargins();
	void SetSelection(int startPos, int endPos);
	~TextField()
	{
		DestroyWindow(hWnd);
	}
	/*TextField(const TextField& TextField);*/
private:
	double leftMargin, rightMargin, topMargin, bottomMargin;
public:
	BOOL Hide();
	BOOL Show();
};

