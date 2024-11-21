/**
*	@file FlyEnemy.h
*	@brief 飛んでいる敵クラスヘッダ
*/

#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

/**
* @class FlyEnemy
* @brief 飛んでいる敵キャラクターを制御するクラス
* @details 敵の移動、描画、衝突処理、浮遊アニメーションを管理する
*/
class FlyEnemy : public GameObject
{
public:

	/**
	* @brief 飛んでいる敵の初期化処理
	* @details モデルや初期位置などの設定を行う
	*/
	void Initialize()override;

	/**
	* @brief 飛んでいる敵の更新処理
	* @details 毎フレーム呼ばれ、移動や浮遊アニメーションの更新を行う
	*/
	void Update()override;

	/**
	* @brief 飛んでいる敵の描画処理
	* @param camera カメラ情報
	* @details モデルをカメラに合わせて描画する
	*/
	void Draw(const Camera& camera)override;

	/**
	* @brief 飛んでいる敵の位置を設定する
	* @param position 新しい位置
	* @details 指定した位置にモデルを移動させ、y座標の初期値を設定する
	*/
	void SetPosition(Vector3 position)
	{
		enemyModel_->GetWorldTransform()->translation_ = position;
		initialY_ = enemyModel_->GetWorldTransform()->translation_.y;
	}


	/**
	* @brief ワールド座標を取得する
	* @return 敵のワールド座標
	*/
	Vector3 GetWorldPosition()override;

	/**
	* @brief 敵のAABB（軸に平行なバウンディングボックス）を取得する
	* @return AABB 敵のバウンディングボックス
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

private:

	std::unique_ptr<Model>enemyModel_;
	std::vector<Model*>enemyModels_;

	bool isAlive_ = true;	//生きているか: true 生きている

	float moveSpeed_ = 0.01f;  // 移動速度
	float moveDistance_ = 5.0f;  // 移動する距離
	float traveledDistance_ = 0.0f;  // 移動した距離
	bool movingRight_ = true;  // 右方向に移動しているかどうか

	float amplitude_ = 1.0f;  // 浮く高さを1.0fに設定
	float speed_ = 0.05f;      // 浮く速度をゆっくりとした0.1fに設定
	float initialY_;  // 現在のy座標を初期値に設定
	float time_ = 0.0f;            // 時間の初期値を0に設定
};

