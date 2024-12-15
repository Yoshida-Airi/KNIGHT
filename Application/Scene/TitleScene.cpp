#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete efect;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	//soundData = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	ddsTexture = TextureManager::GetInstance()->LoadTexture("Resources/SampleAssets/cube.dds");

	camera = new Camera;
	camera->Initialize();



	//fence_.reset(Model::Create("Resources/SampleAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/SampleAssets/cube.obj"));

}

void TitleScene::Update()
{
	camera->CameraDebug();

	cube_->Update();

	cube_->ModelDebug("cube");



}

void TitleScene::Draw()
{

	cube_->Draw(camera);

}

