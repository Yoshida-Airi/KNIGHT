#include "CameraController.h"
#include"Object/Player/Player.h"
using namespace AobaraEngine;

CameraController::~CameraController()
{
	
}
void CameraController::Initialize(Camera* camera)
{
	globalVariables_ = GlobalVariables::GetInstance();
	groupName_ = "Camera";

	camera_ = camera;
	camera_->Initialize();


	globalVariables_->AddItem(groupName_, "targetOffset", targetOffset_);
	globalVariables_->AddItem(groupName_, "imterpolationRate", interpolationRate_);
}

void CameraController::Update()
{
	ApplyGlobalVariables();

	//追従対象のワールドトランスフォームを参照
	const WorldTransform* targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの目標座標を計算
	targetPosition_ = Add(targetWorldTransform->translation_, targetOffset_);

	//座標補間
	camera_->transform_.translate = targetPosition_;/*Lerp(camera_->transform.translate, targetPosition, interpolationRate);*/

	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	//追従対象のワールドトランスフォームを参照
	const WorldTransform* targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	camera_->transform_.translate = Add(targetWorldTransform->translation_ , targetOffset_);


}

void CameraController::ApplyGlobalVariables()
{
	targetOffset_ = globalVariables_->GetVector3Value(groupName_, "targetOffset");
	interpolationRate_ = globalVariables_->GetFloatValue(groupName_, "imterpolationRate");
	

}
