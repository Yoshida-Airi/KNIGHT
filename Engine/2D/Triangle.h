/**
*	@file Triangle.h
*	@brief 三角形クラスヘッダ
*/


#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"VectorMath.h"
#include"Globals.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"
#include"WorldTransform.h"

#include"Camera.h"


#include<wrl.h>

namespace AobaraEngine
{
	struct TriangleData
	{
		Vector4 vertex[3];
		Vector4 color;
	};

	/**
	*   @class Triangle
	*	@brief  三角形クラス
	*/
	class Triangle
	{
	public:
		/// @brief デストラクタ
		~Triangle();
		/// @brief 初期化処理
		void Initialize(const uint32_t& textureHandle);
		/// @brief 更新処理
		void Update();
		/// @brief 描画処理
		void Draw(const Camera& camera);


		/// @brief マテリアルデータの設定
		/// @param color マテリアルの色
		void SetMaterialData(const Vector4& color);

		/// @brief テクスチャSRVハンドルを設定
		/// @param textureSrvHandleGPU ハンドル
		void SetTextureSrvHandleGPU(const D3D12_GPU_DESCRIPTOR_HANDLE& textureSrvHandleGPU)
		{
			textureSrvHandleGPU_ = textureSrvHandleGPU;
		}

		/// @brief スプライトの可視性を設定
	/// @param isInvisible true :非表示
		void SetisInvisible(const bool& isInvisible)
		{
			isInvisible_ = isInvisible;
		}

		/// @brief ワールドトランスフォームを取得
		/// @return ワールドトランスフォーム
		WorldTransform* GetWorldTransform()const { return worldTransform_; };

		/// @brief 三角形の生成
		/// @param textureHandle テクスチャ番号
		/// @return 生成された三角形
		static Triangle* Create(const uint32_t& textureHandle);

	private://プライベート変数

		DirectXCommon* dxCommon_;
		GraphicsPipelineManager* psoManager_;
		TextureManager* texture_;


		Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
		Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
		Microsoft::WRL::ComPtr< ID3D12Resource> wvpResource_;	//wvpリソース
		Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

		VertexData* vertexData_ = nullptr;
		Material* materialData_ = nullptr;	//マテリアルデータ
		//Matrix4x4* wvpData_ = nullptr;	//wvpデータ
		DirectionalLight* lightData_ = nullptr;

		EulerTransform cameraTransform_;

		uint32_t textureHandle_;

		WorldTransform* worldTransform_;

		bool isInvisible_ = false;

	private://プライベート関数

		/// @brief 頂点バッファを取得
		void VertexBuffer();

		/// @brief マテリアルバッファを取得
		void MaterialBuffer();

		/// @brief ライトバッファを取得
		void LightBuffer();
	};

}