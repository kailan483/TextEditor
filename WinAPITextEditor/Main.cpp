#include <Windows.h>
#include <vector>
#include "MainWindow.h"
// Main function for the console
int main() {
	setlocale(LC_ALL, "Russian");
	// Calling the wWinMain function to start the GUI program
	// Parameters:
	// GetModuleHandle(NULL) - To get a handle to the current instance
	// NULL - Previous instance is not needed
	// NULL - Command line parameters are not needed
	// 1 - To show the window normally
	WinMain(GetModuleHandle(NULL), NULL, NULL, 1);

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	vector<WinAPIWindow*> objects;

	MainWindow *mw = new MainWindow(hInstance);

	mw->Show();
	mw->Update();
	//HANDLE image = (HANDLE)::LoadImage(hInstance,MAKEINTRESOURCE(IDB_PNG1),IMAGE_ICON,0,0,LR_DEFAULTCOLOR);

	//auto error = GetLastError();
	HACCEL ha = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			// Check for accelerator keystrokes. 

			if (!TranslateAccelerator(
				mw->hWnd,      // handle to receiving window 
				ha,        // handle to active accelerator table 
				&msg))         // message data 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	objects.clear();

	return msg.wParam;

}