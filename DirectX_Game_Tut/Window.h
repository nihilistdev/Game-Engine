#pragma once

#include <iostream>
#include <Windows.h>

class Window
{
public:
	Window();
	bool isRun();
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();
	RECT getClientWindowRect();
	RECT getSizeScreen();
	virtual ~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};

