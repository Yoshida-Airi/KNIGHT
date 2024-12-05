/**
*	@file ClearScene.h
*	@brief クリアシーンクラスヘッダ
*/

#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"TextureManager.h"
#include"Input.h"
#include"Audio.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Camera.h"
#include"Effect/Fade.h"

#include"EffectManager.h"
#include"Effect/TitleEffect.h"
#include"Object/Skydome/Skydome.h"

/**
*   @class ClearScene
*	@brief  クリアシーンクラス
*/
class ClearScene :public BaseScene
{
public:
	/// @brief デストラクタ
	~ClearScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

	void UpdateSpriteBlink();

private:

	EffectManager* effect_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	Input* input_;
	SceneManager* sceneManager_ = nullptr;
	AobaraEngine::Camera* camera_;

	uint32_t titleTexture_;
	uint32_t spaceTexture_;

	uint32_t soundData_;

	bool isFadeStart_ = false;

	std::unique_ptr<AobaraEngine::Sprite> space_ = nullptr;

	std::unique_ptr<AobaraEngine::Sprite> title_ = nullptr;

	std::unique_ptr <Skydome>skydome_;
	std::unique_ptr<Fade> fade_ = nullptr;
	std::unique_ptr<TitleEffect> titleEffect_ = nullptr;

	const float kDeltaTime_ = 1.0f / 60.0f;
	int frameCount_ = 0;
	int blinkFrames_ = 50;  // 30フレームごとに点滅
};

