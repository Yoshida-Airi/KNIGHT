#include "Weapon.h"
#include"Object/CollisionConfig.h"

void Weapon::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kWeapon);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	weaponModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	weaponModels_ = { weaponModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(weaponModels_);

	weaponModel_->GetWorldTransform()->translation_.z += 2.0f;
	weaponModel_->GetWorldTransform()->translation_.y += 2.0f;
	weaponModel_->SetMaterial({ 0.0f,0.0,1.0f,1.0f });
}

void Weapon::Update()
{
	GameObject::Update();

	weaponModel_->ModelDebug("weapon");
	//weaponModel->GetWorldTransform()->translation_.x += 0.03f;
}

void Weapon::Draw(Camera* camera)
{
	if (isAttack_ == false)
	{
		//攻撃してないときは武器を表示しない
		weaponModel_->SetisInvisible(true);
	}
	else
	{
		weaponModel_->SetisInvisible(false);
	}
	weaponModel_->Draw(camera);
}

Vector3 Weapon::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = weaponModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = weaponModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = weaponModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Weapon::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - weaponModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - weaponModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - weaponModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + weaponModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + weaponModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + weaponModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

[[maybe_unused]]
void Weapon::OnCollision([[maybe_unused]] Collider* other)
{
}
