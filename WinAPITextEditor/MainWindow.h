#pragma once

#include "WinAPIWindow.h"
#include "MenuBar.h"
#include <vector>
#include <string>
#include <map>
#include "resource.h"
#include "FontComboBox.h"
#include "MenuButton.h"
#include "MenuButton2.h"
#include "Fonts.h"
#include "ComboBox.h"
#include "Document.h"
#include "ToolTip.h"
#include "ClientArea.h"
#include "ButtonWithLabel.h"
#include <TextServ.h>
#include <commdlg.h>
enum class WindowSizingTypes { EXPANDED = 2, COLLAPSED = 1, WINDOW_COLLAPSED = 0 };
enum class CollapseMode { COLLAPSE_IN_WINDOW, FULL_COLLAPSE };
using namespace std;
class MainWindow :
	public WinAPIWindow
{
public:
	MainWindow(HINSTANCE hInst);
	/*void InitScrollbar();*/
	static BOOL CALLBACK InsertTableDlgProc(HWND hwndDlg,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
	static BOOL CALLBACK MyColorProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	/*LRESULT CALLBACK ClientAreaProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);*/
private:
	void LoadColors();
	void CreateObjects();
	void ReplaceItems();
	/*TextField tf;*/
	MenuBar menuBar;
	HBITMAP AppIcon;
	HICON taskBarIcon;
	MenuButton bb;
	MenuButton sb;
	MenuButton ufb;
	MenuButton ib;
	MenuButton dib;
	MenuButton uib, leftAlignButton, rightAlignButton, centerAlignButton, justifyButton;
	MenuButton2 InsertTableButton;
	MenuButton2 ColorButton;
	MenuButton2 leftButton, rightButton, saveButton;
	MenuButton2 copyButton, cutButton, pasteButton;
	MenuButton2* findButton = nullptr;
	MenuButton2* findReplaceButton = nullptr;
	MenuButton2* selectAllButton = nullptr;
	Document* doc;
	ComboBox<HFONT> fcb;
	ComboBox <int> fzcb;
	ClientArea clientArea;
	bool drag = false;		
	void Collapse(CollapseMode collapseMode);
	void Expand();	
	void RescalePositions();
	void CheckAlignment();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	string Title = "TextEditor", ClassName = "MainWindow";

	//Flag used to indicate maximized window
	bool Size_Maximized = false;
	WindowSizingTypes WindowSizingType;
	//x and y c-s of TextField before MainWnd was maximized
	int xBeforeMaximized, yBeforeMaximized;

	//x & y of main window before maximized (bm)
	int xWindowBM, yWindowBM;
	//width & height of main window before maximized (bm)
	int wWindowBM, hWindowBM;
	//Client height
	
	RECT workArea;

	HBRUSH titleBrush;

	HINSTANCE AppHinstance;

	CHARFORMAT2 cf2;
	PARAFORMAT2 pf2;
	HFONT font;
	int tableRows, tableColumns;
	int nPos;
	int nOldPos;
	~MainWindow()
	{
		DeleteObject(titleBrush);
		DeleteObject(font);
		DeleteObject(ButtonWithLabel::labelFont);
	}
};