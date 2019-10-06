#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <shobjidl.h>
#include <Richedit.h>
#include <RichOle.h>
#include <atlstr.h>
#include <TOM.h>
#include "TextField.h"

using namespace std;
struct tableInfo
{
	int position;
	int cCell;
	int cRow;
	tableInfo()
	{
		cCell = 0;
		cRow = 0;
		position = 0;
	}
	tableInfo(int pos, int cCell, int cRow)
	{
		this->cCell = cCell;
		this->position = pos;
		this->cRow = cRow;
	}

};

class Document
{
private:
	CString strToFind;
	string GetSelectedText();
	HWND findHwnd, findReplaceHwnd;
	static FINDREPLACE fr;
	static string findBuffer, replaceBuffer;
	vector<tableInfo> tablePositions;
	CHARRANGE findCr;
	int pagesCount;
	int currentPage;
	TextField* Page;
	HWND* hwnd;
	HWND* main;
	HINSTANCE hInst;
	const int A4width = 210;
	const int A4height = 297;
	int x, y, width, height;
	static DWORD CALLBACK WriteFileEditStreamCallback(
		DWORD_PTR dwCookie,
		LPBYTE pbBuff,
		LONG cb,
		LONG* pcb
	);
	static DWORD CALLBACK ReadFileEditStreamCallback(
		DWORD_PTR dwCookie,
		LPBYTE pbBuff,
		LONG cb,
		LONG* pcb
	);
	std::string buffer;
	LPWSTR lpFilePath;
	IFileOpenDialog* pFileOpen;
	IFileSaveDialog* FileSaveDialog;
	IShellItem* pItem;
	IUnknown* pIUnk;
	ITextDocument2* iTextDocInterface;
	ITextSelection2* iTextSelection2;
	void GetITextDoc();
	BOOL GetOleInterface();
public:

	Document(HWND* hParent,HWND *main, HINSTANCE hInst,int width,int height)
	{
		
		this->hwnd = hParent;
		this->main = main;
		pagesCount = 0;
		this->hInst = hInst;
		this->width = width;
		this->height = height;
	}
//	void PushPage();
//	void PopPage();
	void Create(const int pagesCount);
//	void HidePage(int number);
//	void ShowPage(int number);
	void Show();
	int getX();
	int getY();
	int getHeight();
	int getWidth();
	void setX(int x);
	void setY(int y);
	void SetPosition(int x, int y);
	void SetMargins(double rightMargin, double leftMargin, double topMargin, double bottomMargin);
	void OpenFile();
	TextField& GetCurrentPage();
	void InsertTable(int columns, int rows, int EditWidth);

	void Save();
	void SaveAs();
	void CreateNew();

	void Undo();
	void Redo();
	void SelectAll();
	void Cut();
	void DeleteText();
	void Copy();
	void Paste();
	void ChangeSelectedTextColor();

	void DestroyFindDialog();
	void DestroyFindReplaceDialog();
	void Find(bool direction, bool matchCase, bool wholeWord, LPCSTR strToFind);
	void Replace(bool matchCase, bool wholeWord, LPCSTR strToFind,LPCSTR newString);
	void ReplaceAll(FINDREPLACE* fr, bool matchCase, bool wholeWord);
	void CreateFindDialog();
	void CreateFindReplaceDialog();
	void DestroyActiveDialog();
	void RescaleTables();
	void ResetFindInfo();
	void LiftUpTheDialogs();
	BOOL Print();
	~Document()
	{
	
	delete Page;
	this->pFileOpen->Release();
	this->FileSaveDialog->Release();
	pItem->Release();
	CoTaskMemFree(lpFilePath);
	CoUninitialize();
	}
//	void HideAll();
//	void ShowCurrentPage();
	void SetPageMargins(int pageNumber, double rightMargin = 3, double leftMargin = 2, double topMargin = 2, double bottomMargin = 0.5);
	const COMDLG_FILTERSPEC filters[1] =
	{
		{ L"RTF Files", L"*.rtf" },
	};
//	int GetPagesCount();
	//Отладка
//	void PrintContent();
};

