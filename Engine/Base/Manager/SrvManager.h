/**
*	@file SrvManager.h
*	@brief srv制御クラスヘッダ
*/

#pragma once
#include"DirectXCommon.h"

/**
*   @class SrvManager
*	@brief  srv管理クラス
*/
class SrvManager
{
public:
	/// @brief シングルトンインスタンスを取得します。
	/// @return SrvManagerのインスタンス
	static SrvManager* GetInstance();

	/// @brief 初期化処理
	void Initialize();
	/// @brief 描画前準備処理
	void PreDraw();

	/// @brief 新しいSRVを割り当て、インデックスを返します。
	/// @return 割り当てられたSRVのインデックス
	uint32_t Allocate();

	/// @brief SRVの最大数を超えているかチェックします。
	/// @return 最大数を超えている場合はtrue、それ以外はfalse
	bool SrvMaxCountCheck();

	/*ゲッター*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> GetDescriptorHeap()const { return descriptorHeap_.Get(); };

	/*セッター*/
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/// @brief テクスチャ2D用のSRVを生成します。
	/// @param srvIndex SRVのインデックス
	/// @param pResource リソースへのポインタ
	/// @param format テクスチャのフォーマット
	/// @param MipLevels ミップレベル数
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT MipLevels);

	/// @brief 構造化バッファ用のSRVを生成します。
	/// @param srvIndex SRVのインデックス
	/// @param pResource リソースへのポインタ
	/// @param numElements 要素数
	/// @param structureByteStride 構造体のバイトサイズ
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	static const uint32_t kMaxSRVCount;	//最大SRV数

private:
	DirectXCommon* dxCommon_ = nullptr;

	
	uint32_t descriptorSize_;//SRV用のディスクリプタサイズ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap_;	//SRV用のディスクリプタヒープ

	uint32_t useIndex_ = 0;	//次に使用するSRVインデックス

	static SrvManager* instance_;

};

