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
	spaceTexture_ = textureManager_->LoadTexture("Resources/Scene/space.png");


	soundData_ = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera_ = new Camera;
	camera_->Initialize();

	title_.reset(Sprite::Create(titleTexture_));
	title_->GetWorldTransform()->translation_ = { 242.0f,146.0f };

	space_.reset(Sprite::Create(spaceTexture_));
	space_->GetWorldTransform()->translation_ = { 435.0f,490.0f };

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	skydome_->SetLight(false);

	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 2.5f);

	titleEffect_ = std::make_unique <TitleEffect>();
	titleEffect_->Initialize(camera_);
	titleEffect_->SetFlag(true);
	titleEffect_->SetPosition({ 0.0f,-3.5f,0.0f });

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
	space_->Update();
	fade_->Update();
	titleEffect_->Update();

	skydome_->Update();

}

void ClearScene::Draw()
{
	skydome_->Draw(camera_);

	titleEffect_->Draw();
	title_->Draw(camera_);
	space_->Draw(camera_);
	fade_->Draw(camera_);
	

}

