/**
*	@file Enemy.h
*	@brief 敵クラスヘッダ
*/

#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"
#include"Object/Enemy/EnemyBullet.h"
#include"Utility/TimedCall.h"


class Player;
/**
* @class Enemy
* @brief 敵キャラクターの制御を行うクラス
* @details 敵キャラクターの描画、移動、衝突判定などの処理を管理する
*/
class Enemy : public AobaraEngine::GameObject
{
public:
	~Enemy();

	/**
	* @brief 敵キャラクターの初期化処理
	* @details 敵キャラクターのモデルや初期状態を設定する
	*/
	void Initialize()override;

	/**
	* @brief 敵キャラクターの更新処理
	* @details 毎フレーム呼ばれ、敵キャラクターの移動や状態の更新を行う
	*/
	void Update()override;

	/**
	* @brief 敵キャラクターの描画処理
	* @param camera カメラ情報
	* @details 敵キャラクターを画面に描画する
	*/
	void Draw(const AobaraEngine::Camera& camera)override;

	/**
	* @brief 敵キャラクターの位置を設定する
	* @param position 敵キャラクターの新しい位置
	* @details 指定した位置に敵キャラクターを配置する
	*/
	void SetPosition(Vector3 position)
	{
		enemyModel_->GetWorldTransform()->translation_ = position;
	}

	/**
	* @brief 敵キャラクターのワールド座標を取得する
	* @return 敵キャラクターのワールド座標
	*/
	Vector3 GetWorldPosition()override;

	/**
	* @brief 敵キャラクターのAABB（軸に平行なバウンディングボックス）を取得する
	* @return AABB 敵キャラクターのバウンディングボックス
	*/
	AABB GetAABB()override;

	/**
	* @brief 衝突時の処理
	* @param other 衝突したコライダー
	* @details 敵キャラクターが他のオブジェクトと衝突した際の処理を行う
	*/
	void OnCollision([[maybe_unused]] AobaraEngine::Collider* other)override;

	void AttackReset();

	void DeleteBullet();

	void Fire();

	/**
	* @brief 敵キャラクターが生きているかを判定する
	* @return true 生きている場合、false 死んでいる場合
	*/
	bool GetIsAlive()
	{
		return isAlive_;
	}

	void SetPlayer(Player* player) { player_ = player; };

private:

	std::unique_ptr<AobaraEngine::Model>enemyModel_;
	std::vector<AobaraEngine::Model*>enemyModels_;

	////弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;

	bool isAlive_ = true;	//生きているか: true 生きている
	bool isHit = false;


	//発射間隔
	static const int kFireInterval = 180;
	float moveSpeed_ = 0.03f;  // 移動速度
	float moveDistance_ = 5.0f;  // 移動する距離
	float traveledDistance_ = 0.0f;  // 移動した距離
	bool movingRight_ = true;  // 右方向に移動しているかどうか
	Player* player_ = nullptr;
};

