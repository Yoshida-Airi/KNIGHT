/**
*	@file CollisionManager.h
*	@brief 当たり判定マネージャークラスヘッダ
*/

#pragma once
#include<list>
#include"Collider.h"
#include"MathUtilty.h"
#include"Model.h"

/**
*   @class CollisionManager
*	@brief  当たり判定管理クラス
*/
class CollisionManager {

public:

	void Initialize();
	void UpdateWorldTransform();
	void Draw(AobaraEngine::Camera* camera);

	/// <summary>
	/// リストをクリアする
	/// </summary>
	void ListClear();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void ChackAllCollisions();

	/// <summary>
	/// コライダーをリストに登録する
	/// </summary>
	/// <param name="collider">オブジェクト</param>
	void AddColliders(AobaraEngine::Collider* collider);


private:
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheakCollisionPair(AobaraEngine::Collider* colliderA, AobaraEngine::Collider* colliderB);

	void CheackSphereCollision();

	// コライダーリスト
	std::list<AobaraEngine::Collider*> colliders_;

	

};