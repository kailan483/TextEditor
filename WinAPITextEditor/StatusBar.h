#pragma once
#include "WinAPIWindow.h"
class StatusBar :
	public WinAPIWindow
{
public:
	StatusBar(HWND hParent);
	LRESULT WINAPI WindowProc();
	~StatusBar();
};

