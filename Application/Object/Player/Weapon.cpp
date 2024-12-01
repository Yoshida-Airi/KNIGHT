#include "Weapon.h"
#include"Object/CollisionConfig.h"

void Weapon::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kWeapon);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	weaponModel_.reset(Model::Create("Resources/Object/sword.obj"));
	weaponModels_ = { weaponModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(weaponModels_);

	weaponModel_->GetWorldTransform()->translation_.z += 1.2f;
	weaponModel_->GetWorldTransform()->translation_.y += 2.0f;
	weaponModel_->GetWorldTransform()->translation_.x = -1.0f;
	weaponModel_->GetWorldTransform()->scale_ = { 0.5f,0.5f,0.5f };
	weaponModel_->SetMaterial({ 0.0f,0.0,1.0f,1.0f });
}

void Weapon::Update()
{
	GameObject::Update();

	weaponModel_->ModelDebug("weapon");
	//weaponModel->GetWorldTransform()->translation_.x += 0.03f;


	// アニメーション設定
	static constexpr float animationDuration = 0.05f; // アニメーション時間（秒）
	static const Vector3 startRotation = { 0.0f, 0.0f, 0.0f }; // 初期回転角度
	Vector3 endRotation = { 0.0f, -3.0f, 0.0f }; // 目標回転角度
	static float elapsedTime = 0.0f; // 経過時間

	if (isAttack_) {


		// アニメーション中
		elapsedTime += 1.0f / 60.0f; // フレーム時間（60FPS想定）

		// アニメーション完了後、時間を固定
		if (elapsedTime > animationDuration) {
			elapsedTime = animationDuration;
		}

		// 線形補間 (Lerp)
		float t = elapsedTime / animationDuration; // 進行割合（0.0f〜1.0f）
		Vector3 newRotation = Lerp(startRotation, endRotation, t);

		// 回転を設定
		weaponModel_->GetWorldTransform()->rotation_ = newRotation;
	}
	else {
		// 攻撃していない場合はリセット
		elapsedTime = 0.0f;
		weaponModel_->GetWorldTransform()->rotation_ = startRotation;
	}
}



void Weapon::Draw(const Camera& camera)
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
