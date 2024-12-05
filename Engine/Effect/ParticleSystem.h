/**
*	@file ParticleSystem.h
*	@brief パーティクルクラスヘッダ
*/

#pragma once
#include"DirectXCommon.h"
#include"SrvManager.h"
#include"TextureManager.h"
#include"VectorMath.h"
#include"Globals.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include"Transform.h"
#include"WorldTransform.h"
#include"GraphicsPipelineManager.h"
#include<wrl.h>

#include"Camera.h"

#include<random>
#include<numbers>



struct Particle
{
	EulerTransform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct Emitter
{
	EulerTransform transform;	//エミッタのTransform
	uint32_t count;			//発生数
	float frequency;		//発生頻度
	float frequencyTime;	//頻度用時刻
};

struct ParticleForGPU
{
	Matrix4x4 WVP;
	Vector4 color;
};

struct Time
{
	float min;
	float max;
};


/**
*   @class ParticleSystem
*	@brief  パーティクルシステムクラス
*/
class ParticleSystem
{
public:
	~ParticleSystem();

	void Initialize(uint32_t textureHandle, AobaraEngine::Camera* camera, Vector3 velocity, bool isRandomPosition);
	void Update();
	void Draw();


	/// <summary>
	/// マテリアルデータの設定
	/// </summary>
	/// <param name="color"></param>
	void SetMaterialData(const Vector4 color);

	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU)
	{
		textureSrvHandleGPU_ = textureSrvHandleGPU;
	}

	//描画するかしないか　true : 消す
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}


	/// <summary>
	/// テクスチャの左上座標（画像上の描画したい左上座標
	/// </summary>
	/// <param name="leftTop"></param>
	void SetTextureLeftTop(Vector2 leftTop)
	{
		textureLeftTop = leftTop;
	}

	/// <summary>
	/// テクスチャサイズ（切り取って表示したいサイズ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(Vector2 size)
	{
		cutSize_ = size;
	}

	/// <summary>
	/// スプライトのの基準点
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(Vector2 anchorPoint)
	{
		anchorPoint_ = anchorPoint;
	}

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="textureHandle">テクスチャ番号</param>
	/// <param name="camera">カメラ</param>
	/// <param name="velocity">速度</param>
	/// <param name="isRandomPosition">ランダムな位置に置くか　true : 置く</param>
	/// <returns>パーティクル</returns>
	static ParticleSystem* Create(uint32_t textureHandle, AobaraEngine::Camera* camera, Vector3 velocity, bool isRandomPosition);

	/// <summary>
	/// 生存時間の設定
	/// </summary>
	/// <param name="timeMin">一番短く消したい時間</param>
	/// <param name="timeMax">一番長く存在していい時間</param>
	void SetLifeTime(float timeMin, float timeMax)
	{
		lifeTime_.min = timeMin;
		lifeTime_.max = timeMax;
	}

	/// <summary>
	/// ビルボードを使用するかの設定
	/// </summary>
	void SetUseBillBoard()
	{
		isBillboard_ = true;
	}

	/// <summary>
	/// Imgui
	/// </summary>
	void Debug(const char* name);

	/// <summary>
	/// 新しく粒を生成するのを防ぐ
	/// </summary>
	void StopMakeParticle();

	void MoveMakeParticle();

	//ランダムな速度に動かす
	void SetRandomAllVelocity();	//すべての方向にランダムな速度で
	void SetRandomVelocityX(bool isMove);		//X軸のみランダムな速度で
	void SetRandomVelocityY(bool isMove);		//y軸のみランダムな速度で
	void SetRandomVelocityZ(bool isMove);		//z軸のもランダムな速度で

	/// <summary>
	/// パーティクルの粒の存在が０のときtrueを返す
	/// </summary>
	/// <returns></returns>
	bool GetIsParticleEmpty();

	/// <summary>
	/// パーティクルひと粒のカラーの設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(Vector3 color);

	/// <summary>
	/// パーティクルひと粒の色をランダムに変える
	/// </summary>
	void SetRandomColor();

	void SetParitcleScale(Vector3 scale);

	Emitter* emitter_ = new Emitter();
private://プライベート変数

	AobaraEngine::GraphicsPipelineManager* psoManager_ = nullptr;
	AobaraEngine::SrvManager* srvManager_ = nullptr;

	static const uint32_t kNumMaxInstance_ = 500;
	uint32_t numInstance_ = 0;

	DirectXCommon* dxCommon_;
	AobaraEngine::TextureManager* texture_;
	AobaraEngine::Camera* camera_ = nullptr;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> indexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource>instancingResources_;

	D3D12_RESOURCE_DESC resourceDesc_{};	//テクスチャの情報

	Matrix4x4* transformationMatrixData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	VertexData* vertexData_ = nullptr;	//頂点データ
	Material* materialData_ = nullptr;	//マテリアルデータ
	uint32_t* indexData_ = nullptr;		//インデックスデータ
	ParticleForGPU* instancingData_ = nullptr;

	EulerTransform uvTransform_;
	Vector2 textureSize_;	//切り出しサイズ

	bool isInvisible_ = false;	//非表示フラグ	true : 消える

	uint32_t textureHandle_;

	Vector2 cutSize_ = { 100.0f,100.0f };
	Vector2 textureLeftTop = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	//画像のサイズ
	float left_;
	float right_;
	float top_;
	float bottom_;

	//texcoord用
	float texLeft_;
	float texRight_;
	float texTop_;
	float texBottom_;

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

	std::list<Particle> particles_;

	bool isRandomPosition_ = false;
	Vector3 velocity_;
	Time lifeTime_ = { 1.0,3.0f };
	bool isBillboard_ = false;

	bool isMakeParticle_ = true;	//新しく粒を生成するか。

	const float kDeltaTime_ = 1.0f / 60.0f;

	bool isRandomAllVelocity_ = false;
	bool isRandomVelocityX_ = false;
	bool isRandomVelocityY_ = false;
	bool isRandomVelocityZ_ = false;

	Vector3 particleColor_ = { 1.0f,1.0f,1.0f };
	bool isRandomColor_ = false;

	Vector3 particleScale_ = { 0.005f ,0.005f ,0.005f };

private://プライベート関数

	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void VertexBuffer();

	/// <summary>
	/// マテリアルのバッファの取得
	/// </summary>
	void MaterialBuffer();

	/// <summary>
	/// index用
	/// </summary>
	void IndexBuffer();

	/// <summary>
	/// 頂点情報の更新
	/// </summary>
	void UpdateVertexBuffer();

	/// <summary>
	/// テクスチャサイズをオリジナルに合わせる
	/// </summary>
	void AdjustTextureSize();

	void InstancingBuffer();

	void SetSRV();

	Particle MakeNewParticle(std::mt19937& randomEngine, Emitter* emitter, Vector3 velocity, bool isRandamTranslata);

	std::list<Particle>Emission(Emitter* emitter, std::mt19937& randomEngine, Vector3 velocity, bool isRandamTranslata);

};