#include "TitleEffect.h"
using namespace AobaraEngine;

void TitleEffect::Initialize(AobaraEngine::Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture_ = textureManager_->LoadTexture("Resources/SampleAssets/circle.png");

	Vector3 velocity = { 0.0f,5.0f,0.0f };
	risingEffect_.reset(ParticleSystem::Create(circleTexture_, camera, velocity, true));
	//hitEffect->emitter_->transform.translate = { 0.0f,-2.1f,0.0f };
	risingEffect_->emitter_->transform.scale = { 11.0f,2.0f,1.0f };
	risingEffect_->emitter_->count = 10;
	risingEffect_->emitter_->frequency = 0.8f;
	risingEffect_->SetLifeTime(1.0f, 3.0f);
	
	risingEffect_->SetUseBillBoard();
	//hitEffect->SetRandomAllVelocity();
	risingEffect_->SetRandomVelocityY(true);
	risingEffect_->SetRandomVelocityZ(true);
	risingEffect_->SetColor({ 1.0f,1.0f,1.0f });
	
	//hitEffect->SetRandomColor();
}

void TitleEffect::Update()
{
	risingEffect_->Debug("Effect");
	risingEffect_->Update();

	//if (flag)
	//{
	//	playTime--;


	//	if (playTime <= 0)
	//	{
	//		//実行時間を過ぎたら粒の生成を止める
	//		hitEffect->StopMakeParticle();

	//		if (hitEffect->GetIsParticleEmpty())
	//		{
	//			//エフェクトの粒が0になったら消滅フラグを立てる
	//			isDead = true;
	//		}
	//	}
	//}
}

void TitleEffect::Draw()
{
	//if (flag)
	//{
	risingEffect_->Draw();
	//}
}

bool TitleEffect::IsDead()
{
	return isDead_;
}
