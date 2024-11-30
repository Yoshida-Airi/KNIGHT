#include "CollisionManager.h"
#include"Object/CollisionConfig.h"

void CollisionManager::Initialize()
{
#ifdef _DEBUG

	for (Collider* collider : colliders_)
	{
		collider->Initialize();
	}
#endif // _DEBUG

}

void CollisionManager::UpdateWorldTransform()
{
#ifdef _DEBUG

	for (Collider* collider : colliders_)
	{
		collider->UpdateWorldTransform();
	}
#endif // _DEBUG

}

void CollisionManager::Draw(Camera* camera)
{
#ifdef _DEBUG
	for (Collider* collider : colliders_)
	{
		collider->Draw(*camera);
	}
#endif // _DEBUG

	
}

void CollisionManager::ListClear()
{
	colliders_.clear();
}

/// 衝突判定と応答
void CollisionManager::ChackAllCollisions() 
{

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			// コライダーAトコライダーBの当たり判定
			CheakCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheakCollisionPair(Collider* colliderA, Collider* colliderB)
{

	//球どうしの当たり判定
	if (colliderA->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE) &&
		colliderB->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE))
	{
		// コライダーAのワールド座標を取得
		Vector3 posA = colliderA->GetWorldPosition();
		// コライダーBのワールド座標を取得
		Vector3 posB = colliderB->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };

		// 各軸方向の距離の二乗を計算
		float distanceSquaredX = distance.x * distance.x;
		float distanceSquaredY = distance.y * distance.y;
		float distanceSquaredZ = distance.z * distance.z;

		// 各軸方向の半径を取得
		Vector3 radiusA = colliderA->GetRadius();
		Vector3 radiusB = colliderB->GetRadius();


		// 各軸方向の半径の和を計算
		float radiusSumX = radiusA.x + radiusB.x;
		float radiusSumY = radiusA.y + radiusB.y;
		float radiusSumZ = radiusA.z + radiusB.z;

		// 球と球の交差判定
		if (distanceSquaredX / (radiusSumX * radiusSumX) +
			distanceSquaredY / (radiusSumY * radiusSumY) +
			distanceSquaredZ / (radiusSumZ * radiusSumZ) <= 1.0f)
		{
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA);
		}
	}


	//球と四角の当たり判定
	if (colliderA->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::AABB) &&
		colliderB->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE) ||
		colliderA->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE) &&
		colliderB->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::AABB))
	{

		SphereData sphere;
		AABB aabb;

		if (colliderA->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE))
		{
			sphere.center = colliderA->GetWorldPosition();
			sphere.radius = colliderA->GetRadius();
		}
		else if (colliderB->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::SPHERE))
		{
			sphere.center = colliderB->GetWorldPosition();
			sphere.radius = colliderB->GetRadius();
		}

		if (colliderA->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::AABB))
		{
			aabb = colliderA->GetAABB();
		}
		else if (colliderB->GetColliderTypeID() == static_cast<uint32_t>(ColliderType::AABB))
		{
			aabb = colliderB->GetAABB();
		}

		if (IsCollision(aabb, sphere))
		{
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA);
		}
	}


}

void CollisionManager::CheackSphereCollision()
{

}

void CollisionManager::AddColliders(Collider* collider)
{
	//リストに登録する
	colliders_.push_back(collider);
}