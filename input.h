#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>


class Input
{ 
public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	//初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//更新
	void Update();
   bool PushKey(BYTE keyNumber);	
   bool TriggerKey(BYTE keyNumber);
   //DirectInputのインスタンス
   ComPtr<IDirectInput8> directInput;
   // 全キーの入力状態を取得する
   BYTE key[256] = {};
   // 全トリガーキーの入力状態を取得する
   BYTE keyPre[256] = {};
private://メンバ変数
	ComPtr<IDirectInputDevice8>keyboard;

	
};

