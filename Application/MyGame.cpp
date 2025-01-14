#include "MyGame.h"
#include"SceneFactory.h"

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();


	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);

#ifdef _DEBUG
	//シーンマネージャーに最初のシーンをセット
	sceneManager_->ChangeScene("GAMEPLAY");
#endif // _DEBUG

#ifdef _NDEBUG
	//シーンマネージャーに最初のシーンをセット
	sceneManager_->ChangeScene("TITLE");
#endif // _NDEBUG

}

void MyGame::Update()
{

	/*--- ゲームループ  ---*/

	Framework ::Update();
	
}

void MyGame::Draw()
{
	Framework::Draw();
}
