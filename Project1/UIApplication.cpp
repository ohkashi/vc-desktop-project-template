// Project1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "UIApplication.h"
#include "MainWindow.h"
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")


UIApplication theApp;

UIApplication::UIApplication() : m_hInstance(nullptr), m_nDPI(96), m_bDarkMode(false), m_pMainWnd(nullptr)
{
	HWND hDesktop = ::GetDesktopWindow();
	HDC hDC = ::GetDC(hDesktop);
	m_nDPI = ::GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(hDesktop, hDC);
	m_bDarkMode = !IsLightTheme();
	if (m_bDarkMode) {
		m_clrBkgnd = RGB(38, 38, 38);
		m_clrItemsView = RGB(26, 26, 26);
		m_clrText = RGB(255, 255, 255);
	} else {
		m_clrBkgnd = GetSysColor(COLOR_BTNFACE);
		m_clrItemsView = GetSysColor(COLOR_WINDOW);
		m_clrText = GetSysColor(COLOR_WINDOWTEXT);
	}
}

UIApplication::~UIApplication()
{
}

bool UIApplication::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	INITCOMMONCONTROLSEX InitCtrls = { sizeof(InitCtrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&InitCtrls);

	m_hInstance = hInstance;
	CString strAppTitle;
	int nRet = strAppTitle.LoadString(hInstance, IDS_APP_TITLE);
	ATLASSERT(nRet != 0);

	int x = 0, y = 0, w = 800, h = 600;

	MainWindow* pMainWnd = new MainWindow();
	if (!pMainWnd->Create(nullptr, strAppTitle, WS_OVERLAPPEDWINDOW, x, y, w, h, nullptr, nullptr, hInstance)) {
		delete pMainWnd;
		return false;
	}

	m_pMainWnd = pMainWnd;

	m_pMainWnd->ShowWindow(nCmdShow);
	m_pMainWnd->UpdateWindow();

	return true;
}

int UIApplication::ExitInstance()
{
	delete m_pMainWnd;
	m_pMainWnd = nullptr;

	return 0;
}

bool UIApplication::IsLightTheme()
{
	DWORD dwData = 0;
	DWORD cbData = sizeof(dwData);
	auto res = RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
		L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &dwData, &cbData);
	if (res != ERROR_SUCCESS)
		return true;
	return (dwData != 0);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Perform application initialization:
    if (!theApp.InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    // Main message loop:
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));
	while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return theApp.ExitInstance();
}
