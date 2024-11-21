#include "Goal.h"

void Goal::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kGoal);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	goalModel_.reset(Model::Create("Resources/Object/Goal/goal.obj"));
	goalModel_->GetWorldTransform()->translation_.x = 5.0f;
	goalModel_->GetWorldTransform()->translation_.y = 4.1f;

	goalModels_ = { goalModel_.get() };

	GameObject::Initialize();
	GameObject::SetModel(goalModels_);
}

void Goal::Update()
{
	GameObject::Update();

	goalModel_->Update();

	goalModel_->ModelDebug("goal");
}

void Goal::Draw(const Camera& camera)
{
	goalModel_->Draw(camera);
}

Vector3 Goal::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = goalModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = goalModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = goalModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Goal::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - goalModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - goalModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - goalModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + goalModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + goalModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + goalModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Goal::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		
	}
}