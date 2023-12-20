#pragma once
#include "UIWindow.h"

class MainWindow : public UIWindow
{
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDestroy();
	virtual void OnPaint(HDC hDC, LPCRECT lpRect);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
