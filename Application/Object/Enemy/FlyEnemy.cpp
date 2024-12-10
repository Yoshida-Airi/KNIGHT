#include "FlyEnemy.h"
using namespace AobaraEngine;
void FlyEnemy::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	enemyModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels_ = { enemyModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels_);
	enemyModel_->SetMaterial({ 1.0f,0.0,0.0f,1.0f });

}

void FlyEnemy::Update()
{
	GameObject::Update();

	enemyModel_->ModelDebug("enemy");
	//enemyModel->GetWorldTransform()->translation_.x += 0.03f;

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
	if (traveledDistance_ >= moveDistance_) {
		movingRight_ = false;
	}
	else if (traveledDistance_ <= 0.0f) {
		movingRight_ = true;
	}

	if (isHit_ == false)
	{
		// 垂直方向のふわふわとした動き（サイン波を使用）
		time_ += speed_; // 時間を進める
		enemyModel_->GetWorldTransform()->translation_.y = initialY_ + amplitude_ * sin(time_);
	}


	if (isHit_ == true)
	{

		enemyModel_->GetWorldTransform()->translation_.z += 0.3f;
		enemyModel_->GetWorldTransform()->translation_.y += 0.1f;
		enemyModel_->GetWorldTransform()->rotation_.y += 0.1f;


		if (enemyModel_->GetWorldTransform()->translation_.y >= 10.0f)
		{
			isAlive_ = false;
			isHit_ = false;
		}
	}
}

void FlyEnemy::Draw(const Camera& camera)
{
	if ( isAlive_ == false)
	{
		//死んでいたら描画しない
		enemyModel_->SetisInvisible(true);
	}

	enemyModel_->Draw(camera);
}


Vector3 FlyEnemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB FlyEnemy::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void FlyEnemy::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		isHit_ = true;
	}
}