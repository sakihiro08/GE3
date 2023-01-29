#pragma once
#include<DirectXMath.h>
#include"SpriteCommon.h"
#include<cassert>

class Sprite
{
private:

	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color; // 色 (RGBA
	};
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat;
	};
	// 頂点データ構造体
struct Vertex
{
	DirectX::XMFLOAT3 pos; // xyz座標
	DirectX::XMFLOAT2 uv;  // uv座標
};

public: 
	void Initialize(SpriteCommon* _sptriteCommon);
	void Update();
	void Draw();
private:

	SpriteCommon* spriteCommon = nullptr;	
	DirectX::XMFLOAT4 color = { 1,1,1,1.f };
	float rotationZ;
	DirectX::XMFLOAT3 position;
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};