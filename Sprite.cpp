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
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f},
	};
	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
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
	//GPU上のばふぁに
	XMFLOAT3* vertMap = nullptr;
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
	vbview.StrideInBytes = sizeof(XMFLOAT3);
}

void Sprite::Draw()
{
	// 頂点バッファビューの設定コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->IASetVertexBuffers(0, 1, vbView);
	// 描画コマンド
	spriteCommon->GetDirectXComon()->GetCommandList()->DrawInstanced(3, 1, 0, 0); // 全ての頂点を使って描画
}


