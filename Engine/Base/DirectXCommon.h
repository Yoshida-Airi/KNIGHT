/**
*	@file DirectXCommon.h
*	@brief ダイレクトX基底クラスヘッダ
*/

#pragma once
#include"Logger.h"
#include"WinApp.h"
#include"DebugHelper.h"
#include<d3d12.h>
#include<cassert>
#include<wrl.h>
#include<chrono>
#include<thread>

/**
*   @class DirectXCommon
*	@brief  ダイレクトX基底クラス
*/
class DirectXCommon
{
public:
	~DirectXCommon();
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	//シングルトン
	static DirectXCommon* GetInstance();

	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="sizeInBytes">サイズの指定</param>
	/// <returns>バッファ</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateBufferResource(size_t sizeInBytes);
	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap>CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	
	/*=======　　　ゲッター	=======*/
	ID3D12Device* GetDevice()const { return device_.Get(); };
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); };
	//Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> GetSRVDescriptorHeap()const { return srvDescriptorHeap.Get(); };
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc()const { return swapChainDesc_; };
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc()const { return rtvDesc_; };

public:

	//最大SRV数
	//static const uint32_t kMaxSRVCount;

private:

	/// <summary>
	/// デバイスの生成
	/// </summary>
	void SetupDevice();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void SetupCommand();

	/// <summary>
	///　スワップチェインの生成
	/// </summary>
	void SetupSwapChain();

	/// <summary>
	/// レンダーターゲットビューの生成
	/// </summary>
	void SetupRnderTargetView();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void SetupDepthBuffer();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void SetupFence();

	/// <summary>
	/// レンダーターゲットクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// ビューポートの生成
	/// </summary>
	void SetupViewport();

	/// <summary>
	/// シザー矩形の生成
	/// </summary>
	void SetupScissor();

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();


	Microsoft::WRL::ComPtr< ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);


private:
	
	AobaraEngine::WinApp* winApp_;

	HRESULT hr;
	Microsoft::WRL::ComPtr< IDXGIFactory7> dxgiFactory_ = nullptr;	//DXGIFactory
	Microsoft::WRL::ComPtr <IDXGIAdapter4> useAdapter_ = nullptr;	//使用するアダプタ用の変数。
	Microsoft::WRL::ComPtr< ID3D12Device> device_ = nullptr;			//デバイス
	Microsoft::WRL::ComPtr< ID3D12CommandQueue> commandQueue_ = nullptr;	//コマンドキュー
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator_ = nullptr;	//コマンドアロケータ
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList_ = nullptr;	//コマンドリスト
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain_ = nullptr;	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;		//RTV用のディスクリプタヒープ
	//Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvDescriptorHeap = nullptr;		//SRV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;		//DSV用のディスクリプタヒープ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources_[2] = { nullptr };	//スワップチェーンリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource_ = nullptr;	
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_RESOURCE_BARRIER barrier_{};	//トランスフォームバリア
	Microsoft::WRL::ComPtr< ID3D12Fence> fence_ = nullptr;	//フェンス
	uint64_t fenceValue_ = 0;	//フェンスの値
	HANDLE fenceEvent_ = nullptr;

	//記録時間(FPS固定)
	std::chrono::steady_clock::time_point reference_;

	
	D3D12_VIEWPORT viewport_{};	//ビューポート
	D3D12_RECT scissorRect_{};	//シザー矩形

	//静的メンバ変数の宣言と初期化
	static DirectXCommon* instance;

};

