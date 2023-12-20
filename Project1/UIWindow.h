#pragma once


class UIWindow
{
public:
	UIWindow();
	virtual ~UIWindow();

	HWND	m_hWnd;

	bool Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height,
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance);

	void CenterWindow(HWND hParentWnd = NULL);
	bool MoveWindow(int x, int y, int width, int height, BOOL bRepaint);
	bool ShowWindow(int nCmdShow);
	bool UpdateWindow();
	bool DestroyWindow();

protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDestroy();
	virtual void OnPaint(HDC hDC, LPCRECT lpRect);
	virtual int OnEraseBkgnd(HDC hDC);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
