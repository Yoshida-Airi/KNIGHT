#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

class Player;
class EnemyBullet : public AobaraEngine::GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(const AobaraEngine::Camera& camera)override;
	AABB GetAABB()override;

	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 弾のデスフラグが立っているか
	/// </summary>
	/// <returns>true : 立っている</returns>
	bool IsDead()const { return isDead_; };

	//セッター
	void SetPlayer(Player* player) { player_ = player; };

	void OnCollision([[maybe_unused]] AobaraEngine::Collider* other)override;

	void SetVelocity(Vector3 velocity)
	{
		velocity_ = velocity;
	}

	void SetPosition(Vector3 pos)
	{
		bulletModel_->GetWorldTransform()->translation_ = pos;
	}

	//void SetPlayer(Player* player) { player_ = player; }

private:

	std::unique_ptr<AobaraEngine::Model>bulletModel_;
	std::vector<AobaraEngine::Model*>bulletModels_;

	//速度
	Vector3 velocity_;
	static const int32_t kLifeTime_ = 60 * 5;
	int32_t deathTimer_ = kLifeTime_;
	bool isDead_ = false;

	Player* player_ = nullptr;
};
