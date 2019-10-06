#include "MainWindow.h"
#define MAXHEIGHT 32768
constexpr int colorArraySize = 31;
COLORREF colors[colorArraySize];
HBRUSH brushes[colorArraySize];
std::map<int, COLORREF> ButtonColors;
COLORREF selectedColor;
COLORREF ccref[16] =
{
	RGB(255,255,255),
	RGB(255,255,0),
	RGB(255,0,255),
	RGB(0,255,255),
	RGB(0,0,255),
	RGB(0,255,0),
	RGB(255,0,0),
	RGB(0,0,0),
	RGB(142,255,111),
	RGB(31,185,145),
	RGB(255,148,11),
	RGB(255,255,255),
	RGB(255,64,49),
	RGB(84,255,243),
	RGB(231,222,64),
	RGB(11,90,255),

};
extern int FINDMSGSTRINGID;
//Constructor
MainWindow::MainWindow(HINSTANCE hInst)
{
	AppHinstance = hInst;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Load the image 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(L"images\\AppIcon.png");

	image->GetHBITMAP(Color(), &AppIcon);

	image->GetHICON(&taskBarIcon);
	// do something with your image 
	// ... 

	// delete the image when done 
	delete image; image = 0;


	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(gdiplusToken);

	wc.lpszClassName = ClassName.c_str();
	wc.hbrBackground = CreateSolidBrush(RGB(166, 202, 240));
	wc.style = CS_DBLCLKS;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = taskBarIcon;
	wc.hInstance = hInst;
	wc.lpfnWndProc = sWndProc;

	RegisterClass(&wc);

	//�������� ������� ������� ������� ������(��� ������ �����)
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
	setY(CW_USEDEFAULT);
	setX(CW_USEDEFAULT);
	setWidth(workArea.right);
	setHeight(workArea.bottom);
	WindowSizingType = WindowSizingTypes::EXPANDED;
	xWindowBM = 0;
	yWindowBM = 0;
	wWindowBM = 1200;
	hWindowBM = 800;
	hWnd = CreateWindow(ClassName.c_str(), Title.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_MAXIMIZE, x, y, getWidth(), getHeight(), NULL, LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1)), hInst, NULL);
	CreateObjects();
	LoadColors();
	doc->SetMargins(3, 2, 0.2, 0.2);
	HDC hdc = GetDC(hWnd);
	Fonts::fcb = &fcb;
	menuBar.fcb = &fcb;
	Fonts::GetFonts(hdc);
	fcb.SetCurSel(102);
	fzcb.SetCurSel(2);
	SetFocus(doc->GetCurrentPage().hWnd);
	doc->GetCurrentPage().SetSelection(0, 1);
	doc->GetCurrentPage().SetSelection(0, 0);
}
BOOL MainWindow::InsertTableDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int minvalue1 = 2, maxvalue1 = 25, minvalue2 = 2, maxvalue2 = 25;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		auto spin1 = GetDlgItem(hwndDlg, IDC_SPIN1);
		auto spin2 = GetDlgItem(hwndDlg, IDC_SPIN2);
		ToolTip::Create(IDC_SPIN1, hwndDlg, "������� ����� ����� �� 2 �� 25");
		ToolTip::Create(IDC_EDIT1, hwndDlg, "������� ����� ����� �� 2 �� 25");
		ToolTip::Create(IDC_SPIN2, hwndDlg, "������� ����� ����� �� 2 �� 25");
		ToolTip::Create(IDC_EDIT2, hwndDlg, "������� ����� ����� �� 2 �� 25");
		SendMessage(spin1, UDM_SETPOS, 0, 2);
		SendMessage(spin2, UDM_SETPOS, 0, 2);
		SendMessage(spin1, UDM_SETRANGE, 0, (LPARAM)MAKELPARAM(25, 2));
		SendMessage(spin2, UDM_SETRANGE, 0, (LPARAM)MAKELPARAM(25, 2));
		return TRUE;
		break;
	}
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
		{
			auto editHwnd = (HWND)lParam;
			auto editId = LOWORD(wParam);
			int textlen = GetWindowTextLength(editHwnd);
			string text;
			TCHAR buf[256];
			memset(buf, 0, sizeof(buf));
			SendMessage(editHwnd, WM_GETTEXT, textlen, (LPARAM)buf);
			text.append(buf);
			/*GetWindowText(editHwnd, const_cast<char*>(text.c_str()), textlen);*/
			if (!text.empty())
			{
				int value = std::stoi(text);
				switch (editId)
				{
				case IDC_EDIT1:
				{
					auto spin = GetDlgItem(hwndDlg, IDC_SPIN1);
					value = value > maxvalue1 ? maxvalue1 : value < minvalue1 && value != 1 ? minvalue1 : value;
					/*memset(text, 0, sizeof(text));*/
					text = to_string(value);
					SetWindowText(editHwnd, text.c_str());
					SendMessage(editHwnd, EM_SETSEL, (WPARAM)textlen, (LPARAM)textlen);
					break;
				}
				case IDC_EDIT2:
				{
					auto spin = GetDlgItem(hwndDlg, IDC_SPIN1);
					value = value > maxvalue2 ? maxvalue2 : value < minvalue2 && value != 1 ? minvalue2 : value;
					/*memset(text, 0, sizeof(text));*/
					text = _strdup(to_string(value).c_str());
					SetWindowText(editHwnd, text.c_str());
					SendMessage(editHwnd, EM_SETSEL, (WPARAM)textlen, (LPARAM)textlen);
					break;
				}
				}
			}

			break;
		}
		default:
			break;
		}
		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			int tableColumns = GetDlgItemInt(hwndDlg, IDC_EDIT1, NULL, FALSE);
			int tableRows = GetDlgItemInt(hwndDlg, IDC_EDIT2, NULL, FALSE);
			if (tableRows < minvalue2 || tableRows > maxvalue2 || tableColumns < minvalue1 || tableColumns > maxvalue1)
			{
				MessageBox(hwndDlg, "������������ ���������� �����/��������!", "������", MB_ICONERROR);
				return FALSE;
			}
			SendMessage(GetParent(hwndDlg), ID_TABLEPARAMS, 0, (LPARAM)MAKELPARAM(tableColumns, tableRows));
			EndDialog(hwndDlg, IDOK);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}
	default:
		return FALSE;
		break;
	}
}
BOOL MainWindow::MyColorProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int colorIndex;
	static bool b;
	switch (message)
	{
	case WM_INITDIALOG:
		colorIndex = 0;
		b = false;
		break;
	case WM_CTLCOLORBTN:
	{
		if (GetDlgCtrlID((HWND)lParam) != IDC_AUTOCOLOR)
		{
			b = !b;
			if (b == true && colorIndex != 31)
				ButtonColors[GetDlgCtrlID((HWND)lParam)] = colors[colorIndex];
			return (INT_PTR)brushes[colorIndex > colorArraySize - 1 ? 0 : b ? colorIndex++ : colorIndex]; // a red brush created earlier
		}
		else
			return DKGRAY_BRUSH;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		case IDC_AUTOCOLOR:
			selectedColor = RGB(0, 0, 0);
			EndDialog(hwndDlg, IDOK);
			break;
		case IDC_OTHERCOLOR:
			CHOOSECOLOR cc;
			memset(&cc, 0, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = hwndDlg;
			cc.lpCustColors = ccref;
			if (ChooseColor(&cc) == TRUE)
			{
				selectedColor = cc.rgbResult;
				EndDialog(hwndDlg, IDOK);
			}
			else return FALSE;
			break;
		default:
		{
			selectedColor = ButtonColors[LOWORD(wParam)];
			EndDialog(hwndDlg, IDOK);
			return TRUE;
		}
		}
	}
	default:
		return FALSE;
	}

}
void MainWindow::LoadColors()
{
	colors[0] = RGB(255, 0, 0);
	colors[1] = RGB(255, 255, 255);
	colors[2] = RGB(255, 0, 0);
	colors[3] = RGB(0, 255, 255);
	colors[4] = RGB(255, 0, 255);
	colors[5] = RGB(128, 12, 34);
	colors[6] = RGB(224, 64, 172);
	colors[7] = RGB(32, 180, 25);
	colors[8] = RGB(31, 255, 85);
	colors[9] = RGB(132, 192, 84);
	colors[10] = RGB(58, 168, 32);
	colors[11] = RGB(31, 128, 95);
	colors[12] = RGB(194, 64, 25);
	colors[13] = RGB(192, 11, 92);
	colors[14] = RGB(168, 255, 32);
	colors[15] = RGB(128, 94, 159);
	colors[16] = RGB(64, 111, 255);
	colors[17] = RGB(31, 164, 235);
	colors[18] = RGB(168, 45, 64);
	colors[19] = RGB(255, 192, 123);
	colors[20] = RGB(111, 222, 38);
	colors[21] = RGB(184, 32, 11);
	colors[22] = RGB(38, 153, 93);
	colors[23] = RGB(55, 184, 38);
	colors[24] = RGB(132, 25, 11);
	colors[25] = RGB(12, 255, 255);
	colors[26] = RGB(64, 128, 255);
	colors[27] = RGB(44, 232, 10);
	colors[28] = RGB(11, 22, 33);
	colors[29] = RGB(128, 138, 232);
	colors[30] = RGB(138, 242, 84);
	for (size_t i = 0; i < colorArraySize; i++)
	{
		brushes[i] = CreateSolidBrush(colors[i]);
	}
}
void MainWindow::CreateObjects()
{
	HFONT hFont = CreateFont(18, 7, 0, 0, FW_ULTRALIGHT, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_MODERN, "Segoe UI");
	Attach(hWnd, this);
	clientArea = ClientArea(0, 124, getWidth(), getHeight() - 124, hWnd);
	Attach(clientArea.hWnd, &clientArea);
	doc = new Document(&this->clientArea.hWnd, &hWnd, (HINSTANCE)GetWindowLong(this->hWnd, GWL_HINSTANCE), clientArea.getWidth(), clientArea.getHeight());
	doc->Create(1);

	titleBrush = CreateSolidBrush(RGB(237, 239, 238));
	menuBar = MenuBar(0, 0, getWidth() - 2, 125, hWnd);
	Attach(menuBar.hWnd, &menuBar);
	bb = MenuButton(130, 48, 24, 24, menuBar.hWnd, hWnd, L"images/bold.png", ID_BOLDBUTTON, ID_SETBOLD, "������� ���������� �����", "���������� (CTRL + B)");
	Attach(bb.hWnd, &bb);
	sb = MenuButton(160, 48, 24, 24, menuBar.hWnd, hWnd, L"images/strikefont.png", ID_STRIKEBUTTON, ID_SETSTRIKE, "���������� �����", "����������� (CTRL + 5)");
	Attach(sb.hWnd, &sb);
	ib = MenuButton(190, 48, 24, 24, menuBar.hWnd, hWnd, L"images/italic.png", ID_ITALICBUTTON, ID_SETITALIC, "������� ��������� �����", "��������� (CTRL + I)");
	Attach(ib.hWnd, &ib);
	ufb = MenuButton(220, 48, 24, 24, menuBar.hWnd, hWnd, L"images/underlined.png", ID_UNDERLINEBUTTON, ID_SETUNDERLINE, "����������� �����", "������������ (CTRL + U)");
	Attach(ufb.hWnd, &ufb);
	uib = MenuButton(250, 48, 24, 24, menuBar.hWnd, hWnd, L"images/upIndex.png", ID_UPINDEX, ID_SETUPINDEX, "������� ����� ������ � ����, ��� ������", "�����������");
	Attach(uib.hWnd, &uib);
	dib = MenuButton(280, 48, 24, 24, menuBar.hWnd, hWnd, L"images/downIndex.png", ID_DOWNINDEX, ID_SETDOWNINDEX, "������� ����� ������ � ����, ��� ������", "�����������");
	Attach(dib.hWnd, &dib);
	leftAlignButton = MenuButton(450, 48, 24, 24, menuBar.hWnd, hWnd, L"images/alignLeft.png", ID_LEFTALIGN, ID_SETLEFTALIGN, "������������ ������ �� ������ ����", "��������� ����� �� ������ ���� (CTRL + L)");
	Attach(leftAlignButton.hWnd, &leftAlignButton);
	centerAlignButton = MenuButton(480, 48, 24, 24, menuBar.hWnd, hWnd, L"images/alignCenter.png", ID_CENTERALIGN, ID_SETCENTERALIGN, "������������ ������ �� ������", "��������� ����� �� ������ (CTRL + E)");
	Attach(centerAlignButton.hWnd, &centerAlignButton);
	rightAlignButton = MenuButton(510, 48, 24, 24, menuBar.hWnd, hWnd, L"images/alignRight.png", ID_RIGHTALIGN, ID_SETRIGHTALIGN, "������������ ������ �� ������� ����", "��������� ����� �� ������� ���� (CTRL + R)");
	Attach(rightAlignButton.hWnd, &rightAlignButton);
	justifyButton = MenuButton(540, 48, 24, 24, menuBar.hWnd, hWnd, L"images/justify.png", ID_JUSTIFY, ID_SETJUSTIFY, "������������ ������ �� ������", "��������� ����� �� ������");
	Attach(justifyButton.hWnd, &justifyButton);
	fcb = ComboBox<HFONT>(130, 16, 200, 350, menuBar.hWnd, ID_FONTCOMBOBOX);
	Attach(fcb.hWnd, &fcb);

	fzcb = ComboBox<int>(330, 16, 60, 350, menuBar.hWnd, ID_FONTSIZECOMBOBOX);
	Attach(fzcb.hWnd, &fzcb);
	InsertTableButton = MenuButton2(450, 16, 24, 24, menuBar.hWnd, hWnd, L"images/inserttable.png", ID_INSERTTABLEBUTTON, ID_INSERTTABLE, "�������� �������", "�������� �������");
	Attach(InsertTableButton.hWnd, &InsertTableButton);
	ColorButton = MenuButton2(480, 16, 24, 24, menuBar.hWnd, hWnd, L"images/letter.png", ID_COLORBUTTON, ID_SETTEXTCOLOR, "�������� ���� ������", "�������� ����");
	Attach(ColorButton.hWnd, &ColorButton);
	leftButton = MenuButton2(25, 15, 24, 24, menuBar.hWnd, hWnd, L"images/left-arrow.png", ID_UNDO, ID_UNDO, "������ ���������� ��������", "�������� (CTRL + Z)");
	Attach(leftButton.hWnd, &leftButton);
	rightButton = MenuButton2(55, 15, 24, 24, menuBar.hWnd, hWnd, L"images/right-arrow.png", ID_REDO, ID_REDO, "��������� ���������� ���������� ��������", "������� (CTRL + Y)");
	Attach(rightButton.hWnd, &rightButton);
	copyButton = MenuButton2(25, 45, 24, 24, menuBar.hWnd, hWnd, L"images/copy.png", ID_COPY, ID_COPY, "����������� ����������� ����������� ��������� � ����� ������", "���������� (CTRL + C)");
	Attach(copyButton.hWnd, &copyButton);
	cutButton = MenuButton2(55, 45, 24, 24, menuBar.hWnd, hWnd, L"images/cut.png", ID_CUT, ID_CUT, "����������� ����������� ����������� ��������� � ����� ������", "�������� (CTRL + X)");
	Attach(cutButton.hWnd, &cutButton);
	saveButton = MenuButton2(25, 75, 24, 24, menuBar.hWnd, hWnd, L"images/floppy.png", ID_SAVE, ID_SAVEFILE, "���������� ���������", "��������� (CTRL + S)");
	Attach(saveButton.hWnd, &saveButton);
	pasteButton = MenuButton2(55, 75, 24, 24, menuBar.hWnd, hWnd, L"images/paste.png", ID_PASTE, ID_PASTE, "�������� ���������� �� ������ ������", "�������� (CTRL + V)");
	Attach(pasteButton.hWnd, &pasteButton);
	findButton = new MenuButton2(590, 10, 24, 24, menuBar.hWnd, hWnd, L"images/find.png", ID_SEARCHBUTTON, ID_SEARCHBUTTON, "�����", "����� ������ � ���������");
	findReplaceButton = new MenuButton2(590, 40, 24, 24, menuBar.hWnd, hWnd, L"images/change.png", ID_SEARCHREPLACEBUTTON, ID_SEARCHREPLACEBUTTON, "����� � ��������", "������ ������ � ���������");
	selectAllButton = new MenuButton2(590, 70, 24, 24, menuBar.hWnd, hWnd, L"images/select.png", ID_SELECTALLBUTTON, ID_SELECTALLBUTTON, "�������� ���", "�������� ���� ����� � ���������");
	Attach(findButton->hWnd, findButton);
	Attach(findReplaceButton->hWnd, findReplaceButton);
	Attach(selectAllButton->hWnd, selectAllButton);
	for (auto object : WindowsMap)
	{
		object.second->Show();
		object.second->Update();
	}
	/*findButton.lbl->SetFont(hFont);
	findChangeButton.lbl->SetFont(hFont);
	selectAllButton.lbl->SetFont(hFont);*/
	DeleteObject(hFont);
	doc->Show();
	font = CreateFont(16,
		6, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, "Calibri");
	fzcb.SetFont(font);
	fzcb.Init();
	DeleteObject(font);
}
void MainWindow::ReplaceItems()
{
	//if window was maximized
	if (WindowSizingType == WindowSizingTypes::WINDOW_COLLAPSED)
	{

		//save current coords of textfield in variables
		xBeforeMaximized = doc->getX();
		yBeforeMaximized = doc->getY();
		doc->SetPosition(0, doc->getY());
		menuBar.SetPosition(getX(), getY() + 24);
		clientArea.SetPosition(0, 148);
	}
	//if window was restored
	else if (WindowSizingType == WindowSizingTypes::EXPANDED)
	{
		doc->SetPosition(0, doc->getY());
		menuBar.SetPosition(getX(), getY() + 24);
		clientArea.SetPosition(0, 148);
	}
}
void MainWindow::Collapse(CollapseMode collapseMode)
{
	switch (collapseMode)
	{
	case CollapseMode::COLLAPSE_IN_WINDOW:
		WindowSizingType = WindowSizingTypes::WINDOW_COLLAPSED;
		SetPosition(0, 0);
		SetWidthAndHeight(wWindowBM, hWindowBM);
		menuBar.SetPosition(getX(), getY() + 25);
		menuBar.SetWidthAndHeight(getWidth(), menuBar.getHeight());
		bb.SetPosition(bb.getX(), bb.getY());
		ib.SetPosition(ib.getX(), ib.getY());
		ufb.SetPosition(ufb.getX(), ufb.getY());
		clientArea.SetPosition(0, 148);
		clientArea.SetWidthAndHeight(getWidth(), getHeight() - 148);
		doc->SetPosition(0, doc->getY());
		doc->GetCurrentPage().RescaleWidthWithMargins();
		doc->RescaleTables();
		doc->LiftUpTheDialogs();
		break;
	case CollapseMode::FULL_COLLAPSE:
		WindowSizingType = WindowSizingTypes::COLLAPSED;
		SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		break;
	}
}
void MainWindow::Expand()
{
	WindowSizingType = WindowSizingTypes::EXPANDED;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

	RECT wndRect;
	GetWindowRect(hWnd, &wndRect);

	xWindowBM = wndRect.left;
	yWindowBM = wndRect.top;

	wWindowBM = wndRect.right - xWindowBM;
	hWindowBM = wndRect.bottom - yWindowBM;

	SetPosition(workArea.left, workArea.top);
	SetWidthAndHeight(workArea.right, workArea.bottom);
	menuBar.SetPosition(getX(), getY() + 25);
	menuBar.SetWidthAndHeight(getWidth(), menuBar.getHeight());
	bb.SetPosition(bb.getX(), bb.getY());
	ib.SetPosition(ib.getX(), ib.getY());
	ufb.SetPosition(ufb.getX(), ufb.getY());
	clientArea.SetPosition(0, 148);
	clientArea.SetWidthAndHeight(getWidth(), getHeight() - 148);
	doc->SetPosition(0, doc->getY());
	doc->GetCurrentPage().RescaleWidthWithMargins();
	doc->RescaleTables();
	doc->LiftUpTheDialogs();

	Redraw(NULL);
	menuBar.Redraw(NULL);
	clientArea.Redraw(NULL);
}
//void MainWindow::RescalePositions()
//{
//	if (WindowSizingType == WindowSizingTypes::WINDOW_COLLAPSED)
//	{
//		Expand();
//		
//	}
//	else if (WindowSizingType == WindowSizingTypes::EXPANDED)
//	{
//		Collapse();
//		/*SetPosition(0, 0);
//		cb.SetPosition(getWidth() - 25, cb.getY());
//		eb.SetPosition(getWidth() - 55, cb.getY());
//		mb.SetPosition(getWidth() - 85, mb.getY());
//		menuBar.SetPosition(getX(), getY() + 25);
//		bb.SetPosition(bb.getX(), bb.getY());
//		ib.SetPosition(ib.getX(), ib.getY());
//		ufb.SetPosition(ufb.getX(), ufb.getY());
//		clientArea.SetPosition(0, 148);
//		doc->SetPosition(0, doc->getY());
//		doc->GetCurrentPage().RescaleWidthWithMargins();
//		doc->RescaleTables();
//		doc->LiftUpTheDialogs();*/
//	}
//}

void MainWindow::CheckAlignment()
{
	bool leftAlignStatus = pf2.wAlignment == PFA_LEFT;
	bool rightAlignStatus = pf2.wAlignment == PFA_RIGHT;
	bool centerAlignStatus = pf2.wAlignment == PFA_CENTER;
	bool justifyAlignStatus = pf2.wAlignment == PFA_JUSTIFY;

	SendMessage(leftAlignButton.hWnd, CHANGESTATE, 1, LPARAM(leftAlignStatus));
	SendMessage(centerAlignButton.hWnd, CHANGESTATE, 1, LPARAM(centerAlignStatus));
	SendMessage(rightAlignButton.hWnd, CHANGESTATE, 1, LPARAM(rightAlignStatus));
	SendMessage(justifyButton.hWnd, CHANGESTATE, 1, LPARAM(justifyAlignStatus));
}

LRESULT MainWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static LPNMHDR nmhdr;
	static REQRESIZE* rqrsz;
	static POINT mousePos, currentMousePos;
	static LPFINDREPLACE lpfr;
	if (msg == FINDMSGSTRINGID)
	{
		lpfr = (LPFINDREPLACE)lParam;
		bool direction = lpfr->Flags & FR_DOWN ? true : false;
		bool findNext = lpfr->Flags & FR_FINDNEXT ? true : false;
		bool matchCase = lpfr->Flags & FR_MATCHCASE ? true : false;
		bool wholeWord = lpfr->Flags & FR_WHOLEWORD ? true : false;
		bool dialogTermination = lpfr->Flags & FR_DIALOGTERM ? true : false;
		bool replace = lpfr->Flags & FR_REPLACE ? true : false;
		bool replaceAll = lpfr->Flags & FR_REPLACEALL ? true : false;

		if (findNext && lpfr->lpstrFindWhat[0] != '\0' && replace == false)
			doc->Find(direction, matchCase, wholeWord, lpfr->lpstrFindWhat);
		else if (replace == true && findNext == false)
			doc->Replace(matchCase, wholeWord, lpfr->lpstrFindWhat, lpfr->lpstrReplaceWith);
		else if (replaceAll == true)
			doc->ReplaceAll(lpfr, matchCase, wholeWord);
		if (dialogTermination)
			doc->DestroyActiveDialog();

	}
	else
		switch (msg)
		{

			break;
		case WM_NOTIFY:
		{
			nmhdr = (LPNMHDR)lParam;
			rqrsz = (REQRESIZE*)lParam;
			switch (nmhdr->code)
			{
				//Handle caret moving
			case EN_SELCHANGE:
			{
				SELCHANGE* pSelChange = (SELCHANGE*)lParam;
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				memset(&pf2, 0, sizeof(PARAFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				pf2.cbSize = sizeof(PARAFORMAT2);
				SendMessage(nmhdr->hwndFrom, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SendMessage(nmhdr->hwndFrom, EM_GETPARAFORMAT, 0, (LPARAM)&pf2);
				bool boldStatus = cf2.dwEffects & CFE_BOLD;
				bool strikeStatus = cf2.dwEffects & CFE_STRIKEOUT;
				bool italicStatus = cf2.dwEffects & CFE_ITALIC;
				bool underlineStatus = cf2.dwEffects & CFE_UNDERLINE;
				bool upIndexStatus = cf2.yOffset > 0;
				bool downIndexStatus = cf2.yOffset < 0;
				int fontHeight = cf2.yHeight;
				char fontname[LF_FACESIZE];
				strcpy_s(fontname, cf2.szFaceName);
				WinAPIWindow::sWndProc(fcb.hWnd, CHANGESTATE, (WPARAM)ID_FONTCOMBOBOX, (LPARAM)((LPCSTR)fontname));
				WinAPIWindow::sWndProc(fzcb.hWnd, CHANGESTATE, (WPARAM)ID_FONTSIZECOMBOBOX, (LPARAM)(fontHeight));
				SendMessage(this->fzcb.hWnd, CHANGESTATE, 1, LPARAM(true));
				SendMessage(this->fcb.hWnd, CHANGESTATE, 1, LPARAM(fontHeight));
				SendMessage(bb.hWnd, CHANGESTATE, 1, LPARAM(boldStatus));
				SendMessage(sb.hWnd, CHANGESTATE, 1, LPARAM(strikeStatus));
				SendMessage(ib.hWnd, CHANGESTATE, 1, LPARAM(italicStatus));
				SendMessage(ufb.hWnd, CHANGESTATE, 1, LPARAM(underlineStatus));
				SendMessage(uib.hWnd, CHANGESTATE, 1, LPARAM(upIndexStatus));
				SendMessage(dib.hWnd, CHANGESTATE, 1, LPARAM(downIndexStatus));
				CheckAlignment();
			}; break;
			}
		}; break;
		case ID_SETBOLD:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.dwEffects ^= CFE_BOLD;
			cf2.dwMask |= CFM_BOLD;
			SendMessage(doc->GetCurrentPage().hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETSTRIKE:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.dwEffects ^= CFE_STRIKEOUT;
			cf2.dwMask |= CFM_STRIKEOUT;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETITALIC:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.dwEffects ^= CFE_ITALIC;
			cf2.dwMask |= CFM_ITALIC;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETUNDERLINE:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.dwEffects ^= CFE_UNDERLINE;
			cf2.dwMask |= CFM_UNDERLINE;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETUPINDEX:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.yOffset = cf2.yOffset > 0 ? 0 : cf2.yHeight / 2;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETDOWNINDEX:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.yOffset = cf2.yOffset < 0 ? 0 : -cf2.yHeight / 2;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
		}; break;
		case ID_SETFONTSIZE:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			cf2.yHeight = (int)lParam * 20;
			cf2.dwMask = CFM_SIZE;
			int i = SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
			break;
		}
		case ID_SETFONTNAME:
		{
			auto& currentPage = doc->GetCurrentPage();
			LPCTSTR FontName = (LPCTSTR)lParam;
			memset(&cf2, 0, sizeof(CHARFORMAT2));
			cf2.cbSize = sizeof(CHARFORMAT2);

			SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			strcpy_s(cf2.szFaceName, FontName);
			cf2.dwMask = CFM_FACE;
			SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
			SetFocus(currentPage.hWnd);
			break;
		}
		case ID_SETLEFTALIGN:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&pf2, 0, sizeof(PARAFORMAT2));
			pf2.cbSize = sizeof(PARAFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf2);
			pf2.dwMask = PFM_ALIGNMENT;
			pf2.wAlignment = PFA_LEFT;
			SendMessage(currentPage.hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
			SetFocus(currentPage.hWnd);
			CheckAlignment();
		}; break;
		case ID_SETRIGHTALIGN:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&pf2, 0, sizeof(PARAFORMAT2));
			pf2.cbSize = sizeof(PARAFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf2);
			pf2.dwMask = PFM_ALIGNMENT;
			pf2.wAlignment = pf2.wAlignment == PFA_RIGHT ? PFA_LEFT : PFA_RIGHT;
			SendMessage(currentPage.hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
			SetFocus(currentPage.hWnd);
			CheckAlignment();
		}; break;
		case ID_SETCENTERALIGN:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&pf2, 0, sizeof(PARAFORMAT2));
			pf2.cbSize = sizeof(PARAFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf2);
			pf2.dwMask = PFM_ALIGNMENT;
			pf2.wAlignment = pf2.wAlignment == PFA_CENTER ? PFA_LEFT : PFA_CENTER;
			SendMessage(currentPage.hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
			SetFocus(currentPage.hWnd);
			CheckAlignment();
		}; break;
		case ID_SETJUSTIFY:
		{
			auto& currentPage = doc->GetCurrentPage();
			memset(&pf2, 0, sizeof(PARAFORMAT2));
			pf2.cbSize = sizeof(PARAFORMAT2);
			SendMessage(currentPage.hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf2);
			pf2.dwMask = PFM_ALIGNMENT;
			pf2.wAlignment = pf2.wAlignment == PFA_JUSTIFY ? PFA_LEFT : PFA_JUSTIFY;
			SendMessage(currentPage.hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
			SetFocus(currentPage.hWnd);
			CheckAlignment();
			break;
		}
		case WM_RESTOREFOCUS:
		{
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		}
		case ID_SAVEFILE:
			doc->Save();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_OPEN:
			doc->OpenFile();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_SAVEAS:
			doc->SaveAs();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_CREATENEW:
			doc->CreateNew();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_UNDO:
			doc->Undo();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_REDO:
			doc->Redo();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_CUT:
			doc->Cut();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_COPY:
			doc->Copy();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_PASTE:
			doc->Paste();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_DELETE:
			doc->DeleteText();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_SELECTALL:
			doc->SelectAll();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_SELECTALLBUTTON:
			doc->SelectAll();
			SetFocus(doc->GetCurrentPage().hWnd);
			break;
		case ID_SEARCHBUTTON:
			doc->CreateFindDialog();
			break;
		case ID_SEARCHREPLACEBUTTON:
			doc->CreateFindReplaceDialog();
			break;
		case ID_INSERTTABLE:
		{
			if (DialogBox(AppHinstance,
				MAKEINTRESOURCE(IDD_DIALOG1),
				this->hWnd,
				(DLGPROC)InsertTableDlgProc) == IDOK)
			{

				doc->InsertTable(tableColumns, tableRows, doc->getWidth());
				SetFocus(doc->GetCurrentPage().hWnd);

			}
			break;
		}
		case ID_SETTEXTCOLOR:
		{
			if (DialogBox(AppHinstance,
				MAKEINTRESOURCE(IDD_DIALOG2),
				this->hWnd,
				(DLGPROC)MyColorProc) == IDOK)
			{
				auto& currentPage = doc->GetCurrentPage();
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				cf2.dwEffects = 0;
				cf2.dwMask = CFM_COLOR;
				cf2.crTextColor = selectedColor;
				/*cf2.crBackColor = selectedColor;*/
				SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SetFocus(currentPage.hWnd);
			}
			break;
		}
		case ID_TABLEPARAMS:
		{
			tableColumns = LOWORD(lParam);
			tableRows = HIWORD(lParam);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}; break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps), MemDCExercising;
			EndPaint(hWnd, &ps);
		}
		break;
		//Button Handling
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				// Process the accelerator and menu commands. 
			case IDA_BOLD:
			{
				auto& currentPage = doc->GetCurrentPage();
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				cf2.dwEffects ^= CFE_BOLD;
				cf2.dwMask |= CFM_BOLD;
				SendMessage(doc->GetCurrentPage().hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SetFocus(currentPage.hWnd);
				break;
			}

			case IDA_ITALIC:
			{
				auto& currentPage = doc->GetCurrentPage();
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				cf2.dwEffects ^= CFE_ITALIC;
				cf2.dwMask |= CFM_ITALIC;
				SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SetFocus(currentPage.hWnd);
				break;
			}
			case IDA_UNDERSTRIKE:
			{
				auto& currentPage = doc->GetCurrentPage();
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				cf2.dwEffects ^= CFE_UNDERLINE;
				cf2.dwMask |= CFM_UNDERLINE;
				SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SetFocus(currentPage.hWnd);
				break;
			}
			case IDA_STRIKE:
			{
				auto& currentPage = doc->GetCurrentPage();
				memset(&cf2, 0, sizeof(CHARFORMAT2));
				cf2.cbSize = sizeof(CHARFORMAT2);
				SendMessage(currentPage.hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				cf2.dwEffects ^= CFE_STRIKEOUT;
				cf2.dwMask |= CFM_STRIKEOUT;
				SendMessage(currentPage.hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
				SetFocus(currentPage.hWnd);
				break;
			}
			case IDA_OPEN:
				doc->OpenFile();
				SetFocus(doc->GetCurrentPage().hWnd);
				break;
			case IDA_SAVE:
				doc->Save();
				SetFocus(doc->GetCurrentPage().hWnd);
				break;
			case IDA_FIND:
				doc->CreateFindDialog();
				break;
			case IDA_REPLACE:
				doc->CreateFindReplaceDialog();
				break;
			}
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				HWND edit = (HWND)(lParam);
				GETTEXTLENGTHEX gtlx;
				gtlx.flags = GTL_DEFAULT;
				gtlx.codepage = CP_ACP;
				int charAmount = SendMessage(edit, EM_GETTEXTLENGTHEX, (WPARAM)&gtlx, 0);
				if (charAmount == 0)
					doc->ResetFindInfo();
				break;
			}
			}; break;
			//process all other messages
		default:return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	};
}