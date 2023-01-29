#pragma once
#include<DirectXMath.h>
#include"SpriteCommon.h"
#include<cassert>

class Sprite
{
private:

	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color; // �F (RGBA
	};
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat;
	};
	// ���_�f�[�^�\����
struct Vertex
{
	DirectX::XMFLOAT3 pos; // xyz���W
	DirectX::XMFLOAT2 uv;  // uv���W
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