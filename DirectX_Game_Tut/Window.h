#pragma once

#include<Windows.h>

class Window
{
public:
	Window();
	bool init();
	bool broadcast();
	bool release();
	bool isRun();
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	RECT getClientWindowRect();
	void setHWND(HWND hwnd);
	~Window();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

