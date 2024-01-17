#pragma once
#include"Model.h"
#include"Collider.h"

class PlayerBullet :public Collider
{
public:
	void Initialize(Vector3 pos, const Vector3& velocity);
	void Update();
	void Draw(ICamera* camera);

	/// <summary>
	/// 弾のですフラグが立っているか
	/// </summary>
	/// <returns>true : 立っている</returns>
	bool IsDead()const { return isDead_; };

	void OnCollision()override;

	Vector3 GetWorldPosition()override;

private:

	std::unique_ptr<Model> bulletModel_ = nullptr;

	//速度
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

};
