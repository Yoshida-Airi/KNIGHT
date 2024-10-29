#include "Boss.h"
#include"Object/CollisionConfig.h"
#include"Object/Player/Player.h"

void Boss::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);
	Collider::SetRadius({1.0f,1.0f,1.0f});

	enemyModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels_ = { enemyModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels_);
	enemyModel_->GetWorldTransform()->scale_ = { 2.0f, 2.0f, 2.0f };
	enemyModel_->SetMaterial({ 1.0f,0.0,0.0f,1.0f });
	enemyModel_->GetWorldTransform()->translation_ = { 19.3f,60.9f,0.0f };

	phase_ = Phase::kWait;

	

}

void Boss::Update()
{
	GameObject::Update();

	enemyModel_->ModelDebug("enemy");

#ifdef _DEBUG
	ImGui::Text("BossHP %d", hp_);
#endif // _DEBUG


	//enemyModel->GetWorldTransform()->translation_.x += 0.03f;

	

	switch (phase_)
	{
	case Boss::Phase::kWait:
		//ゲームプレイフェーズの処理
		WaitPhase();
		break;
	case Boss::Phase::kAttack:
		//自キャラ死亡時の処理
		AttackPhase();
		break;
	case Boss::Phase::kWork:
		WorkPhase();
		break;
	case Boss::Phase::kDeath:
		//自キャラ死亡時の処理
		DeathPhase();
		break;
	}

}

void Boss::Draw(Camera* camera)
{
	if (isAlive_ == false)
	{
		//死んでいたら描画しない
		enemyModel_->SetisInvisible(true);
	}

	enemyModel_->Draw(camera);
}


Vector3 Boss::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Boss::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Boss::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		if (!isInvincible_)
		{
			hp_ -= 1;
			isInvincible_ = true;
			invincibilityTimer_ = invincibilityDuration_;
		}
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayer))
	{
		if (phase_==Phase::kWait)
		{
			ChangePhase(Phase::kWork);
		}
	}
}

void Boss::ChangePhase(Phase phase)
{
	phase_ = phase;
}

void Boss::WaitPhase()
{
	
}

void Boss::AttackPhase()
{
	Vector3 playerPosition = player_->GetWorldTransform()->translation_;

	const float kBulletSpeed = 1.0f;

	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 differenceVector;
	differenceVector.x = playerPosition.x - enemyModel_->GetWorldTransform()->translation_.x;
	differenceVector.y = playerPosition.y - enemyModel_->GetWorldTransform()->translation_.y;
	differenceVector.z = playerPosition.z - enemyModel_->GetWorldTransform()->translation_.z;

	// 正規化
	Vector3 normarizeVector;
	normarizeVector = Normalize(differenceVector);

	// ベクトルの長さを速さに合わせる
	Vector3 velocity;
	velocity.x = normarizeVector.x * kBulletSpeed;
	velocity.y = normarizeVector.y * kBulletSpeed;
	velocity.z = normarizeVector.z * kBulletSpeed;

	////球を生成し、初期化
	//EnemyBullet* newBullet = new EnemyBullet();
	//newBullet->SetPlayer(player_);
	//newBullet->Initialize();


	////弾の登録
	//gameScene_->AddEnemyBullet(newBullet);

	//無敵時間
	if (isInvincible_)
	{
		invincibilityTimer_ -= 1.0f / 60.0f;
		if (invincibilityTimer_ <= 0.0f)
		{
			isInvincible_ = false;
		}
	}
}

void Boss::WorkPhase()
{
	// 移動距離を更新
	if (movingRight_) {
		enemyModel_->GetWorldTransform()->translation_.x += moveSpeed_;
		traveledDistance_ += moveSpeed_;
	}
	else {
		enemyModel_->GetWorldTransform()->translation_.x -= moveSpeed_;
		traveledDistance_ -= moveSpeed_;
	}

	// 一定の距離を移動したら方向を反転
	//if (traveledDistance_ >= moveDistance_) {
	//	movingRight_ = false;
	//}
	//else if (traveledDistance_ <= 0.0f) {
	//	movingRight_ = true;
	//}

	if (enemyModel_->GetWorldTransform()->translation_.x >= 28.0f)
	{
		movingRight_ = false;
	}
	else if (enemyModel_->GetWorldTransform()->translation_.x <= 3.0f)
	{
		movingRight_ = true;
	}

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	//無敵時間
	if (isInvincible_)
	{
		invincibilityTimer_ -= 1.0f / 60.0f;
		if (invincibilityTimer_ <= 0.0f)
		{
			isInvincible_ = false;
		}
	}

	// 垂直方向のふわふわとした動き（サイン波を使用）
	time_ += speed_; // 時間を進める
	initialY_ = enemyModel_->GetWorldTransform()->translation_.y;
	enemyModel_->GetWorldTransform()->translation_.y = initialY_ + amplitude_ * sin(time_);

	if (time_ == 60.0f)
	{
		ChangePhase(Phase::kAttack);
	}

}

void Boss::DeathPhase()
{
	//無敵時間
	if (isInvincible_)
	{
		invincibilityTimer_ -= 1.0f / 60.0f;
		if (invincibilityTimer_ <= 0.0f)
		{
			isInvincible_ = false;
		}
	}
}
