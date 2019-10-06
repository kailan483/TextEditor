#include "ComboBox.h"

LRESULT CALLBACK ComboProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		HWND hCombo;
		hCombo = GetParent(hWnd);
		DoAutoComplete(hCombo, hWnd, (TCHAR)wParam);
		break;
	}
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

int Combo_FindString(HWND hwndCtl, INT indexStart, LPTSTR lpszFind)
{
	TCHAR lpszBuffer[DEFAULT_TXT_LIM];
	TCHAR tmp[DEFAULT_TXT_LIM];
	int ln = _tcslen(lpszFind) + 1;
	if (ln == 1 || indexStart > ComboBox_GetCount(hwndCtl))
		return CB_ERR;

	for (int i = indexStart == -1 ? 0 : indexStart; i < ComboBox_GetCount(hwndCtl); i++)
	{
		ComboBox_GetLBText(hwndCtl, i, lpszBuffer);
		lstrcpyn(tmp, lpszBuffer, ln);
		if (!_tcsicmp(lpszFind, tmp))
			return i;
	}
	return CB_ERR;
}

bool isPositiveInteger(const std::string& s)
{
	return !s.empty() &&
		(std::count_if(s.begin(), s.end(), std::isdigit) == s.size());
}

void DoAutoComplete(HWND hwnd, HWND hEdit, TCHAR ch)
{
	/*int hr = ComboBox_SetCurSel(hwnd, 5);*/

	// Note: If user presses VK_RETURN or VK_TAB then
	//	the ComboBox Notification = CBN_SELENDCANCEL and
	//	a call to ComboBox_GetCurSel() will return the canceled index.
	//	If the user presses any other key that causes a selection
	//	and closure of the dropdown then
	//	the ComboBox Notification = CBN_SELCHANGE

	static TCHAR buf[DEFAULT_TXT_LIM];
	static TCHAR toFind[DEFAULT_TXT_LIM];
	static BOOL fMatched = TRUE;
	int index = 0;
	// Handle keyboard input
	if (VK_RETURN == ch)
	{

		/*int i = ComboBox_GetCurSel(hwnd);*/
		HWND hwndMenuBar = (HWND)GetParent(hwnd);
		HWND hMain = (HWND)GetParent(hwndMenuBar);
		int id = (int)GetProp(hwnd, "ID");
		if (id == ID_FONTCOMBOBOX)
		{
			int textlen = GetWindowTextLength(hEdit);
			TCHAR buf[256];
			memset(buf, 0, sizeof(buf));
			SendMessage(hEdit, WM_GETTEXT, textlen, (LPARAM)buf);
			int result = ComboBox_FindStringExact(hwnd, 1, buf);
			if (result == CB_ERR)
				ComboBox_SetCurSel(hwnd, 1);
			SendMessage(hwndMenuBar, WM_COMMAND, MAKEWPARAM(id, CBN_SELCHANGE), (LPARAM)(HANDLE)hwnd);
			SendMessage(hMain, WM_RESTOREFOCUS, 0, 0);
			Combo_SetEditSel(hwnd, -1, 0); //selects the entire item
		}
		else if (id == ID_FONTSIZECOMBOBOX)
		{
			int textlen = GetWindowTextLength(hEdit);
			TCHAR buf[256];
			memset(buf, 0, sizeof(buf));
			SendMessage(hEdit, WM_GETTEXT, textlen, (LPARAM)buf);
			int fontsize;
			if (!(buf[0] == _T('\0')) && isPositiveInteger(buf))
			{
				// m_sLogPath contains empty string.
				fontsize = stoi(buf);
				if (fontsize < 0)
				{
					fontsize = 10;
					memset(buf, 0, sizeof(buf));
					_itoa_s(fontsize, buf, 256, 10);
				}
			}
			else
			{
				fontsize = 10;
				memset(buf, 0, sizeof(buf));
				_itoa_s(fontsize, buf, 256, 10);
			}
			SendMessage(hMain, ID_SETFONTSIZE, NULL, (LPARAM)fontsize);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)buf);
			SendMessage(hMain, WM_RESTOREFOCUS, 0, 0);
		}

		ComboBox_ShowDropdown(hwnd, FALSE);

	}
	else if (VK_BACK == ch)
	{
		if (fMatched)// 27Jan11 - added
		{
			//Backspace normally erases highlighted match
			//	we only want to move the highlighter back a step
			index = ComboBox_GetCurSel(hwnd);
			int bs = LOWORD(ComboBox_GetEditSel(hwnd)) - 1;

			// keep current selection selected
			ComboBox_SetCurSel(hwnd, index);

			// Move cursor back one space to the insertion point for new text
			// and hilight the remainder of the selected match or near match
			Combo_SetEditSel(hwnd, bs, -1);
		}
		else// 27Jan11 - added
		{
			if (_tcslen(toFind) > 0)
				toFind[_tcslen(toFind) - 1] = 0;
			ComboBox_SetText(hwnd, toFind);
			Combo_SetEditSel(hwnd, -1, -1);
			FORWARD_WM_KEYDOWN(hwnd, VK_END, 0, 0, SNDMSG);
		}
	}
	else if (!_istcntrl(ch))
	{
		BOOL status = GetWindowLongPtr(hwnd, GWL_STYLE) & CBS_DROPDOWN;
		if (status)
			ComboBox_ShowDropdown(hwnd, TRUE);

		//if (IsExtended(hwnd)) // keep focus on edit box
		//	SetFocus(ComboBoxEx_GetEditControl(hwnd));
		/*SetFocus(FindWindowEx(hwnd, NULL, WC_EDIT, NULL));*/
		HWND hWnd = FindWindowEx(hwnd, NULL, WC_EDIT, NULL);
		int i = SendMessage(hWnd, EM_GETLIMITTEXT, 0, 0);

		// Get the substring from 0 to start of selection
		ComboBox_GetText(hwnd, buf, NELEMS(buf));
		buf[LOWORD(ComboBox_GetEditSel(hwnd))] = 0;

		_stprintf_s(toFind, NELEMS(toFind),
#ifdef _UNICODE
			_T("%ls%lc"),
#else
			_T("%s%c"),
#endif
			buf, ch);
		/*MessageBox(NULL, toFind, toFind, MB_OK);*/
		// Find the first item in the combo box that matches ToFind
		index = ComboBox_FindStringExact(hwnd, -1, toFind);

		if (CB_ERR == index) //no match
		{
			// Find the first item in the combo box that starts with ToFind
			index = Combo_FindString(hwnd, -1, toFind);
		}
		if (CB_ERR != index)
		{
			// Else for match
			fMatched = TRUE;
			int hr = ComboBox_SetCurSel(hwnd, index);
			SendMessage(hwnd, CB_SETEDITSEL, 0, MAKELPARAM(_tcslen(toFind), -1));
		}
		else // 27Jan11 - Display text that is not in the selected list 
		{
			fMatched = FALSE;
			ComboBox_SetText(hwnd, toFind);
			Combo_SetEditSel(hwnd, _tcslen(toFind), -1);
			FORWARD_WM_KEYDOWN(hwnd, VK_END, 0, 0, SNDMSG);
		}
	}
}
