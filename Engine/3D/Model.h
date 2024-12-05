/**
*	@file Model.h
*	@brief モデルクラスヘッダ
*/


#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"ModelLoader.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"
#include"ModelData.h"
#include"Animation.h"
#include"SrvManager.h"
#include<span>

const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence
{
	std::array<float, kNumMaxInfluence>weights;
	std::array<int32_t, kNumMaxInfluence>jointIndices;
};

struct WellForGPU
{
	Matrix4x4 skeletonSpaceMatrix;	//位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;	//法線用
};

struct SkinCluster
{
	std::vector<Matrix4x4>inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource>influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence>mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource>palatteResource;
	std::span<WellForGPU>mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, CD3DX12_GPU_DESCRIPTOR_HANDLE>paletteSrvHandle;
};

/**
*   @class DeathEffect
*	@brief  モデルクラス
*/
namespace AobaraEngine
{
	class Model
	{
	public:
		~Model(); ///< デストラクタ

		/// @brief モデルの初期化
		/// @param filename モデルファイルのパス
		void Initialize(const std::string& filename);

		/// @brief モデルの更新処理
		void Update();

		/// @brief モデルの描画処理
		/// @param camera 使用するカメラ
		void Draw(const AobaraEngine::Camera& camera);

		/// @brief 表示切り替え
		/// @param isInvisible true:表示シない
		void SetisInvisible(bool isInvisible)
		{
			isInvisible_ = isInvisible;
		}

		/// @brief アニメーションの使用設定
		/// @param isUse true:アニメーション使用する
		void UseAnimation(bool isUse)
		{
			animationData_.isValid = isUse;
		}

		/// @brief モデルを生成
		/// @param filename モデルファイルのパス
		/// @return 生成したモデル
		static Model* Create(const std::string& filename);

		/// @brief デバッグ情報の表示
		/// @param name モデル名
		void ModelDebug(const char* name);

		/// @brief 親子関係を結ぶ
		/// @param model 親のモデル
		void Parent(Model* model);

		/// @brief ワールドトランスフォームを取得
		/// @return ワールドトランスフォーム
		WorldTransform* GetWorldTransform()const { return worldTransform_; };

		/// @brief ワールドトランスフォームを設定
		/// @param worldTransform 設定するワールドトランスフォーム
		void SetWorldTransform(WorldTransform* worldTransform)
		{
			worldTransform_ = worldTransform;
		}

		/// @brief アニメーションデータを設定
		/// @param animationData アニメーションデータ
		void SetAnimation(AnimationData animationData)
		{
			animationData_ = animationData;
		}

		/// @brief アニメーションを移動させるフラグの設定
		/// @param isActive アニメーションが有効な場合は true
		void MoveAnimation(bool isActive)
		{
			isActiveAnimation_ = isActive;
		}

		/// @brief マテリアルデータを設定
		/// @param material マテリアルの色
		void SetMaterial(Vector4 material)
		{
			materialData_->color = material;
		}

		/// @brief ライトの設定
		/// @param isLight ライトを有効にする場合は true
		void SetLight(uint32_t isLight)
		{
			materialData_->enableLighting = isLight;
		}

	private:
		DirectXCommon* dxCommon_;
		GraphicsPipelineManager* psoManager_;
		TextureManager* texture_;
		ModelLoader* modelLoader_;
		Animation* animation_;
		SrvManager* srvManager_;

		WorldTransform* worldTransform_;

		ModelData modelData_;

		Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
		Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
		Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;
		Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;


		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		VertexData* vertexData_ = nullptr;
		Material* materialData_ = nullptr;	//マテリアルデータ
		DirectionalLight* lightData_ = nullptr;
		uint32_t* indexData_ = nullptr;

		uint32_t textureHandle_;
		bool isInvisible_ = false;


		AnimationData animationData_;
		Skeleton skelton_;
		SkinCluster skinCluster_;
		float animationTime_ = 0.0f;

		bool isActiveAnimation_ = true;


	private:

		/// @brief 頂点のバッファを取得
		void VertexBuffer();

		/// @brief マテリアルのバッファを取得
		void MaterialBuffer();

		/// @brief ライトのバッファを取得
		void LightBuffer();

		/// @brief インデックスバッファを取得
		void IndexBuffer();

		/// @brief スキンクラスターを生成
		/// @param device DirectXデバイス
		/// @param skeleton スケルトンデータ
		/// @return 生成したスキンクラスター
		SkinCluster CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton/*, const ModelData& modelData*/);

		/// @brief クラスターを更新
		/// @param skinCluster 更新するスキンクラスター
		/// @param skelton スケルトンデータ
		void ClasterUpdate(SkinCluster& skinCluster, const Skeleton& skeltion);

	};

}