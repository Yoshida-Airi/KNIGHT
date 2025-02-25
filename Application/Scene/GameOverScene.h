/**
*	@file GameOverScene.h
*	@brief ゲームオーバーシーンクラスヘッダ
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

#include"EffectManager.h"

/**
*   @class GameOverScene
*	@brief  ゲームオーバーシーンクラス
*/
class GameOverScene :public AobaraEngine::BaseScene
{
public:
	/// @brief デストラクタ
	~GameOverScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

private:

	std::unique_ptr<EffectManager> effect_ = nullptr;
	AobaraEngine::TextureManager* textureManager_ = nullptr;

	AobaraEngine::Input* input_;
	SceneManager* sceneManager_ = nullptr;
	std::unique_ptr<AobaraEngine::Camera> camera_;

	uint32_t titleTexture_;

	uint32_t soundData_;

	std::unique_ptr<AobaraEngine::Sprite> title_ = nullptr;




};

