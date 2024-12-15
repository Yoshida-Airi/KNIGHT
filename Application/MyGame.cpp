#include "MyGame.h"
#include"SceneFactory.h"

MyGame::~MyGame()
{
	delete scene;

}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();


	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);

	//シーンマネージャーに最初のシーンをセット
	sceneManager_->ChangeScene("TITLE");
	
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
