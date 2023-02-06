#include "Sprite.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
void Sprite::Initialize(SpriteCommon* _spriteCommon,uint32_t textureIndex)
{
	HRESULT result;
	assert(_spriteCommon);
	spriteCommon=_spriteCommon;
	//テクスチャサイズ
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
	//頂点データ
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
		

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの作成


	 result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//転送
	Vertex* vertMap = nullptr;
	 result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	//つながりを解除
	vertBuff->Unmap(0, nullptr);
	//頂点バッファビューの作成

	//GPU仮想
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	// 頂点データ1つ分のサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
		// 定数バッファの生成
		result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	};
	assert(SUCCEEDED(result));
	// 定数バッファのマッピング
	 result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMapMaterial->color = color;              // RGBAで半透明の赤
	//行列
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		 result = spriteCommon->GetDirectXComon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));

		assert(SUCCEEDED(result));
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
		assert(SUCCEEDED(result));
		//ワールド
		XMMATRIX matWorld;
		matWorld = XMMatrixIdentity();
		//回転
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
		matWorld *= matRot;
		//平行
		XMMATRIX matTrans;
         matTrans = XMMatrixTranslation(position.x, position.y, 0.f);
		matWorld *= matTrans;
		//射影
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
	//転送
		Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	constMapMaterial->color = color;
	//ワールド
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	//回転
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;

	//平行
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position.x, position.y, 0.f);
	matWorld *= matTrans;

	//射影
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
	//テクスチャコマンド
	spriteCommon->SetTextureCommands(textureIndex);
	// 頂点バッファビューの設定コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(4, 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex);
	assert(textureBuffer);
	//テクスチャ
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();
	texturesize.x=static_cast<float>(resDesc.Width);
	texturesize.y=static_cast<float>(resDesc.Height);
}


