/**
*	@file EffectManager.h
*	@brief エフェクトマネージャークラスヘッダ
*/

#pragma once
#include<wrl.h>
#include"Sprite.h"

struct FadeConstants
{
	float fadeInProgress;
};

/**
*   @class EffectManager
*	@brief  エフェクト管理クラス
*/
class EffectManager
{
	
public:
	void Initialize();
	void Update(float deltaTime);
	void Draw(Camera* camera);

	
private:
	TextureManager* texture;
	Camera* camera_;

	uint32_t blackTexture_;

	std::unique_ptr<AobaraEngine::Sprite> fadeIn_ = nullptr;

	float deltaTime_ = 0.0f;

	bool initialized_ = false;


};

