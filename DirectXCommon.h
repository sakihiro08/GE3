#pragma once
#include<d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include"Winapp.h"
#include <vector>
#include<chrono>
class DirectXCommon
{
public:
	void Initialize(Winapp* winApp);
	void InitializeDecvice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
	void PreDraw();
	void PostDraw();
	ID3D12Device* GetDevice()const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }
private:
	
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	  DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	  std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	Winapp* winApp = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	void InitializeFIXFPS();
	void UpdateFIXFPS();
	std::chrono::steady_clock::time_point reference;
};