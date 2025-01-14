#include "Skydome.h"

using namespace AobaraEngine;

void Skydome::Initialize()
{
	skydomeModel_.reset(Model::Create("Resources/Object/Skydome/skydome.obj"));
	

	// 天球の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)
	skydomeModel_->GetWorldTransform()->scale_.x = 1000.0f;
	skydomeModel_->GetWorldTransform()->scale_.y = 1000.0f;
	skydomeModel_->GetWorldTransform()->scale_.z = 1000.0f;

	
}

void Skydome::Update()
{
	skydomeModel_->Update();
}

void Skydome::Draw(const Camera& camera)
{
	skydomeModel_->Draw(camera);
}