#include "Sprite.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
void Sprite::Initialize(SpriteCommon* _sptriteCommon)
{
	HRESULT result();
	assert(_spriteCommon)
	spriteCommon* _spriteCommon;
	//���_�f�[�^
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f},
	};
	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̍쐬
	ID3D12Resource* vertBuff = nullptr;
	result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	//GPU��̂΂ӂ���
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbview{};
	//GPU���z
	vbview.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeVB;
	vbview.StrideInBytes = sizeof(XMFLOAT3);
}

void Sprite::Draw()
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, vbView);
	// �`��R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(3, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}


