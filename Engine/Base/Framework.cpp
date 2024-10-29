#include "Framework.h"

Framework::~Framework()
{

	delete texture_;
	texture_ = nullptr;

	delete srvManager_;

	delete audio_;

	delete input_;
	input_ = nullptr;


	//DirectXの解放
	delete dxCommon_;
	dxCommon_ = nullptr;

	delete psoManager_;

	//WindowAPIの解放
	delete winApp_;
	winApp_ = nullptr;


	delete imgui_;


	delete sceneManager_;


#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG

	
}

void Framework::Initialize()
{
	//WindowAPIの初期化
	winApp_ = WinApp::GetInstance();
	winApp_->Initialize();

	//DirectXの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize();

	psoManager_ = GraphicsPipelineManager::GetInstance();
	psoManager_->Initialize();

	srvManager_ = SrvManager::GetInstance();
	srvManager_->Initialize();

	texture_ = TextureManager::GetInstance();
	texture_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	audio_ = Audio::GetInstance();
	audio_->Initialize();

	imgui_ = ImGuiManager::GetInstance();
	imgui_->Initialize();

	glovalVariable_ = GlobalVariables::GetInstance();
	glovalVariable_->LoadFiles();


	sceneManager_ = SceneManager::GetInstance();
	delete sceneFactory_;
}

void Framework::Update()
{

	imgui_->Begin();

	input_->Update();
	glovalVariable_->Update();


	sceneManager_->Update();

}

void Framework::Draw()
{
	dxCommon_->PreDraw();
	srvManager_->PreDraw();
	sceneManager_->Draw();

#ifdef _DEBUG
	ImGui::ShowDemoWindow();
#endif // _DEBUG

	imgui_->End();
	imgui_->Draw();

	dxCommon_->PostDraw();
}

void Framework::Run()
{
	//初期化
	Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp_->ProcessMessage()==0)
	{
		//更新
		Update();
		//描画
		Draw();

	}
}
