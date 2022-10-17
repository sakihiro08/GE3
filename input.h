#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>


class Input
{
public://メンバ関数
	//初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//更新
	void Update();
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private://メンバ変数
	ComPtr<IDirectInputDevice8>keyboard;

};

