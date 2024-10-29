#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

class Player;
class EnemyBullet : public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;

	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 弾のデスフラグが立っているか
	/// </summary>
	/// <returns>true : 立っている</returns>
	bool IsDead()const { return isDead_; };

	//セッター
	void SetPlayer(Player* player) { player_ = player; };

	void OnCollision([[maybe_unused]] Collider* other)override;

	void SetPosition(Vector3 pos)
	{
		bulletModel_->GetWorldTransform()->translation_ = pos;
	}

private:

	std::unique_ptr<Model>bulletModel_;
	std::vector<Model*>bulletModels_;

	//速度
	Vector3 velocity_;
	static const int32_t kLifeTime_ = 60 * 5;
	int32_t deathTimer_ = kLifeTime_;
	bool isDead_ = false;

	Player* player_ = nullptr;
};
