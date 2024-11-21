#include "EffectManager.h"

void EffectManager::Initialize()
{

	initialized_ = false;  // 初期化が行われていない状態に設定
}

void EffectManager::Update(float deltaTime)
{
    if (!initialized_)
    {
        // アニメーションの初期化はここで行う
        deltaTime_ = deltaTime;

        texture = TextureManager::GetInstance();  // TextureManager のインスタンスを取得するメソッドが存在すると仮定
        blackTexture_ = texture->LoadTexture("Resources/uvChecker.png");
        fadeIn_.reset(AobaraEngine::Sprite::Create(blackTexture_));
        fadeIn_->SetSize({ 1920, 720 });

        initialized_ = true;  // 初期化が完了したことを示すフラグを設定
    }

    // アニメーションの更新はここで行う
    deltaTime_ -= 0.01f;
    fadeIn_->Update();
    fadeIn_->SetMaterialData({ 1.0f, 1.0f, 1.0f, deltaTime_ });
}

void EffectManager::Draw(Camera* camera)
{
    fadeIn_->Draw(*camera);
}

