/**
*	@file DeathEffect.h
*	@brief デスエフェクトクラスヘッダ
*/


#pragma once
#include"ParticleSystem.h"

/** 
*   @class DeathEffect
*	@brief  何かが死んだときのパーティクル制御クラス
*/
class DeathEffect
{
public:

	/**
	* @brief デスエフェクトの初期化関数
	* @param camera->カメラ
	* @details カメラ情報を使用して、パーティクルエフェクトの表示位置や動作を設定する初期化処理を行う。
	*/
	void Initialize(AobaraEngine::Camera* camera);

	/**
	* @brief デスエフェクトの更新処理
	* @details 毎フレーム実行され、エフェクトの進行を管理する。エフェクトの表示時間が過ぎると、消滅フラグを立てる。
	*/
	void Update();

	/**
	* @brief デスエフェクトの描画処理
	* @details パーティクルシステムを使って、デスエフェクトを描画する処理を実行する。
	*/
	void Draw();

	/**
	* @brief エフェクトを実行するかどうかを設定する
	* @param isFlag->エフェクトを実行するかどうかのフラグ（trueなら実行、falseなら非実行）
	* @details この関数でフラグを設定し、エフェクトの実行状態を制御する。
	*/
	void SetFlag(bool isFlag)
	{
		flag_ = isFlag;
	}

	/**
	* @brief デスエフェクトの表示位置を設定する
	* @param position->デスエフェクトのエミッター位置
	* @details パーティクルのエミッター位置を設定する。
	*/
	void SetPosition(Vector3 position)
	{
		hitEffect_->emitter_->transform.translate.x = position.x;
		hitEffect_->emitter_->transform.translate.y = position.y + 0.2f;
		hitEffect_->emitter_->transform.translate.z = position.z;
	}

	/**
	* @brief デスエフェクトが終了（消滅）したかどうかを判定する
	* @return bool->エフェクトが終了した場合はtrue、まだ表示中ならfalse
	* @details エフェクトの表示時間が終了し、消滅したかどうかを確認するための関数。
	*/
	bool IsDead();

private:

	//! テクスチャ管理用のポインタ
	AobaraEngine::TextureManager* textureManager_ = nullptr;
	//! パーティクルシステムのインスタンス 
	std::unique_ptr<ParticleSystem> hitEffect_ = nullptr;
	//! 使用する円形テクスチャのID 
	uint32_t circleTexture_;

	//! エフェクトが実行中かどうかを示すフラグ 
	bool flag_;	
	//!  エフェクトの再生時間（フレーム数）
	int32_t playTime_ = 20;
	//! エフェクトが消滅したかどうかのフラグ
	bool isDead_ = false;


};
