#include "Collider.h"

void Collider::Initialize()
{
	model_.reset(Model::Create("Resources/SampleAssets/sphere.obj"));
}

void Collider::UpdateWorldTransform()
{
	model_->GetWorldTransform()->translation_ = GetWorldPosition();
	model_->GetWorldTransform()->scale_.x = radius_.x;
	model_->GetWorldTransform()->scale_.y = radius_.y;
	model_->GetWorldTransform()->scale_.z = radius_.z;
	model_->GetWorldTransform()->UpdateWorldMatrix();

}

void Collider::Draw(const Camera& camera)
{
	model_->Draw(camera);
}