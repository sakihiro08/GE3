#pragma once
#include"DirectXMath.h"
#include<d3d12.h>
#include<wrl.h>
#include"DirectXCommon.h"
#include<array>
#include<string>
class SpriteCommon
{

public:
	void Initialize(DirectXCommon* _dxCommon);
	void PreDraw();
	void PostDraw();
	void LoadTexture(uint32_t index , const std::string& fileName);
	void SetTextureCommands(uint32_t index);
	DirectXCommon* GetDirectXComon() { return dxCommon; }
	ID3D12Resource* GetTextureBuffer(uint32_t index)const { return texBuff[index].Get(); }
private:
	// SRVの最大個数
	static const size_t kMaxSRVCount = 2056;
	static std::string kDefaultTextureDirectoryPsth;
	DirectXCommon* dxCommon;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSIgnsture;
		//ヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap;

	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,kMaxSRVCount>texBuff;


};