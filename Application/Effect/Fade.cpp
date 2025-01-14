#include "Fade.h"

using namespace AobaraEngine;

void Fade::Initialize()
{
	fadeTex_ = TextureManager::GetInstance()->LoadTexture("Resources/SampleAssets/black.png");

	sprite_ = std::make_unique <AobaraEngine::Sprite>();
	sprite_->Initialize(fadeTex_);
	sprite_->SetSize({ 1280.0f,720.0f });
	sprite_->SetMaterialData({ 0.0f,0.0f,0.0f,1.0f });
}

void Fade::Update()
{
	sprite_->Update();

	switch (status_)
	{
	case Fade::Status::None:
		//何もしない
		break;
	case Fade::Status::FadeIn:
		//フェードイン中の更新処理
		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		//フェード継続時間に達したら打ち止め
		if (counter_ >= duration_)
		{
			counter_ = duration_;
		}

		//0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を小さくする
		sprite_->SetMaterialData({ 0.0f, 0.0f, 0.0f, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f) });

		break;
	case Fade::Status::FadeOut:
		//フェードアウト中の更新処理

		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		//フェード継続時間に達したら打ち止め
		if (counter_ >= duration_)
		{
			counter_ = duration_;
		}

		//0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
		sprite_->SetMaterialData({ 0.0f,0.0f,0.0f,std::clamp(counter_ / duration_,0.0f,1.0f) });

		break;
	}

}

void Fade::Draw(const Camera& camera)
{
	if (status_ == Status::None)
	{
		return;
	}

	sprite_->Draw(camera);
}

void Fade::Start(Status status, float duration)
{
	
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop()
{
	status_ = Status::None;
}

bool Fade::IsFinished() const
{
	switch (status_)
	{
	case Fade::Status::FadeIn:
		return false;
	case Fade::Status::FadeOut:
		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}
