/**
*	@file GraphicsPipelineManager.h
*	@brief パイプラインステートクラスヘッダ
*/


#pragma once
#include"Logger.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<chrono>
#include<dxcapi.h>

#include<wrl.h>

#pragma comment(lib,"dxcompiler.lib")

#include"DirectXCommon.h"

namespace AobaraEngine
{

	enum BlendMode
	{
		kBlendModeNone,		//ブレンドなし
		kBlendModeNormal,	//通常のブレンド
		kBlendModeAdd,		//加算
		kBlendModeSubtract,	//減算
		kBlendModeMultily,	//乗算
		kBlendModeScreen,	//スクリーン
	};

	/**
	*   @class GraphicsPipelineManager
	*	@brief  グラフィックパイプラインマネージャークラス
	*/
	class GraphicsPipelineManager
	{
	public:

		struct PSOData
		{
			Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
			Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;
			Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成
			Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState = nullptr;
		};

		struct PsoMember
		{
			PSOData object3D;
			PSOData skinningObject3D;
			PSOData sprite;
			PSOData particle;
		};


		/// @brief 初期化処理を行います。
		void Initialize();

		/// @brief シングルトンインスタンスを取得します。
		/// @return GraphicsPipelineManagerのインスタンス
		static GraphicsPipelineManager* GetInstance();

		/// @brief PSOメンバーを取得します。
		/// @return PsoMember構造体
		PsoMember GetPsoMember()const { return psoMember_; };

	private:

		DirectXCommon* dxCommon_ = nullptr;
		HRESULT hr_;

		IDxcUtils* dxcUtils_ = nullptr;
		IDxcCompiler3* dxcCompiler_ = nullptr;
		IDxcIncludeHandler* includeHandler_ = nullptr;

		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature_ = nullptr;	//バイナリを元に生成


		D3D12_BLEND_DESC NormalblendDesc_{};

		D3D12_RASTERIZER_DESC rasterizerDesc_{};

		Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob_;
		Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob_;

		D3D12_DEPTH_STENCIL_DESC depthStencil_{};	//DepthStensilStateの設定

		Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState_ = nullptr;

		PsoMember psoMember_;

		//静的メンバ変数の宣言と初期化
		static GraphicsPipelineManager* instance_;

	private:

		/// @brief 3Dオブジェクト用のPSOを生成します。
		/// @param filePath シェーダーファイルのパス
		/// @return 生成したPSOデータ
		PSOData CreateObject3D(const std::wstring& filePath);

		/// @brief スキニングされた3Dオブジェクト用のPSOを生成します。
		/// @param filePath シェーダーファイルのパス
		/// @return 生成したPSOデータ
		PSOData CreateSkinningObject3D(const std::wstring& filePath);

		/// @brief スプライト用のPSOを生成します。
		/// @param filePath シェーダーファイルのパス
		/// @return 生成したPSOデータ
		PSOData CreateSprite(const std::wstring& filePath);

		/// @brief パーティクル用のPSOを生成します。
		/// @param filePath シェーダーファイルのパス
		/// @return 生成したPSOデータ
		PSOData CreateParticle(const std::wstring& filePath);

		/// @brief 共通のPSOを生成します。
		/// @param filePath シェーダーファイル
		/// @param rootParameters ルートパラメータ
		/// @param numRootParameters ルートパラメータの数
		/// @param depthStencilDesc 深度ステンシル設定
		/// @param inputLayoutDesc 入力レイアウト設定
		/// @return 生成したPSOデータ
		GraphicsPipelineManager::PSOData CreateCommonPSO
		(
			const std::wstring& filePath,
			D3D12_ROOT_PARAMETER* rootParameters,
			int numRootParameters,
			D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc
		);

		/// @brief DXCコンパイラを初期化します。
		void InitializeDXCCompiler();


		/// @brief ブレンドステートを設定します。
		/// @param blendMode 設定するブレンドモード
		void SetupBlendState(BlendMode blendMode);

		/// @brief ラスタライザー設定を生成します。
		void SetupRasterrizerState();

		/// @brief 深度ステンシル設定を行います。
		void SetupDepthStencilState();

		/// @brief シェーダーをコンパイルします。
		/// @param filePath シェーダーファイルのパス
		/// @param profile コンパイルプロファイル
		/// @param dxcUtils DXCユーティリティ
		/// @param dxcCompiler DXCコンパイラ
		/// @param includeHandler インクルードハンドラ
		/// @return コンパイルされたシェーダーのバイナリ
		IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler);


	};
}
