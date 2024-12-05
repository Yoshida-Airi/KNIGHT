/**
*	@file Weapon.h
*	@brief 武器クラスヘッダ
*/

#pragma once
#include"GameObject.h"

/**
* @class Weapon
* @brief 武器クラス
* @details 武器の動作と描画を管理し、攻撃状態や親オブジェクトとの関連を設定する。
*/
class Weapon :public GameObject
{
public:

	/**
	* @brief 初期化処理
	* @details 武器の初期化を行う。
	*/
	void Initialize()override;

	/**
	* @brief 更新処理
	* @details 武器の更新処理を行う。攻撃状態などが反映される。
	*/
	void Update()override;

	/**
	* @brief 描画処理
	* @param camera カメラ
	* @details 武器の描画を行う。
	*/
	void Draw(const AobaraEngine::Camera& camera)override;

	/**
	* @brief ワールド座標を取得
	* @return 武器のワールド座標
	*/
	Vector3 GetWorldPosition()override;

	/**
	* @brief AABB（軸に平行な境界ボックス）を取得
	* @return AABB
	*/
	AABB GetAABB()override;

	/**
	* @brief 衝突時の処理
	* @param other 衝突相手のコライダー
	* @details 衝突が発生した際の処理を定義する。
	*/
	[[maybe_unused]] void OnCollision([[maybe_unused]] Collider* other)override;

	/**
	* @brief 親オブジェクトを設定
	* @param model 親モデルオブジェクト
	* @details 武器の親オブジェクトを設定し、親モデルに対して武器が追従するようにする。
	*/
	void SetParent(Model* model)
	{
		weaponModel_->Parent(model);
	};

	/**
	* @brief 攻撃状態の設定
	* @param isAttack 攻撃中かどうかのフラグ
	* @details 攻撃中であるかどうかを設定する。
	*/
	void SetIsAttack(bool isAttack)
	{
		isAttack_ = isAttack;
	}

	/**
	* @brief 攻撃状態の取得
	* @return 攻撃中かどうか
	*/
	bool GetIsAttack()
	{
		return isAttack_;
	}

private:
	std::unique_ptr<Model>weaponModel_;
	std::vector<Model*>weaponModels_;

	bool isAttack_ = false; //攻撃中かどうか : ture 攻撃中
};

