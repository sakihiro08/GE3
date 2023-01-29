#include "Sprite.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
void Sprite::Initialize(SpriteCommon* _sptriteCommon)
{
	HRESULT result();
	assert(_spriteCommon)
	spriteCommon* _spriteCommon;
	//頂点データ
	Vertex vertices[] = {
			// x      y     z       u     v
			{{0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}}, // 左下
			{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 左上
			{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}}, // 右下
			{{100.0f, 0.0, 0.0f}, {1.0f, 0.0f}}, // 右上
		};

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
	ID3D12Resource* vertBuff = nullptr;
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
	D3D12_VERTEX_BUFFER_VIEW vbview{};
	//GPU仮想
	vbview.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeVB;
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
		ID3D12Resource* constBuffMaterial = nullptr;
		// 定数バッファの生成
		result = device->CreateCommittedResource(
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
		rotationZ = 0.f;
		position={0.f,0.f,0.f};
		//回転
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
		matWorld *= matRot;
		//平行
		XMMATRIX matTrans;
         matTrans *= XMMatrixTranslation(position.x, position.y, position.z);
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
	constMapMaterial->color = color;
	//ワールド
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	rotationZ = 0.f;
	position = { 0.f,0.f,0.f };
	//回転
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;
	//平行
	XMMATRIX matTrans;
	matTrans *= XMMatrixTranslation(position.x, position.y, position.z);
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
	// 頂点バッファビューの設定コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXComon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(4, 1, 0, 0); // 全ての頂点を使って描画
}


