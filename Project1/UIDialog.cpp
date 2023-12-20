#include "pch.h"
#include "UIDialog.h"
#include "UIApplication.h"


UIDialog::UIDialog(LPCTSTR lpszTemplateName, HWND hParentWnd) : UIWindow(),
	m_lpszDlgTemplate(lpszTemplateName), m_hParentWnd(hParentWnd), m_hbrBkgnd(NULL)
{
}

UIDialog::UIDialog(UINT nIDTemplate, HWND hParentWnd) : UIWindow(),
	m_lpszDlgTemplate(MAKEINTRESOURCE(nIDTemplate)), m_hParentWnd(hParentWnd), m_hbrBkgnd(NULL)
{
}

UIDialog::~UIDialog()
{
	if (m_hbrBkgnd) {
		DeleteObject(m_hbrBkgnd);
		m_hbrBkgnd = NULL;
	}
}

bool UIDialog::Create(LPCTSTR lpszTemplateName, HWND hParentWnd)
{
	m_hWnd = CreateDialogParam(theApp.Instance, lpszTemplateName, hParentWnd, &UIDialog::DlgProc, (LPARAM)this);
	return (m_hWnd != NULL);
}

bool UIDialog::Create(UINT nIDTemplate, HWND hParentWnd)
{
	m_hWnd = CreateDialogParam(theApp.Instance, MAKEINTRESOURCE(nIDTemplate), hParentWnd, &UIDialog::DlgProc, (LPARAM)this);
	return (m_hWnd != NULL);
}

INT_PTR UIDialog::DoModal()
{
	INT_PTR nResult = DialogBoxParam(theApp.Instance, m_lpszDlgTemplate, m_hParentWnd, &UIDialog::DlgProc, (LPARAM)this);
	if (m_hParentWnd)
		::SetForegroundWindow(m_hParentWnd);
	return nResult;
}

void UIDialog::EndDialog(int nResult)
{
	::EndDialog(m_hWnd, nResult);
}

bool UIDialog::OnInitDialog()
{
	if (0 == UIWindow::OnCreate(NULL)) {
		CenterWindow(m_hParentWnd);
	}

	return true;
}

INT_PTR UIDialog::DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

INT_PTR CALLBACK UIDialog::DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UIDialog* pThis = reinterpret_cast<UIDialog*>((LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
	case WM_INITDIALOG:
		pThis = reinterpret_cast<UIDialog*>(lParam);
		pThis->m_hWnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->OnInitDialog();
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			::EndDialog(hwnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) {
			::EndDialog(hwnd, IDCANCEL);
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
		if (theApp.DarkMode) {
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, theApp.TextColor);
			SetBkColor(hdcStatic, theApp.BkgndColor);
			if (!pThis->m_hbrBkgnd)
				pThis->m_hbrBkgnd = CreateSolidBrush(theApp.BkgndColor);
			return (INT_PTR)pThis->m_hbrBkgnd;
		}
		break;
	default:
		break;
	}

	if (pThis)
		return pThis->DialogProc(uMsg, wParam, lParam);
	return 0;
}
