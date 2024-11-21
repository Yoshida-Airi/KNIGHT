/**
*	@file Collider.h
*	@brief 当たり判定クラスヘッダ
*/

#pragma once
#include"VectorMath.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Object/CollisionConfig.h"

/**
*   @class Collider
*	@brief  当たり判定クラス
*/
class Collider
{
public:

	void Initialize();
	void UpdateWorldTransform();
	void Draw(const Camera& camera);

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	//AABB取得
	virtual AABB GetAABB() = 0;

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

	//半径を取得
	Vector3 GetRadius() const { return radius_; };

	//半径を設定
	void SetRadius(Vector3 radius) { radius_ = radius; };

	////衝突属性(自分)を取得
	//uint32_t GetCollisionAttribute() const { return collisionAttribute_; };
	////衝突属性(自分)を指定
	//void SetCollisionAttribute(uint32_t attribute)
	//{
	//	collisionAttribute_ = attribute;
	//}

	////衝突マスク(相手)を取得
	//uint32_t GetCollisionMask() const { return CollisionMask_; };

	////衝突マスク(相手)を設定
	//void SetCollisionMask(uint32_t mask)
	//{
	//	CollisionMask_ = mask;
	//}

	//種別IDを取得
	uint32_t GetTypeID()const { return typeID_; };
	//種別IDを設定
	void SetTypeID(CollisionTypeDef typeID) { typeID_ = static_cast<uint32_t>(typeID); };

	//コライダータイプIDを取得
	uint32_t GetColliderTypeID()const { return collisionType_; };
	//コライダータイプIDを設定
	void SetColliderTypeID(ColliderType collisionType) { collisionType_ = static_cast<uint32_t>(collisionType); };


private:
	//衝突半径
	Vector3 radius_ = { 1.0f,1.0f,1.0f };
	uint32_t typeID_ = 0u;
	uint32_t collisionType_ = 0u;


	std::unique_ptr<Model>model_;

};