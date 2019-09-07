#include "MainWnd.h"
#include "LogIn.h"
#include "Cashier.h"



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
// 	CPaintManagerUI::SetInstance(hInstance);
// 	MainWnd duiFrame;
// 	duiFrame.Create(NULL, _T("MAINWND"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
// 	duiFrame.CenterWindow();
// 	duiFrame.ShowModal();



	CPaintManagerUI::SetInstance(hInstance);
	LogInWnd loginWnd;
	loginWnd.Create(NULL, _T("LOGINWND"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	loginWnd.CenterWindow();
	loginWnd.ShowModal();
	return 0;
}