#pragma once
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

};