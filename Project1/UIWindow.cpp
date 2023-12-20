#include "pch.h"
#include "UIWindow.h"
#include "UIApplication.h"
#include <dwmapi.h>

#pragma comment(lib, "DwmApi.lib")


UIWindow::UIWindow() : m_hWnd(NULL)
{
}

UIWindow::~UIWindow()
{
	if (m_hWnd && IsWindow(m_hWnd))
		DestroyWindow();
}

bool UIWindow::Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height,
	HWND hWndParent, HMENU hMenu, HINSTANCE hInstance)
{
	CString strClassName;
	if (lpClassName && _tcslen(lpClassName) > 0)
		strClassName = lpClassName;
	else
		strClassName = _T("UIWindow");

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	if (!GetClassInfo(hInstance, strClassName, &wc)) {
		wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = &UIWindow::WndProc;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = strClassName;
		if (!RegisterClass(&wc))
			return false;
	}

	m_hWnd = CreateWindowW(strClassName, lpWindowName, dwStyle, x, y, width, height, hWndParent, hMenu, hInstance, this);
	return (m_hWnd != NULL);
}

void UIWindow::CenterWindow(HWND hParentWnd)
{
	CRect rect;
	GetWindowRect(m_hWnd, &rect);
	int x, y, cx = rect.Width(), cy = rect.Height();
	if (!hParentWnd) {
		int cxScrn = GetSystemMetrics(SM_CXSCREEN), cyScrn = GetSystemMetrics(SM_CYSCREEN);
		x = (cxScrn - cx) / 2;
		y = (cyScrn - cy) / 2;
	} else {
		CRect rcParent;
		GetWindowRect(hParentWnd, &rcParent);
		x = rcParent.left + (rcParent.Width() - cx) / 2;
		y = rcParent.top + (rcParent.Height() - cy) / 2;
	}
	MoveWindow(x, y, cx, cy, FALSE);
}

bool UIWindow::MoveWindow(int x, int y, int width, int height, BOOL bRepaint)
{
	ATLASSERT(m_hWnd);
	return ::MoveWindow(m_hWnd, x, y, width, height, bRepaint);
}

bool UIWindow::ShowWindow(int nCmdShow)
{
	ATLASSERT(m_hWnd);
	return ::ShowWindow(m_hWnd, nCmdShow);
}

bool UIWindow::UpdateWindow()
{
	ATLASSERT(m_hWnd);
	return ::UpdateWindow(m_hWnd);
}

bool UIWindow::DestroyWindow()
{
	ATLASSERT(m_hWnd);
	bool bRet = ::DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
	return bRet;
}

int UIWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (theApp.DarkMode) {
		BOOL value = TRUE;
		HRESULT hr = ::DwmSetWindowAttribute(m_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
		if (SUCCEEDED(hr)) {
			LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
			if (lStyle & WS_CAPTION) {
				int value = (int)DWMSBT_TRANSIENTWINDOW;
				DwmSetWindowAttribute(m_hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof(value));
			}
		}
	}

	return 0;
}

void UIWindow::OnDestroy()
{
}

void UIWindow::OnPaint(HDC hDC, LPCRECT lpRect)
{
}

int UIWindow::OnEraseBkgnd(HDC hDC)
{
	if (theApp.DarkMode) {
		CRect rect;
		GetClientRect(m_hWnd, &rect);
		COLORREF clrOld = ::SetBkColor(hDC, theApp.BkgndColor);
		ATLASSERT(clrOld != CLR_INVALID);
		if (clrOld != CLR_INVALID) {
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rect, NULL, 0, NULL);
			::SetBkColor(hDC, clrOld);
		}
		return 1;
	}
	return 0;
}

LRESULT UIWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(m_hWnd, message, wParam, lParam);
}

LRESULT CALLBACK UIWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UIWindow* pThis = reinterpret_cast<UIWindow*>((LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (uMsg) {
	case WM_CREATE:
		return pThis->OnCreate((LPCREATESTRUCT)lParam);
	case WM_DESTROY:
		pThis->OnDestroy();
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			pThis->OnPaint(ps.hdc, &ps.rcPaint);
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_ERASEBKGND:
		return pThis->OnEraseBkgnd((HDC)wParam);
	case WM_NCCREATE:
		pThis = reinterpret_cast<UIWindow*>((LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
		pThis->m_hWnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		break;
	default:
		break;
	}

	if (pThis)
		return pThis->WindowProc(uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
