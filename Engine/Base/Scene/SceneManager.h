/**
*	@file SceneManager.h
*	@brief シーンマネージャークラスヘッダ
*/

#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"
#include<memory>

/**
*   @class SceneManager
*	@brief  シーン管理クラス
*/
class SceneManager
{
public:

	//シングルトン
	static SceneManager* GetInstance();
	~SceneManager();
	void Update();
	void Draw();

	/// <summary>
	/// 次シーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AobaraEngine::AbstractSceneFactory* sceneFactory);

private:
	static SceneManager* instance_;	//シングルトン
	AobaraEngine::AbstractSceneFactory* sceneFactory_ = nullptr;
	//今のシーン
	std::unique_ptr<AobaraEngine::BaseScene> scene_ = nullptr;
	std::unique_ptr<AobaraEngine::BaseScene> nextSscene_ = nullptr;
};

