/**
*	@file SceneManager.h
*	@brief シーンマネージャークラスヘッダ
*/

#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

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

	void SetSceneFactory(AbstractSceneFactory* sceneFactory);

private:
	static SceneManager* instance_;	//シングルトン
	AbstractSceneFactory* sceneFactory_ = nullptr;
	//今のシーン
	BaseScene* scene_ = nullptr;
	BaseScene* nextSscene_ = nullptr;
};

