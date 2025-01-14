#include "WorldTransform.h"

WorldTransform::~WorldTransform()
{
}

void WorldTransform::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	matWorld_ = MakeIdentity4x4();
	CreateConstBuffer();
	Map();
	TransferMatrix();

}


void WorldTransform::CreateConstBuffer()
{
	if (dxCommon_) {
		constBuffer_ = dxCommon_->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));
	}
	else {
		assert(dxCommon_);
	}
}

void WorldTransform::Map()
{
	constBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap_));
}

void WorldTransform::TransferMatrix()
{
	constMap_->matWorld = matWorld_;
}

void WorldTransform::UpdateWorldMatrix()
{
	Matrix4x4 AffineMatrix = MakeAffinMatrix(scale_, rotation_, translation_);
	matWorld_ = AffineMatrix;
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}