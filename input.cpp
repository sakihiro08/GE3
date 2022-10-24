#include "Input.h"
#include <cassert>
//#include <wrl.h>
//using namespace Microsoft::WRL;
//#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
//#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
    HRESULT result;
    ////DirectInput�̃C���X�^���X
    //ComPtr<IDirectInput8> directInput = nullptr;
    result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));
    // �L�[�{�[�h�f�o�C�X�̐���
    //ComPtr<IDirectInputDevice8> keyboard;
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // ���̓f�[�^�`���̃Z�b�g
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    assert(SUCCEEDED(result));
    // �r�����䃌�x���̃Z�b�g
    result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

}

void Input::Update()
{
    memcpy(keyPre, key, sizeof(key));
    // �L�[�{�[�h���̎擾�J�n
    keyboard->Acquire();
    //// �S�L�[�̓��͏�Ԃ��擾����
    //BYTE key[256] = {};
    keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
    if (key[keyNumber])
    {
        return true;
    }
    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
    if (key[keyNumber]&&!keyPre[keyNumber])
    {
        return true;
    }
    return false;
}
