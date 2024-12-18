#include "GameOverScene.h"
#include"SceneManager.h"

GameOverScene::~GameOverScene()
{
	delete camera;
	delete efect;
}

void GameOverScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	textureManager = TextureManager::GetInstance();

	titleTexture = textureManager->LoadTexture("Resources/Scene/gameOver.png");

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();

	title.reset(Sprite::Create(titleTexture));


	fence_.reset(Model::Create("Resources/SampleAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	fence_->GetWorldTransform()->rotation_.y = 3.1f;
}

void GameOverScene::Update()
{
	camera->CameraDebug();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
			//Audio::GetInstance()->SoundStopWave(soundData);
		}
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
		//Audio::GetInstance()->SoundStopWave(soundData);
	}

	title->Update();

	fence_->Update();
	cube_->Update();



	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());


}

void GameOverScene::Draw()
{
	title->Draw(camera);

	fence_->Draw(camera);
	cube_->Draw(camera);

}

