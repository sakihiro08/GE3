#pragma once
#include <Windows.h>
class Winapp
{
public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	void Initialize();
	void Update();
	static const int window_width = 1280;
	static const int window_height = 720;
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	void Finalize();
	bool ProcessMessage();
private:
	HWND hwnd = nullptr;

	WNDCLASSEX w{};	
	
};