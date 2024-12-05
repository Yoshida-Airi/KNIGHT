#include "SceneFactory.h"
#include"GamePlayScene.h"
#include"TitleScene.h"
#include"TestPlayScene.h"
#include"ClearScene.h"
#include"GameOverScene.h"
#include"BossScene.h"

using namespace AobaraEngine;

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlayScene();
	}
	else if (sceneName == "TEST")
	{
		newScene = new TestPlayScene();
	}
	else if (sceneName == "CLEAR")
	{
		newScene = new ClearScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GameOverScene();
	}
	else if (sceneName == "Boss")
	{
		newScene = new BossScene();
	}
	

	return newScene;
}


