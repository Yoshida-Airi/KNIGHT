#include "Camera.h"

void AobaraEngine::Camera::Initialize()
{
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	transform_ =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,6.0f,-30.0f}
	};

	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void AobaraEngine::Camera::CreateConstBuffer()
{
	constBuffer_ = dxCommon_->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void AobaraEngine::Camera::Map() {
	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constMap_));
}

void AobaraEngine::Camera::UpdateMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void AobaraEngine::Camera::TransferMatrix()
{
	constMap_->view = matView_;
	constMap_->projection = matProjection_;

	constMap_->sview = smatView_;
	constMap_->sprojection = smatProjection_;

	constMap_->worldPosition = transform_.translate;
}

void AobaraEngine::Camera::UpdateViewMatrix()
{
	cameraMatrix_ = MakeAffinMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matView_ = Inverse(cameraMatrix_);
	smatView_ = MakeIdentity4x4();
}

void AobaraEngine::Camera::UpdateProjectionMatrix()
{
	matProjection_ = MakePerspectiveFovMatrix(0.45f, float(winApp_->kCilentWidth) / float(winApp_->kCilentHeight), nearZ_, farZ_);
	smatProjection_ = MakeOrthographicmatrix(0.0f, 0.0f, float(winApp_->kCilentWidth), float(winApp_->kCilentHeight), 0.0f, 100.0f);
}

void AobaraEngine::Camera::CameraDebug()
{
#ifdef _DEBUG
	ImGui::Begin("camera");

	ImGui::DragFloat3("transform", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);

	UpdateMatrix();

	ImGui::End();

#endif // _DEBUG
	
}
