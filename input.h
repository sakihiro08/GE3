#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>


class Input
{
public://�����o�֐�
	//������
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//�X�V
	void Update();
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private://�����o�ϐ�
	ComPtr<IDirectInputDevice8>keyboard;

};

