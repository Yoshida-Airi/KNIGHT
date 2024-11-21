/**
*	@file GameObject.h
*	@brief ゲームオブジェクトクラスヘッダ
*/


#pragma once

#include"Model.h"
#include"Collider.h"

/**
*   @class GameObject
*	@brief  ゲームオブジェクトクラス
*/
class GameObject : public Collider
{
protected:
	std::vector<Model*>models_;

public:
	/// @brief 初期化処理
	virtual void Initialize();
	/// @brief 更新処理
	virtual void Update();
	/// @brief 描画処理
	/// @param camera 描画に使用するカメラ
	virtual void Draw(const Camera& camera);
	/// @brief モデルの設定
	/// @param models モデルベクター
	virtual void SetModel(const std::vector<Model*>& models)
	{
		models_ = models;
	}

	/// @brief ワールド座標を取得
	/// @return ワールド座標
	virtual Vector3 GetWorldPosition() override = 0;
	/// @brief AABBを取得
	/// @return AABB
	virtual AABB GetAABB() override = 0;
	/// @brief 衝突時に呼ばれる関数
	/// @param other 衝突した他のCollider
	virtual void OnCollision([[maybe_unused]] Collider* other) override = 0;

};

