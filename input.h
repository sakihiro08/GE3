#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>


class Input
{ 
public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://�����o�֐�
	//������
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	//�X�V
	void Update();
   bool PushKey(BYTE keyNumber);	
   bool TriggerKey(BYTE keyNumber);
   //DirectInput�̃C���X�^���X
   ComPtr<IDirectInput8> directInput;
   // �S�L�[�̓��͏�Ԃ��擾����
   BYTE key[256] = {};
   // �S�g���K�[�L�[�̓��͏�Ԃ��擾����
   BYTE keyPre[256] = {};
private://�����o�ϐ�
	ComPtr<IDirectInputDevice8>keyboard;

	
};

