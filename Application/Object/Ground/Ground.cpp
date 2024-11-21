#include "Ground.h"

void Ground::Initialize()
{
	groundModel_.reset(Model::Create("Resources/Level/Map.obj"));

	groundModel_->GetWorldTransform()->translation_.x = -10.0f;
	groundModel_->GetWorldTransform()->translation_.y = 1.0f;

	groundModel_->GetWorldTransform()->scale_ = { 2.0f,1.0f,1.0f };

	SetRadius({ groundModel_->GetWorldTransform()->scale_.x / 2.0f,groundModel_->GetWorldTransform()->scale_.y / 2.0f,groundModel_->GetWorldTransform()->scale_.z / 2.0f });

	//groundModel->SetisInvisible(true);
}

void Ground::Update()
{
	groundModel_->Update();
}

void Ground::Draw(const Camera& camera)
{
	groundModel_->Draw(camera);
}

Vector3 Ground::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = groundModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = groundModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = groundModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Ground::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = 
	{ 
		worldPos.x - groundModel_->GetWorldTransform()->scale_.x   ,
		worldPos.y - groundModel_->GetWorldTransform()->scale_.y  ,
		worldPos.z - groundModel_->GetWorldTransform()->scale_.z
	};

	aabb.max = 
	{ 
		worldPos.x + groundModel_->GetWorldTransform()->scale_.x  ,
		worldPos.y + groundModel_->GetWorldTransform()->scale_.y  ,
		worldPos.z + groundModel_->GetWorldTransform()->scale_.z
	};

	return aabb;
}

[[maybe_unused]]
void Ground::OnCollision([[maybe_unused]] Collider* other)
{
}

void Ground::Debug(const char* name)
{
	groundModel_->ModelDebug(name);
}


