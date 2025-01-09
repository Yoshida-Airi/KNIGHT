/**
*	@file Sprite.h
*	@brief テクスチャクラスヘッダ
*/


#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"VectorMath.h"
#include"Globals.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include"Transform.h"
#include"WorldTransform.h"
#include<wrl.h>

#include"Camera.h"


namespace AobaraEngine
{
	/**
	*   @class Sprite
	*	@brief  スプライトクラス
	*/
	class Sprite
	{
	public:
		/// @brief デストラクタ
		~Sprite();

		/// @brief スプライトの初期化
		/// @param textureHandle テクスチャのハンドル
		void Initialize(const uint32_t& textureHandle);

		/// @brief スプライトの更新
		void Update();

		/// @brief スプライトの描画
		/// @param camera 使用するカメラ
		void Draw(const Camera& camera);

		/// @brief スプライトの頂点データを設定
		/// @param left 左座標
		/// @param right 右座標
		/// @param top 上座標
		/// @param bottom 下座標
		void SetVertexData(const float& left, const float& right, const float& top, const float& bottom);


		/// @brief マテリアルデータの設定
		/// @param color マテリアルの色
		void SetMaterialData(const Vector4& color);

		/// @brief テクスチャのGPUハンドルを設定
		/// @param textureSrvHandleGPU GPU用テクスチャハンドル
		void SetTextureSrvHandleGPU(const D3D12_GPU_DESCRIPTOR_HANDLE& textureSrvHandleGPU)
		{
			textureSrvHandleGPU_ = textureSrvHandleGPU;
		}

		/// @brief スプライトの可視性を設定
		/// @param isInvisible true	:非表示
		void SetisInvisible(const bool& isInvisible)
		{
			isInvisible_ = isInvisible;
		}

		/// @brief スプライトの可視性を取得
		/// @return true:非表示
		bool GetisInvisible()
		{
			return isInvisible_;
		}

		/// @brief テクスチャの左上座標を設定
		/// @param leftTop 左上座標
		void SetTextureLeftTop(const Vector2& leftTop)
		{
			textureLeftTop = leftTop;
		}

		/// @brief テクスチャサイズを設定
		/// @param size 切り取って表示したいサイズ
		void SetSize(const Vector2& size)
		{
			cutSize_ = size;
		}

		/// @brief スプライトの基準点を設定
		/// @param anchorPoint 基準点
		void SetAnchorPoint(const Vector2& anchorPoint)
		{
			anchorPoint_ = anchorPoint;
		}

		/// @brief ワールド変換を取得
		/// @return ワールドトランスフォーム
		WorldTransform* GetWorldTransform()const { return worldTransform_.get(); };


		/// @brief スプライトの生成
		/// @param textureHandle テクスチャ番号
		/// @return 生成されたスプライト
		static Sprite* Create(const uint32_t& textureHandle);

		/// @brief ImGuiのデバッグ
		/// @param name 表示する名前
		void Debug(char *name);

	private://プライベート変数

		DirectXCommon* dxCommon_;
		GraphicsPipelineManager* psoManager_;
		TextureManager* texture_;

		std::unique_ptr< WorldTransform> worldTransform_;

		Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
		Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
		Microsoft::WRL::ComPtr< ID3D12Resource> indexResource_;	//頂点リソース

		D3D12_RESOURCE_DESC resourceDesc_{};	//テクスチャの情報

		Matrix4x4* transformationMatrixData_ = nullptr;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		VertexData* vertexData_ = nullptr;	//頂点データ
		Material* materialData_ = nullptr;	//マテリアルデータ
		uint32_t* indexData_ = nullptr;		//インデックスデータ

		EulerTransform uvTransform_;
		Vector2 textureSize_;	//切り出しサイズ

		bool isInvisible_ = false;	//非表示フラグ	true : 消える

		uint32_t textureHandle_;

		Vector2 cutSize_ = { 100.0f,100.0f };
		Vector2 textureLeftTop = { 0.0f,0.0f };	//テクスチャ左上座標
		Vector2 anchorPoint_ = { 0.0f,0.0f };

		//画像のサイズ
		float textureSizeLeft_;
		float textureSizeRight_;
		float textureSizeTop_;
		float textureSizeBottom_;

		//texcoord用
		float texLeft_;
		float texRight_;
		float texTop_;
		float texBottom_;

	private://プライベート関数

		/// @brief 頂点バッファの取得
		void VertexBuffer();

		/// @brief マテリアルバッファの取得
		void MaterialBuffer();

		/// @brief インデックスバッファの取得
		void IndexBuffer();

		/// @brief 頂点情報の更新
		void UpdateVertexBuffer();

		/// @brief テクスチャサイズをオリジナルに合わせる
		void AdjustTextureSize();

	};
}

