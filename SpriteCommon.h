#pragma once
#include"DirectXMath.h"
#include<d3d12.h>
#include<wrl.h>
#include"DirectXCommon.h"

class SpriteCommon
{

public:
	void Initialize(DirectXCommon* _dxCommon);
	void PreDraw();
	DirectXCommon* GetDirectXComon() { return dxCommon; }
private:
	DirectXCommon* dxCommon;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSIgnsture;

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff;

	//ヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap;
};