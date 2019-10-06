//#pragma once
#ifndef WINAPI_WINDOW_H
#define WINAPI_WINDOW_H
//#pragma comment(lib,"Gdi32.lib")
#include <Windows.h>
#include <map>
#include <iostream>
#pragma comment(lib,"gdiplus.lib")
#include <gdiplus.h>
#include <Richedit.h>
#pragma comment (lib, "Msimg32.lib")
#include <CommCtrl.h>
#pragma comment(lib,"ComCtl32.Lib")
#include <algorithm>
#include "resource1.h"
#include "resource.h"
using namespace std;
using namespace Gdiplus;

class WinAPIWindow;

using WndMap = map<HWND, WinAPIWindow*>;


class WinAPIWindow
{
public:
	static LRESULT CALLBACK sWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static WndMap WindowsMap;

	void Show() { ShowWindow(hWnd,SW_SHOWNORMAL); }
	void Update() { UpdateWindow(hWnd); }

	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual ~WinAPIWindow() {
	
		
		/*cout << "”ничтожение" << endl;*/
	}
	static void Attach(HWND h, WinAPIWindow* w);
	HWND hWnd;
	

	int getX() const 
	{ 
		return this->x;
	}
	int getY() const
	{
		return this->y;
	}
	int getWidth() const
	{
		return width;
	}
	void Redraw(const RECT* r)
	{		
		InvalidateRect(hWnd, r,TRUE);
		UpdateWindow(hWnd);
		delete r;
	}
	int getHeight() const
	{
		return height;
	}
	void SetRect(int x, int y, int width, int height)
	{
		this->y = y;	
		this->x = x;		
	}
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void DebugPrintDimensions()
	{
		cout << "x:" << this->x << " y:" << this->y << " height:" << height << " width:" << width << endl;
	}
	void SetPosition(int x, int y)
	{						
		setX(x);
		setY(y);
		BOOL result = SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);		
		/*UpdateWindow(hWnd);*/
	}
	void SetWidthAndHeight(int width, int height)
	{
		this->width = width;
		this->height = height;
		SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOREDRAW);
	}
	void LoadPng(HBITMAP& Image, const WCHAR* path)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// Load the image 
		Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(path);

		image->GetHBITMAP(Color(228, 228, 228), &Image);

		// delete the image when done 
		delete image; image = 0;

		// Shutdown Gdiplus 
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
	RECT getRect() { return RECT{ x,y,x+width,y+height }; }
protected:
	
	int width, height, x, y;

	WNDCLASS wc;
};
#endif
