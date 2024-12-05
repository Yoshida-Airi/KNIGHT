/**
*	@file Boss.h
*	@brief ボスクラスヘッダ
*/

#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"
#include"Object/Enemy/EnemyBullet.h"

class Player;

/**
* @class Boss
* @brief ボスキャラクターを制御するクラス
* @details ボスの移動、描画、衝突処理、HP管理などを担当する
*/
class Boss : public AobaraEngine::GameObject
{
public:

	/**
	* @brief ボスの初期化処理
	* @details モデルや状態の初期設定を行う
	*/
	void Initialize()override;

	/**
	* @brief ボスの更新処理
	* @details 毎フレーム呼ばれ、移動やHPの管理、無敵状態の更新などを行う
	*/
	void Update()override;

	/**
	* @brief ボスの描画処理
	* @param camera カメラ情報
	* @details モデルをカメラに合わせて描画する
	*/
	void Draw(const AobaraEngine::Camera& camera)override;

	/**
	* @brief ボスの位置を設定する
	* @param position 新しい位置
	* @details 指定した位置にボスのモデルを移動させる
	*/
	void SetPosition(Vector3 position)
	{
		enemyModel_->GetWorldTransform()->translation_ = position;
	}

	/**
	* @brief プレイヤー情報取得
	* @param player プレイヤー情報
	*/
	void SetPlayer(Player* player)
	{
		player_ = player;
	}

	/**
	* @brief ボスのワールド座標を取得する
	* @return ボスのワールド座標
	*/
	Vector3 GetWorldPosition()override;

	/**
	* @brief ボスのAABB（軸に平行なバウンディングボックス）を取得する
	* @return AABB ボスのバウンディングボックス
	*/
	AABB GetAABB()override;

	/**
	* @brief 衝突時の処理
	* @param other 衝突したコライダー
	* @details 他のオブジェクトとの衝突時に呼び出される処理
	*/
	void OnCollision([[maybe_unused]] Collider* other)override;

	/**
	* @brief 生存状態を取得する
	* @return bool 生きているかどうか
	*/
	bool GetIsAlive()
	{
		return isAlive_;
	}

	enum class Phase
	{
		kWait,
		kWork,
		kAttack,
		kDeath,
	};


	/**
	* @brief フェーズの変更
	* @param[in] phase 変更するフェーズ
	*/
	void ChangePhase(Phase phase);




private:

	std::unique_ptr<Model>enemyModel_;
	std::vector<Model*>enemyModels_;

	bool isAlive_ = true;	//生きているか: true 生きている

	float moveSpeed_ = 0.03f;  // 移動速度
	float moveDistance_ = 15.0f;  // 移動する距離
	float traveledDistance_ = 0.0f;  // 移動した距離
	bool movingRight_ = true;  // 右方向に移動しているかどうか

	int32_t hp_ = 3;
	bool isInvincible_;
	float invincibilityTimer_;
	static constexpr float invincibilityDuration_ = 1.0f; // 無敵時間の長さ（秒）

	Phase phase_;

	float amplitude_ = 0.2f;  // 浮く高さを1.0fに設定
	float speed_ = 0.05f;      // 浮く速度をゆっくりとした0.1fに設定
	float initialY_;  // 現在のy座標を初期値に設定
	float time_ = 0.0f;            // 時間の初期値を0に設定

	Player* player_;

private:

	void WaitPhase();
	void AttackPhase();
	void WorkPhase();
	void DeathPhase();
};

