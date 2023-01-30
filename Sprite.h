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
	enum VertexNumber
	{
		LB,LT,RB,RT
	};
public: 
	void Initialize(SpriteCommon* _sptriteCommon,uint32_t textureIndex=UINT32_MAX);
	void Update();
	void Draw();
	//�������[
	const DirectX::XMFLOAT4 Getcolor() const { return color; }
	const DirectX::XMFLOAT2& GetPosition() const { return position; }
	const float& GetRotationZ()const { return rotationZ; }
	const DirectX::XMFLOAT2& Getsize() const { return size; }
	const DirectX::XMFLOAT2& GetAnchorPoint() const { return anchorPoint; }
	const bool& GetIsFlipX() const { return IsFlipX; }
	const bool& GetIsFlipY() const { return IsFlipY; }
	const bool& Getisinvisible() const { return isinvisible; }
	const uint32_t GetTextureIndex()const { return textureIndex; }
	const DirectX::XMFLOAT2& GetTextureLeftTop()const { return textureLefttop; }
	const DirectX::XMFLOAT2& GetTextureSize()const { return texturesize; }
	//�������[
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
	void setPosition(const DirectX::XMFLOAT2& position) { this->position = position; }
	
	void setRotationZ(const float& rotationZ) { this->rotationZ = rotationZ; }
	void setSize(const DirectX::XMFLOAT2& size) { this->size = size; }
	void setAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { this->anchorPoint = anchorPoint; }

	void SetIsFlipX(const bool& isFlipX) { this->IsFlipX = isFlipX; }
	void SetIsFlipY(const bool& isFlipY) { this->IsFlipY = isFlipY; }
	void Setisinvisible(const bool& isinvisible) { this->isinvisible = isinvisible; }
	void SetTextureIndex(uint32_t index) { this->textureIndex = index; }
	void SetTexturelefttop(const DirectX::XMFLOAT2 lefttop) { this->textureLefttop = lefttop; }
	void SetTexturesize(const DirectX::XMFLOAT2 size) { this->texturesize = size; }
	void SetTextureIndex(uint32_t index) { this->textureIndex = index; }
private:
	void AdjustTextureSize();
private:
	//�e�N�X�`���ԍ�
	uint32_t textureIndex = 0;

	SpriteCommon* spriteCommon = nullptr;	
	DirectX::XMFLOAT4 color = { 1,1,1,1.f };
	//�X�v���C�g
	DirectX::XMFLOAT2 position = { 0.f,0.f };
	float rotationZ=0.f;
	DirectX::XMFLOAT2 size = { 100.f,100.f };
	DirectX::XMFLOAT2 anchorPoint= { 0.f,0.f };
	//�e�N�X�`��
	DirectX::XMFLOAT2 textureLefttop= { 0.f,0.f };
	DirectX::XMFLOAT2 texturesize= { 100.f,100.f };

	bool IsFlipX = false;
	bool IsFlipY = false;
	bool isinvisible = false;
	Vertex vertices[4];

	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};