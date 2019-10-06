#include "Document.h"
#include "Document.h"

#define DEFINE_GUIDXXX(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID CDECL name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }


DEFINE_GUIDXXX(IID_ITextDocument2, 0xC241F5E0, 0x7206, 0x11D8, 0xA2, 0xC7,
	0x00, 0xA0, 0xD1, 0xD6, 0xC6, 0xB3);
DEFINE_GUIDXXX(IID_IUnknown, 0x00000000, 0x0000, 0x0000, 0xC0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
DEFINE_GUIDXXX(IID_ITextSelection2, 0xC241F5E1, 0x7206, 0x11D8, 0xA2, 0xC7,
	0x00, 0xA0, 0xD1, 0xD6, 0xC6, 0xB3);
string Document::findBuffer = "";
string Document::replaceBuffer = "";
FINDREPLACE Document::fr;
int FINDMSGSTRINGID;

string Document::GetSelectedText()
{
	string tmp = "";
	CHARRANGE cr;
	SendMessage(Page->hWnd, EM_EXGETSEL, 0, (LPARAM)& cr);
	TEXTRANGEW tr;
	if (cr.cpMin != cr.cpMax)
	{
		tr.chrg = cr;
		tr.lpstrText = new WCHAR[tr.chrg.cpMax - tr.chrg.cpMin + 1];
		SendMessageW(Page->hWnd, EM_GETTEXTRANGE, 0, (LPARAM)& tr);
		tmp = CW2A(tr.lpstrText);
		tmp = tmp.substr(0, tmp.find_first_of('\r'));
	}
	return tmp;
}

DWORD CALLBACK Document::WriteFileEditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	HANDLE hFile = (HANDLE)dwCookie;
	if (!WriteFile(hFile, pbBuff, cb, (DWORD*)pcb, NULL))
	{
		//handle errors
		return -1;
		// or perhaps return GetLastError();
	}
	return 0;
}

DWORD Document::ReadFileEditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	HANDLE hFile = (HANDLE)dwCookie;

	if (ReadFile(hFile, pbBuff, cb, (DWORD*)pcb, NULL))
	{
		/*cout << pbBuff << endl;*/
		return 0;
	}

	return -1;
}

void Document::Create(const int pagesCount)
{
	x = 0;
	y = 0;
	width = width;
	height = height;
	Page = new TextField(hInst, x, y, width, height, *(hwnd));
	WinAPIWindow::Attach(Page->hWnd, Page);
	SendMessage(Page->hWnd, EM_SETEVENTMASK, NULL, (LPARAM)(ENM_SELCHANGE | ENM_CHANGE));

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		hr = CoCreateInstance(CLSID_FileSaveDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileSaveDialog,
			reinterpret_cast<void**>(&FileSaveDialog));
	}
	GetOleInterface();
	GetITextDoc();
}

void Document::Show()
{
	UpdateWindow(Page->hWnd);
	ShowWindow(Page->hWnd, SW_SHOWNORMAL);
}

int Document::getX()
{
	return Page->getX();
}

int Document::getY()
{
	return Page->getY();
}

int Document::getHeight()
{	
	return Page->getHeight();
}

int Document::getWidth()
{
	return Page->getWidth();
}

void Document::setX(int x)
{
	this->x = x;
}

void Document::setY(int y)
{
	this->y = y;
}

void Document::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	/*this->width = width;
	this->height = height;*/
	Page->SetPosition(x, y);
	/*SetWindowPos(Page->hWnd, NULL, x, y, width, height, SWP_SHOWWINDOW);*/
}

void Document::SetMargins(double rightMargin, double leftMargin, double topMargin, double bottomMargin)
{
	Page->SetMargins(rightMargin, leftMargin, topMargin, bottomMargin);
}

void Document::OpenFile()
{
	HRESULT hr;
	pFileOpen->SetFolder(pItem);
	// Show the Open dialog box.
	hr = pFileOpen->Show(NULL);

	// Get the file name from the dialog box.
	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->GetResult(&pItem);
		if (SUCCEEDED(hr))
		{
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &lpFilePath);
			// Display the file name to the user.
			if (SUCCEEDED(hr))
			{
				HANDLE hFile = CreateFileW(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					cout << GetLastError() << endl;
					MessageBox(NULL, "Ошибка при сохранении файла!", "Ошибка!", MB_ICONERROR);
					return;
				}

				EDITSTREAM es;
				es.pfnCallback = ReadFileEditStreamCallback;
				es.dwCookie = (DWORD_PTR)hFile;
				SendMessage(Page->hWnd, EM_STREAMIN, SF_RTF, (LPARAM)& es);
				CloseHandle(hFile);
			}

		}
	}
	return;
}

TextField& Document::GetCurrentPage()
{
	// TODO: вставьте здесь оператор return
	return *Page;
}

void Document::InsertTable(int columns, int rows, int EditWidth)
{	
	DWORD startSelectionPos;
	SendMessage(Page->hWnd, EM_GETSEL, (WPARAM)& startSelectionPos, NULL);
	tablePositions.push_back(tableInfo(LOWORD(startSelectionPos), (columns * rows), rows));
	TABLEROWPARMS tbr;
	TABLECELLPARMS tbc;
	tbr.bTableLevel = 0;
	tbr.cbCell = sizeof(tbc);
	tbr.cbRow = sizeof(tbr);
	tbr.cCell = columns;
	tbr.cpStartRow = 1;
	tbr.cRow = rows;
	tbr.dxCellMargin = 0;
	tbr.dxIndent = 0;
	tbr.dyHeight = 250;
	tbr.fKeep = 0;
	tbr.fRTL = 0;
	tbr.fWrap = 0;
	tbr.fKeepFollow = 0;

	tbr.nAlignment = PFA_LEFT;

	tbc.crBackPat = RGB(255, 255, 255);

	tbc.crBrdrBottom = RGB(0, 0, 0);
	tbc.crBrdrLeft = RGB(0, 0, 0);
	tbc.crBrdrRight = RGB(0, 0, 0);
	tbc.crBrdrTop = RGB(0, 0, 0);

	tbc.crForePat = RGB(0, 0, 0);

	tbc.dxBrdrLeft = 1;
	tbc.dxWidth = (EditWidth / columns) * 15;
	tbc.dyBrdrBottom = 1;
	tbc.dyBrdrTop = 1;
	tbc.fMergeCont = 0;
	tbc.fMergePrev = 0;
	tbc.fMergeStart = 0;
	tbc.fMergeTop = 0;
	tbc.fVertical = 0;
	tbc.nVertAlign = 1;
	tbc.wShading = 0;
	HRESULT hr = SendMessage(Page->hWnd, EM_INSERTTABLE, WPARAM(&tbr), LPARAM(&tbc));
}

void Document::GetITextDoc()
{
	pIUnk->QueryInterface(IID_ITextDocument2, (void**)& iTextDocInterface);
}

BOOL Document::GetOleInterface()
{
	return SendMessage(Page->hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)& pIUnk);
}
void Document::Save()
{
	HRESULT hr;

	if (lpFilePath == NULL || lpFilePath[0] == '\0')
	{
		hr = FileSaveDialog->SetFileTypes(1, filters);
		FileSaveDialog->SetDefaultExtension(L"rtf");
		pFileOpen->SetFolder(pItem);
		hr = FileSaveDialog->Show(NULL);
		if (SUCCEEDED(hr))
		{
			hr = FileSaveDialog->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				/*LPWSTR pszFilePath;				*/
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &lpFilePath);
				if (SUCCEEDED(hr))
				{
					HANDLE hFile = CreateFileW(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						cout << GetLastError() << endl;
						MessageBox(NULL, "Ошибка при сохранении файла!", "Ошибка!", MB_ICONERROR);
						return;
					}
					EDITSTREAM es;
					es.pfnCallback = WriteFileEditStreamCallback;
					es.dwCookie = (DWORD_PTR)hFile;
					SendMessage(Page->hWnd, EM_STREAMOUT, SF_RTF, (LPARAM)& es);
					CloseHandle(hFile);
				}
			}
		}
	}
	else {
		HANDLE hFile = CreateFileW(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		EDITSTREAM es;
		es.pfnCallback = WriteFileEditStreamCallback;
		es.dwCookie = (DWORD_PTR)hFile;
		SendMessage(Page->hWnd, EM_STREAMOUT, SF_RTF, (LPARAM)& es);
		CloseHandle(hFile);
	}
}

void Document::SaveAs()
{
	HRESULT hr;

	if (lpFilePath == NULL || lpFilePath[0] == '\0')
	{
		hr = FileSaveDialog->SetFileTypes(1, filters);
		FileSaveDialog->SetDefaultExtension(L"rtf");
		pFileOpen->SetFolder(pItem);
		hr = FileSaveDialog->Show(NULL);
		if (SUCCEEDED(hr))
		{
			hr = FileSaveDialog->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				/*LPWSTR pszFilePath;				*/
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &lpFilePath);
				if (SUCCEEDED(hr))
				{
					HANDLE hFile = CreateFileW(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						cout << GetLastError() << endl;
						MessageBox(NULL, "Ошибка при сохранении файла!", "Ошибка!", MB_ICONERROR);
						return;
					}

					EDITSTREAM es;
					es.pfnCallback = WriteFileEditStreamCallback;
					es.dwCookie = (DWORD_PTR)hFile;
					SendMessage(Page->hWnd, EM_STREAMOUT, SF_RTF, (LPARAM)& es);
					CloseHandle(hFile);
				}
			}
		}
	}
}

void Document::CreateNew()
{
	HRESULT hr;
	hr = FileSaveDialog->SetFileTypes(1, filters);
	FileSaveDialog->SetDefaultExtension(L"rtf");
	FileSaveDialog->SetTitle(L"Создать документ");
	FileSaveDialog->SetOkButtonLabel(L"Создать");
	pFileOpen->SetFolder(pItem);
	hr = FileSaveDialog->Show(NULL);
	if (SUCCEEDED(hr))
	{
		hr = FileSaveDialog->GetResult(&pItem);
		if (SUCCEEDED(hr))
		{
			/*LPWSTR pszFilePath;				*/
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &lpFilePath);
			if (SUCCEEDED(hr))
			{
				HANDLE hFile = CreateFileW(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					cout << GetLastError() << endl;
					MessageBox(NULL, "Ошибка при сохранении файла!", "Ошибка!", MB_ICONERROR);
					return;
				}
				CloseHandle(hFile);
			}
		}
	}
}

void Document::Undo()
{
	if (iTextDocInterface != NULL)
		iTextDocInterface->Undo(1, NULL);
}

void Document::Redo()
{
	if (iTextDocInterface != NULL)
		iTextDocInterface->Redo(1, NULL);
}

void Document::SelectAll()
{
	SendMessage(Page->hWnd, EM_SETSEL, 0, -1);
}

void Document::Cut()
{
	HRESULT hr = iTextDocInterface->GetSelection2(&iTextSelection2);
	if (SUCCEEDED(hr))
		iTextSelection2->Cut(NULL);
}

void Document::DeleteText()
{
	HRESULT hr = iTextDocInterface->GetSelection2(&iTextSelection2);
	if (SUCCEEDED(hr))
		iTextSelection2->Delete(tomWord, 1, NULL);
}

void Document::Copy()
{
	HRESULT hr = iTextDocInterface->GetSelection2(&iTextSelection2);
	if (SUCCEEDED(hr))
		iTextSelection2->Copy(nullptr);
}

void Document::Paste()
{
	HRESULT hr = iTextDocInterface->GetSelection2(&iTextSelection2);
	if (SUCCEEDED(hr))
		iTextSelection2->Paste(nullptr, 0);
}

void Document::ChangeSelectedTextColor()
{
}
void Document::DestroyFindDialog()
{
	DestroyWindow(findHwnd);
	findHwnd = NULL;
}
void Document::DestroyFindReplaceDialog()
{
	DestroyWindow(findReplaceHwnd);
	findReplaceHwnd = NULL;
}
FINDTEXTEXW ftt;
bool prevDirection;
void Document::Find(bool direction, bool matchCase, bool wholeWord, LPCSTR strToFind)
{
	UINT flags = 0;
	if (direction)
	{
		flags |= FR_DOWN;
		if (strcmp(CStringA(this->strToFind), "") != 0 && strcmp(this->strToFind, strToFind) == 0 && (prevDirection == direction))
		{
		}
			/*findCr.cpMin += ftt.chrgText.cpMax - ftt.chrgText.cpMin;*/
		else if (direction != prevDirection)
			findCr.cpMin = ftt.chrgText.cpMax;
		else
		{
			findCr.cpMin = 0;
			findCr.cpMax = -1;
		}
		prevDirection = direction;
	}
	else
	{
		if (strcmp(CStringA(this->strToFind), "") != 0 && strcmp(this->strToFind, strToFind) == 0 && !(findCr.cpMin == 0 && findCr.cpMax == -1) && (prevDirection == direction))
		{
		}			
		else if (direction != prevDirection)
			findCr.cpMin = ftt.chrgText.cpMin;
		else if (findCr.cpMin == 0 && findCr.cpMax == -1)
		{
			GETTEXTLENGTHEX gtlex;
			gtlex.flags = GTL_DEFAULT;
			gtlex.flags = CP_ACP;
			int count = SendMessage(Page->hWnd, EM_GETTEXTLENGTHEX, (WPARAM)& gtlex, 0);
			if (count != E_INVALIDARG)
			{
				findCr.cpMin = count;
				findCr.cpMax = 0;
			}
			
		}
		else
		{
			GETTEXTLENGTHEX gtlex;
			gtlex.flags = GTL_DEFAULT;
			gtlex.flags = CP_ACP;
			int count = SendMessage(Page->hWnd, EM_GETTEXTLENGTHEX, (WPARAM)& gtlex, 0);
			if (count != E_INVALIDARG)
			{
				findCr.cpMin = count;
				findCr.cpMax = 0;
			}
		}
		prevDirection = direction;
	}
	if (matchCase) flags |= FR_MATCHCASE;
	if (wholeWord) flags |= FR_WHOLEWORD;
	memset(&ftt, 0, sizeof(ftt));
	ftt.chrg = findCr;
	ftt.lpstrText = CStringW(strToFind);
	this->strToFind = strToFind;
	int res = SendMessage(Page->hWnd, EM_FINDTEXTEXW, flags, (LPARAM)& ftt);
	if (res != -1)
	{
		findCr.cpMin = direction ? ftt.chrgText.cpMax : ftt.chrgText.cpMin;
		findCr.cpMax = -1;
		SendMessage(Page->hWnd, EM_EXSETSEL, 0, (LPARAM)& ftt.chrgText);
	}
	else
	{
		ftt.chrgText.cpMin = 0;
		ftt.chrgText.cpMax = 0;

		findCr.cpMin = 0;
		findCr.cpMax = -1;
		Find(direction, matchCase, wholeWord, strToFind);
	}
}

void Document::Replace(bool matchCase, bool wholeWord, LPCSTR strToFind, LPCSTR newString)
{

	CHARRANGE cr;
	SendMessage(Page->hWnd, EM_EXGETSEL, 0, (LPARAM)& cr);
	if (cr.cpMax != cr.cpMin)
		SendMessage(Page->hWnd, EM_REPLACESEL, TRUE, (LPARAM)newString);
	UINT flags = 0;
	flags |= FR_DOWN;
	if (strcmp(CStringA(this->strToFind), "") != 0 && strcmp(this->strToFind, strToFind) == 0)
		findCr.cpMin += ftt.chrgText.cpMax - ftt.chrgText.cpMin;
	else
	{
		findCr.cpMin = 0;
		findCr.cpMax = -1;
	}
	if (matchCase) flags |= FR_MATCHCASE;
	if (wholeWord) flags |= FR_WHOLEWORD;
	memset(&ftt, 0, sizeof(ftt));
	ftt.chrg = findCr;
	ftt.lpstrText = CStringW(strToFind);
	this->strToFind = strToFind;
	int res = SendMessage(Page->hWnd, EM_FINDTEXTEXW, flags, (LPARAM)& ftt);
	if (res != -1)
	{
		/*findCr.cpMin = ftt.chrgText.cpMax;*/
		findCr.cpMax = -1;
		SendMessage(Page->hWnd, EM_EXSETSEL, 0, (LPARAM)& ftt.chrgText);
	}
	else
	{
		ftt.chrgText.cpMin = 0;
		ftt.chrgText.cpMax = 0;

		findCr.cpMin = 0;
		findCr.cpMax = -1;
		MessageBox(findReplaceHwnd, "Поиск завершен!", "Заменить", MB_ICONINFORMATION);
	}
}

void Document::ReplaceAll(FINDREPLACE* fr, bool matchCase, bool wholeWord)
{
	int counter = 0;
	DWORD flags = FR_DOWN;
	flags |= matchCase ? FR_MATCHCASE : 0;
	flags |= wholeWord ? FR_WHOLEWORD : 0;
	FINDTEXTEXW ftt;
	memset(&ftt, 0, sizeof(ftt));
	ftt.chrg.cpMin = 0;
	ftt.chrg.cpMax = -1;
	CStringW strToFind = fr->lpstrFindWhat;
	ftt.lpstrText = strToFind;
	CHARRANGE cr = ftt.chrgText;
	int i = 0;

	while ((i = (SendMessage(Page->hWnd, EM_FINDTEXTEXW, flags, (LPARAM)& ftt))) != -1)
	{
		counter++;

		SendMessage(Page->hWnd, EM_EXSETSEL, 0, (LPARAM)& ftt.chrgText);
		SendMessage(Page->hWnd, EM_REPLACESEL, TRUE, (LPARAM)fr->lpstrReplaceWith);
	}
	MessageBox(findReplaceHwnd, string("Произведено замен: " + std::to_string(counter)).c_str(), "Заменить все", MB_ICONINFORMATION);
}

void Document::CreateFindDialog()
{
	if (findHwnd == NULL)
	{
		if (FINDMSGSTRINGID == 0)
			FINDMSGSTRINGID = RegisterWindowMessage(FINDMSGSTRING);
		findCr.cpMin = 0;
		findCr.cpMax = -1;
		memset(&fr, 0, sizeof(fr));
		memset(&findBuffer, 0, sizeof(findBuffer));
		findBuffer = GetSelectedText();

		if (findReplaceHwnd != NULL)
		{
			DestroyFindReplaceDialog();
		}

		fr.lStructSize = sizeof(fr);
		fr.hwndOwner = *main;
		fr.hInstance = (HINSTANCE)GetWindowLong(*(hwnd), GWL_HINSTANCE);
		fr.lpstrFindWhat = (char*)findBuffer.c_str();
		fr.wFindWhatLen = 2000;
		findHwnd = FindText(&fr);
	}
	else
	{
		MessageBox(*(hwnd), "Закройте окно поиска, прежде чем открывать новое!", "Поиск", MB_ICONWARNING);
	}
}

void Document::CreateFindReplaceDialog()
{
	if (findReplaceHwnd == NULL)
	{
		if (FINDMSGSTRINGID == 0)
			FINDMSGSTRINGID = RegisterWindowMessage(FINDMSGSTRING);
		memset(&fr, 0, sizeof(fr));
		findCr.cpMin = 0;
		findCr.cpMax = -1;
		
		memset(&findBuffer, 0, sizeof(findBuffer));
		memset(&replaceBuffer, 0, sizeof(replaceBuffer));
		findBuffer = GetSelectedText();
		if (findHwnd != NULL)
		{
			DestroyFindDialog();
		}

		fr.lStructSize = sizeof(fr);
		fr.hwndOwner = *main;
		fr.hInstance = (HINSTANCE)GetWindowLong(*(hwnd), GWL_HINSTANCE);
		fr.lpstrFindWhat = (char*)findBuffer.c_str();
		fr.wFindWhatLen = 2000;
		fr.lpstrReplaceWith = (char*)replaceBuffer.c_str();
		fr.wReplaceWithLen = 2000;
		findReplaceHwnd = ReplaceText(&fr);
	}
	else
	{
		MessageBox(*(hwnd), "Закройте окно поиска, прежде чем открывать новое!", "Поиск", MB_ICONWARNING);
	}
}
void Document::DestroyActiveDialog()
{
	if (findHwnd != NULL)
	{
		DestroyWindow(findHwnd);
		findHwnd = NULL;
	}
	if (findReplaceHwnd != NULL)
	{
		DestroyWindow(findReplaceHwnd);
		findReplaceHwnd = NULL;
	}

}
TABLEROWPARMS tbr1;
TABLECELLPARMS tbc1;
void Document::RescaleTables()
{
	iTextDocInterface->Undo(tomSuspend, NULL);
	for (size_t i = 0; i < tablePositions.size(); i++)
	{
		memset(&tbc1, 0, sizeof(TABLECELLPARMS));
		memset(&tbr1, 0, sizeof(TABLEROWPARMS));
		tbr1.cbRow = sizeof(TABLEROWPARMS);
		tbr1.cbCell = sizeof(TABLECELLPARMS);
		tbr1.cpStartRow = tablePositions[i].position;
		tbr1.cCell = tablePositions[i].cCell;
		HRESULT hr = SendMessage(Page->hWnd, EM_GETTABLEPARMS, WPARAM(&tbr1), LPARAM(&tbc1));
		if (hr == E_FAIL && !tablePositions.empty())
			tablePositions.erase(tablePositions.begin() + i);
		else
		{
			tbr1.cRow = -1;
			tbc1.dxWidth = (Page->getWidth() / (tablePositions[i].cCell / tablePositions[i].cRow)) * 15;
			SendMessage(Page->hWnd, EM_SETTABLEPARMS, WPARAM(&tbr1), LPARAM(&tbc1));
		}
	}
	iTextDocInterface->Undo(tomResume, NULL);
}
void Document::ResetFindInfo()
{
	findCr.cpMin = 0;
	findCr.cpMax = -1;
}
void Document::LiftUpTheDialogs()
{
	if (findHwnd != NULL)
		SetWindowPos(findHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (findReplaceHwnd != NULL)
		SetWindowPos(findReplaceHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
