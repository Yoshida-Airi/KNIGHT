#include "ClearScene.h"
#include"SceneManager.h"

ClearScene::~ClearScene()
{
	delete camera_;
	delete effect_;
}

void ClearScene::Initialize()
{
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	titleTexture_ = textureManager_->LoadTexture("Resources/Scene/clear.png");

	soundData_ = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera_ = new Camera;
	camera_->Initialize();

	title_.reset(Sprite::Create(titleTexture_));

}

void ClearScene::Update()
{
	camera_->CameraDebug();

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

	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
		//Audio::GetInstance()->SoundStopWave(soundData);
	}
	
	title_->Update();





}

void ClearScene::Draw()
{
	title_->Draw(camera_);


}

