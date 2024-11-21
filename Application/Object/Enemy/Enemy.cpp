#include "Enemy.h"
#include"Object/CollisionConfig.h"

void Enemy::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	enemyModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels_ = { enemyModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels_);
	enemyModel_->SetMaterial({ 1.0f,0.0,0.0f,1.0f });
}

void Enemy::Update()
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

}

void Enemy::Draw(const Camera& camera)
{
	if (isAlive_ == false)
	{
		//死んでいたら描画しない
		enemyModel_->SetisInvisible(true);
	}

	enemyModel_->Draw(camera);
}


Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Enemy::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Enemy::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		isAlive_ = false;
	}
}