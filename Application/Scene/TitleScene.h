/**
*	@file TitleScene.h
*	@brief タイトルシーンクラスヘッダ
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
#include"Effect/TitleEffect.h"
#include"ParticleSystem.h"
#include"Object/Skydome/Skydome.h"
#include"EffectManager.h"

/**
*   @class TitleScene
*	@brief タイトルシーンクラス
*/
class TitleScene :public BaseScene
{
public:
	/// @brief デストラクタ
	~TitleScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

	/// @brief スプライトの点滅の更新
	void UpdateSpriteBlink();

private:

	std::unique_ptr < EffectManager> effect_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	Input* input_;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr < Camera> camera_;

	uint32_t titleLogo_;
	uint32_t backGroundTexture_;
	uint32_t spaceTexture_;
	uint32_t soundData_;
	
	std::unique_ptr<AobaraEngine::Sprite> title_ = nullptr;
	std::unique_ptr<AobaraEngine::Sprite> backGround_ = nullptr;
	std::unique_ptr<AobaraEngine::Sprite> space_ = nullptr;

	std::unique_ptr <Skydome>skydome_;

	std::unique_ptr<Fade> fade_ = nullptr;
	std::unique_ptr<TitleEffect> titleEffect_ = nullptr;
	
	bool isSceneChange_ = false;

	const float kDeltaTime_ = 1.0f / 60.0f;
	int frameCount_ = 0;
	int blinkFrames_ = 50;  // 30フレームごとに点滅
};

