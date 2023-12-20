#include "pch.h"
#include "MainWindow.h"
#include "UIApplication.h"
#include "UIDialog.h"


MainWindow::MainWindow() : UIWindow()
{
}

MainWindow::~MainWindow()
{
}

int MainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);

	CenterWindow();
	PostMessage(m_hWnd, WM_COMMAND, IDM_ABOUT, 0);
	return 0;
}

void MainWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void MainWindow::OnPaint(HDC hDC, LPCRECT lpRect)
{
}

LRESULT MainWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId) {
			case IDM_ABOUT:
				{
					UIDialog dlg(IDD_ABOUTBOX, m_hWnd);
					INT_PTR nRet = dlg.DoModal();
					ATLTRACE("dlg.DoModal(): nRet = %d\n", nRet);
				}
				break;
			case IDM_EXIT:
				DestroyWindow();
				break;
			default:
				return UIWindow::WindowProc(message, wParam, lParam);
			}
		}
		break;
	default:
		return UIWindow::WindowProc(message, wParam, lParam);
	}
	return 0;
}
