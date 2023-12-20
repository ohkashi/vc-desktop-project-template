#pragma once
#include "UIWindow.h"


class UIDialog : public UIWindow
{
public:
	explicit UIDialog(LPCTSTR lpszTemplateName, HWND hParentWnd = NULL);
	explicit UIDialog(UINT nIDTemplate, HWND hParentWnd = NULL);
	virtual ~UIDialog();

	virtual bool Create(LPCTSTR lpszTemplateName, HWND hParentWnd = NULL);
	virtual bool Create(UINT nIDTemplate, HWND hParentWnd = NULL);
	virtual INT_PTR DoModal();
	void EndDialog(int nResult);

protected:
	virtual bool OnInitDialog();
	virtual INT_PTR DialogProc(UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPCTSTR	m_lpszDlgTemplate;
	HWND	m_hParentWnd;
	HBRUSH	m_hbrBkgnd;
};
