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
	Vertex vertices[] = {
			// x      y     z       u     v
			{{0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}}, // ����
			{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // ����
			{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}}, // �E��
			{{100.0f, 0.0, 0.0f}, {1.0f, 0.0f}}, // �E��
		};

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
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
	//�]��
	Vertex* vertMap = nullptr;
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
	// ���_�f�[�^1���̃T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		ID3D12Resource* constBuffMaterial = nullptr;
		// �萔�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	};
	assert(SUCCEEDED(result));
	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));
	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = color;              // RGBA�Ŕ������̐�
	//�s��
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));

		assert(SUCCEEDED(result));
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
		assert(SUCCEEDED(result));
		//���[���h
		XMMATRIX matWorld;
		matWorld = XMMatrixIdentity();
		rotationZ = 0.f;
		position={0.f,0.f,0.f};
		//��]
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
		matWorld *= matRot;
		//���s
		XMMATRIX matTrans;
         matTrans *= XMMatrixTranslation(position.x, position.y, position.z);
		matWorld *= matTrans;
		//�ˉe
		XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
			0.f, Winapp::window_width,
			Winapp::window_height, 0.f,
			0.0f, 1.0f
		);
		constMapTransform->mat = matWorld * matProjection;

		
	}
}

void Sprite::Update()
{
	constMapMaterial->color = color;
	//���[���h
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	rotationZ = 0.f;
	position = { 0.f,0.f,0.f };
	//��]
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;
	//���s
	XMMATRIX matTrans;
	matTrans *= XMMatrixTranslation(position.x, position.y, position.z);
	matWorld *= matTrans;
	//�ˉe
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.f, Winapp::window_width,
		Winapp::window_height, 0.f,
		0.0f, 1.0f
	);
	constMapTransform->mat = matWorld * matProjection;
}

void Sprite::Draw()
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, vbView);
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(4, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}


