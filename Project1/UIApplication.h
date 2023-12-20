#pragma once
#include "resource.h"


class UIWindow;

class UIApplication
{
public:
	UIApplication();
	~UIApplication();

	bool InitInstance(HINSTANCE hInstance, int nCmdShow);
	int ExitInstance();

	bool IsLightTheme();

	inline HINSTANCE get_instance() const { return m_hInstance; }
	inline int get_dpi() const { return m_nDPI; }
	inline bool is_dark_mode() const { return m_bDarkMode; }
	inline COLORREF get_bkgnd_color() const { return m_clrBkgnd; }
	inline COLORREF get_itemsview_color() const { return m_clrItemsView; }
	inline COLORREF get_text_color() const { return m_clrText; }

	__declspec(property(get = get_instance))		HINSTANCE Instance;
	__declspec(property(get = get_dpi))				int DPI;
	__declspec(property(get = is_dark_mode))		bool DarkMode;
	__declspec(property(get = get_bkgnd_color))		COLORREF BkgndColor;
	__declspec(property(get = get_itemsview_color))	COLORREF ItemsViewColor;
	__declspec(property(get = get_text_color))		COLORREF TextColor;

protected:

private:
	HINSTANCE	m_hInstance;
	int			m_nDPI;
	bool		m_bDarkMode;
	UIWindow*	m_pMainWnd;
	COLORREF	m_clrBkgnd;
	COLORREF	m_clrItemsView;
	COLORREF	m_clrText;
};

extern UIApplication theApp;
