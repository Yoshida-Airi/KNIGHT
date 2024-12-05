/**
*	@file TextureManager.h
*	@brief テクスチャ保存クラスヘッダ
*/

#pragma once

#include"Logger.h"
#include"DirectXCommon.h"
#include"SrvManager.h"

#include"DirectXTex.h"
#include"d3dx12.h"

#include<d3d12.h>
#include<array>
#include<unordered_map>


#pragma comment(lib,"d3d12.lib")

namespace AobaraEngine
{
	/**
	*   @class TextureManager
	*	@brief テクスチャ管理クラス
	*/
	class TextureManager
	{
	public:
		static TextureManager* GetInstance();

		struct TextureData
		{
			D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
			D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
			Microsoft::WRL::ComPtr< ID3D12Resource> textureResource;
			std::string filename{};
			uint32_t textureHandle;
			uint32_t srvIndex;
		};

		/// <summary>
		/// デストラクタ
		/// </summary>
		~TextureManager();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 画像読み込み
		/// </summary>
		/// <param name="filePath"></param>
		uint32_t LoadTexture(const std::string& filePath);

		//ゲッター
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGPUHandle(uint32_t textureHandle);

		/// <summary>
		/// テクスチャの情報を取得
		/// </summary>
		/// <param name="textureHandle"></param>
		/// <returns></returns>
		const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

		std::string SearchFilepath(uint32_t index);

	private:

		static const size_t kMaxTexture_ = 256;	//最大テクスチャ数
		DirectXCommon* dxCommon_;
		AobaraEngine::SrvManager* srvManager_;

		DirectX::ScratchImage mipImages_;
		Microsoft::WRL::ComPtr< ID3D12Resource> textureResource_;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};
		Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptoHeap_;

		D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_ = {};
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_ = {};

		//中間リソース
		std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxTexture_> intermediateResource_;

		std::unordered_map<std::string, TextureData> textureDatas_;
		bool isUsedTexture_[kMaxTexture_];
		uint32_t descriptorSizeSRV_;

		static TextureManager* instance_;

		static uint32_t kSRVIndexTop_;

	private:

		DirectX::ScratchImage ImageFileOpen(const std::string& filePath);
		Microsoft::WRL::ComPtr< ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

		[[nodiscard]]
		Microsoft::WRL::ComPtr< ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


		//uint32_t FindOrCreateTextureIndex(const std::string& filePath);

	};
}