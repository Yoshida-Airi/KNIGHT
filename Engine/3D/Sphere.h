/**
*	@file Sphere.h
*	@brief 球体クラスヘッダ
*/


#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"MathUtilty.h"
#include"VectorMath.h"
#include"Transform.h"
#include"TextureManager.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"

#include<numbers>
#include<cmath>
#include<assert.h>

#include"Transform.h"

/**
*   @class Sphere
*	@brief  球体クラス
*/
class Sphere
{
public:

	~Sphere();///< デストラクタ

	/// @brief 初期化処理
	/// @param textureHandle テクスチャのハンドル
	void Initialize(uint32_t textureHandle);

	/// @brief 更新処理
	void Update();

	/// @brief 描画処理
	/// @param camera カメラオブジェクト
	void Draw(Camera* camera);



	/// @brief 表示設定
/// @param isInvisible true の場合、球体を非表示にする
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	/// @brief ワールドトランスフォームを取得
	/// @return ワールドトランスフォームへのポインタ
	WorldTransform* GetWorldTransform()const { return worldTransform_; };

	/// @brief 球体の生成
	/// @param textureHandle テクスチャ番号
	/// @return 生成した球体のポインタ
	static Sphere* Create(uint32_t textureHandle);

private:

	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;

	Microsoft::WRL::ComPtr < ID3D12Resource>vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	DirectionalLight* lightData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW materialBufferView_;
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView_;

	uint32_t textureHandle_;

	WorldTransform* worldTransform_ = {};

	//分割数
	const uint32_t kSubdivision_ = 16;

	//経度分割1つ分の角度
	const float kLonEvery_ = static_cast<float>(std::numbers::pi) * 2.0f / float(kSubdivision_);
	//緯度分割1つ分の角度
	const float kLatEvery_ = static_cast<float>(std::numbers::pi) / float(kSubdivision_);

	//uint32_t latIndex = 0;
	//uint32_t lonIndex = 0;

	//球の全ての頂点
	const uint32_t totalVertex_ = 1536;

	bool isInvisible_ = false;

private:
	/// @brief 頂点バッファの取得
	void VertexBuffer();

	/// @brief マテリアルバッファの取得
	void MaterialBuffer();

	/// @brief ライトバッファの取得
	void LightBuffer();
};

