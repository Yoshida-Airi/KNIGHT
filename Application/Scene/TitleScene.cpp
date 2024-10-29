#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera_;
	delete effect_;
}

void TitleScene::Initialize()
{
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	titleLogo_ = textureManager_->LoadTexture("Resources/Scene/logo.png");
	backGroundTexture_= textureManager_->LoadTexture("Resources/Scene/backGround.png");
	spaceTexture_ = textureManager_->LoadTexture("Resources/Scene/space.png");

	soundData_ = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera_ = new Camera;
	camera_->Initialize();

	title_.reset(Sprite::Create(titleLogo_));
	title_->GetWorldTransform()->translation_ = { 200.0f,110.0f };

	backGround_.reset(Sprite::Create(backGroundTexture_));

	space_.reset(Sprite::Create(spaceTexture_));
	space_->GetWorldTransform()->translation_ = { 435.0f,490.0f };
	

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
	skydome_->SetLight(false);


	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.5f);

	titleEffect_ = std::make_unique <TitleEffect>();
	titleEffect_->Initialize(camera_);
	titleEffect_->SetFlag(true);
	titleEffect_->SetPosition({ 0.0f,-3.5f,0.0f });


}

void TitleScene::Update()
{
	camera_->CameraDebug();

	fade_->Update();

	UpdateSpriteBlink();

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

	if (input_->TriggerKey(DIK_SPACE) && isSceneChange_ == false)
	{
		fade_->Start(Fade::Status::FadeOut, 1.5f);
		isSceneChange_ = true;
		//Audio::GetInstance()->SoundStopWave(soundData);
	}

	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	title_->Update();
	backGround_->Update();
	space_->Update();

	skydome_->Update();
	
	

	titleEffect_->Update();

	
	
}

void TitleScene::Draw()
{
	

	
	
	skydome_->Draw(camera_);

	titleEffect_->Draw();
	//backGround->Draw(camera);
	title_->Draw(camera_);
	space_->Draw(camera_);

	fade_->Draw(camera_);

	
}

void TitleScene::UpdateSpriteBlink()
{
	frameCount_++;

	if (frameCount_ >= blinkFrames_)
	{
		space_->SetisInvisible(!space_->GetisInvisible());
		frameCount_ = 0;
	}
}