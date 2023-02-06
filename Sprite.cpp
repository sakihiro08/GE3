#include "Sprite.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
void Sprite::Initialize(SpriteCommon* _spriteCommon,uint32_t textureIndex)
{
	HRESULT result;
	assert(_spriteCommon);
	spriteCommon=_spriteCommon;
	//�e�N�X�`���T�C�Y
	if (textureIndex!=UINT32_MAX)
	{
		this->textureIndex = textureIndex;
		AdjustTextureSize();
		size = texturesize;
	}
	//uv
	{
		ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
		if (textureBuffer)
		{
			D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();
			float tex_left = textureLefttop.x / resDesc.Width;
			float tex_right = (textureLefttop.x + texturesize.x) / resDesc.Width;
			float tex_top = textureLefttop.y / resDesc.Height;
			float tex_bottom = (textureLefttop.y + texturesize.y) / resDesc.Height;
			vertices[LB].uv = { tex_left,tex_bottom };
			vertices[LT].uv = { tex_left,tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}
	//���_�f�[�^
	float left = (0.0f - anchorPoint.x) * size.x;
	    float right= (1.0f - anchorPoint.x) * size.x;
	    float top= (0.0f - anchorPoint.y) * size.y;
	    float bottom= (1.0f - anchorPoint.y) * size.y;
		if (IsFlipX)
		{
			left = -left;
			right = -right;
		}
		if (IsFlipY)
		{
			top= -top;
			bottom = -bottom;
		}
			// x      y     z       u     v
		vertices[LB].pos = { left, bottom, 0.0f };
		vertices[LT].pos = { left, top, 0.0f };
		vertices[RB].pos = { right,bottom, 0.0f };
		vertices[RT].pos = { right, top, 0.0f };
		

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

	//GPU���z
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
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
	
		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
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
		//��]
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
		matWorld *= matRot;
		//���s
		XMMATRIX matTrans;
         matTrans = XMMatrixTranslation(position.x, position.y, 0.f);
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
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;
	if (IsFlipX)
	{
		left = -left;
		right = -right;
	}
	if (IsFlipY)
	{
		top = -top;
		bottom = -bottom;
	}// x      y     z       u     v
	vertices[LB].pos = { left, bottom, 0.0f };
	vertices[LT].pos = { left, top, 0.0f };
	vertices[RB].pos = { right,bottom, 0.0f };
	vertices[RT].pos = { right, top, 0.0f };//uv
		{
			ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
			if (textureBuffer)
			{
				D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();
				float tex_left = textureLefttop.x / resDesc.Width;
				float tex_right = (textureLefttop.x + texturesize.x) / resDesc.Width;
				float tex_top = textureLefttop.y / resDesc.Height;
				float tex_bottom = (textureLefttop.y + texturesize.y) / resDesc.Height;
				vertices[LB].uv = { tex_left,tex_bottom };
				vertices[LT].uv = { tex_left,tex_top };
				vertices[RB].uv = { tex_right,tex_bottom };
				vertices[RT].uv = { tex_right,tex_top };
			}
		}
	//�]��
		Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	constMapMaterial->color = color;
	//���[���h
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	//��]
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;

	//���s
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position.x, position.y, 0.f);
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
	if (isinvisible)
	{
		return;
	}
	//�e�N�X�`���R�}���h
	spriteCommon->SetTextureCommands(textureIndex);
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(4, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
	assert(textureBuffer);
	//�e�N�X�`��
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();
	texturesize.x=static_cast<float>(resDesc.Width);
	texturesize.y=static_cast<float>(resDesc.Height);
}


