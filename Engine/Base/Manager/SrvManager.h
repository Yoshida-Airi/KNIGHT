#pragma once
#include"DirectXCommon.h"
#include"DirectXTex.h"

class SrvManager
{
public:

	static SrvManager* GetInstance();

	void Initialize();
	void PreDraw();

	uint32_t Allocate();

	//最大数を超えているかのチェック関数
	bool SrvMaxCountCheck();

	/*ゲッター*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> GetDescriptorHeap()const { return descriptorHeap.Get(); };

	/*セッター*/
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	//SRV生成関数
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DirectX::TexMetadata metadata, UINT MipLevels);
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	static const uint32_t kMaxSRVCount;	//最大SRV数

private:
	DirectXCommon* dxCommon = nullptr;


	uint32_t descriptorSize;//SRV用のディスクリプタサイズ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;	//SRV用のディスクリプタヒープ

	uint32_t useIndex = 0;	//次に使用するSRVインデックス

	static SrvManager* instance;

};
