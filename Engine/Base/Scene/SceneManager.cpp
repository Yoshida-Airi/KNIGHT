#include "SceneManager.h"
#include<cassert>

using namespace AobaraEngine;

SceneManager* SceneManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager;
	}
	return instance_;
}

SceneManager::~SceneManager()
{
	//最後のシーンの解放
	//delete scene_;
}

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (nextSscene_)
	{
		//旧シーンの終了
		if (scene_)
		{
			scene_.reset();
		}

		//シーン切り替え
		scene_.reset(nextSscene_);
		nextSscene_ = nullptr;
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextSscene_ == nullptr);

	//次シーンを生成
	nextSscene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory)
{
	sceneFactory_ = sceneFactory;
}


//静的メンバ変数の宣言と初期化
SceneManager* SceneManager::instance_ = nullptr;