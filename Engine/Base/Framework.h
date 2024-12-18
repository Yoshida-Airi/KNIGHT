/**
*	@file Framework.h
*	@brief フレームワーククラスヘッダ
*/

#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"DebugHelper.h"
#include"TextureManager.h"
#include"Input.h"
#include"Audio.h"
#include"SceneManager.h"
#include"ImGuiManager.h"
#include"AbstractSceneFactory.h"
#include"SrvManager.h"
#include"GlobalVariables.h"

/**
*   @class Framework
*	@brief  フレームワーククラス
*/
class Framework
{

public:

	~Framework();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();

protected:
	//WindowAPIの初期化
	WinApp* winApp = nullptr;
	//DirectXの初期化
	DirectXCommon* dxCommon = nullptr;
	GraphicsPipelineManager* psoManager = nullptr;
	TextureManager* texture = nullptr;
	Input* input;
	Audio* audio;
	//IScene* scene_ = nullptr;	//シーン
	SceneManager* sceneManager_ = nullptr;
	GlobalVariables* glovalVariable = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;


	ImGuiManager* imgui;

	SrvManager* srvManager = nullptr;

};

