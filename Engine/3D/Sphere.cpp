#include "Sphere.h"
using namespace AobaraEngine;

Sphere::~Sphere()
{
	delete worldTransform_;
}

void Sphere::Initialize(uint32_t textureHandle)
{
	dxCommon_ = DirectXCommon::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	texture_ = TextureManager::GetInstance();

	VertexBuffer();
	MaterialBuffer();
	LightBuffer();

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	textureHandle_ = textureHandle;

	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision_; ++latIndex)
	{
		float lat = static_cast<float>(-std::numbers::pi) / 2.0f + kLatEvery_ * latIndex;

		//経度の方向に分割しながら線を書く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex)
		{
			vertexData_[0].normal = { 0.0f,0.0f,-1.0f };

			//最初の頂点位置
			uint32_t start = (latIndex * kSubdivision_ + lonIndex) * 6;

			float lon = lonIndex * kLonEvery_;

			//頂点にデータを入力する	頂点a
			vertexData_[start].position.x = std::cos(lat) * std::cos(lon);
			vertexData_[start].position.y = std::sin(lat);
			vertexData_[start].position.z = std::cos(lat) * std::sin(lon);
			vertexData_[start].position.w = 1.0f;

			vertexData_[start].texcoord.x = float(lonIndex) / float(kSubdivision_);
			vertexData_[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision_);

			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;


			//頂点b
			vertexData_[start + 1].position.x = std::cos(lat + kLatEvery_) * std::cos(lon);
			vertexData_[start + 1].position.y = std::sin(lat + kLatEvery_);
			vertexData_[start + 1].position.z = std::cos(lat + kLatEvery_) * std::sin(lon);
			vertexData_[start + 1].position.w = 1.0f;

			vertexData_[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision_);
			vertexData_[start + 1].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision_);

			vertexData_[start + 1].normal.x = vertexData_[start + 1].position.x;
			vertexData_[start + 1].normal.y = vertexData_[start + 1].position.y;
			vertexData_[start + 1].normal.z = vertexData_[start + 1].position.z;

			//頂点c
			vertexData_[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery_);
			vertexData_[start + 2].position.y = std::sin(lat);
			vertexData_[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery_);
			vertexData_[start + 2].position.w = 1.0f;

			vertexData_[start + 2].texcoord.x = float(lonIndex + 1) / float(kSubdivision_);
			vertexData_[start + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision_);

			vertexData_[start + 2].normal.x = vertexData_[start + 2].position.x;
			vertexData_[start + 2].normal.y = vertexData_[start + 2].position.y;
			vertexData_[start + 2].normal.z = vertexData_[start + 2].position.z;

			//頂点d
			vertexData_[start + 3].position.x = std::cos(lat) * std::cos(lon + kLonEvery_);
			vertexData_[start + 3].position.y = std::sin(lat);
			vertexData_[start + 3].position.z = std::cos(lat) * std::sin(lon + kLonEvery_);
			vertexData_[start + 3].position.w = 1.0f;

			vertexData_[start + 3].texcoord.x = float(lonIndex + 1) / float(kSubdivision_);
			vertexData_[start + 3].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision_);

			vertexData_[start + 3].normal.x = vertexData_[start + 3].position.x;
			vertexData_[start + 3].normal.y = vertexData_[start + 3].position.y;
			vertexData_[start + 3].normal.z = vertexData_[start + 3].position.z;

			//頂点e
			vertexData_[start + 4].position.x = std::cos(lat + kLatEvery_) * std::cos(lon);
			vertexData_[start + 4].position.y = std::sin(lat + kLatEvery_);
			vertexData_[start + 4].position.z = std::cos(lat + kLatEvery_) * std::sin(lon);
			vertexData_[start + 4].position.w = 1.0f;

			vertexData_[start + 4].texcoord.x = float(lonIndex) / float(kSubdivision_);
			vertexData_[start + 4].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision_);

			vertexData_[start + 4].normal.x = vertexData_[start + 4].position.x;
			vertexData_[start + 4].normal.y = vertexData_[start + 4].position.y;
			vertexData_[start + 4].normal.z = vertexData_[start + 4].position.z;

			//頂点f
			vertexData_[start + 5].position.x = std::cos(lat + kLatEvery_) * std::cos(lon + kLonEvery_);
			vertexData_[start + 5].position.y = std::sin(lat + kLatEvery_);
			vertexData_[start + 5].position.z = std::cos(lat + kLatEvery_) * std::sin(lon + kLonEvery_);
			vertexData_[start + 5].position.w = 1.0f;

			vertexData_[start + 5].texcoord.x = float(lonIndex + 1) / float(kSubdivision_);
			vertexData_[start + 5].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision_);

			vertexData_[start + 5].normal.x = vertexData_[start + 5].position.x;
			vertexData_[start + 5].normal.y = vertexData_[start + 5].position.y;
			vertexData_[start + 5].normal.z = vertexData_[start + 5].position.z;



		}
	}

	
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 70.0f;

	//Lightingを有効にする
	materialData_->enableLighting = true;

	//ライトのデフォルト値

	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;
}

void Sphere::Update()
{
	worldTransform_->UpdateWorldMatrix();

#ifdef _DEBUG

	ImGui::Begin("light");

	float direction[] = { lightData_->direction.x,lightData_->direction.y,lightData_->direction.z };
	ImGui::DragFloat3("lightDirection", direction, 0.1f);

	lightData_->direction.x = direction[0];
	lightData_->direction.y = direction[1];
	lightData_->direction.z = direction[2];

	float color[] = { materialData_->color.x,materialData_->color.y,materialData_->color.z,materialData_->color.w };
	ImGui::ColorEdit4("Pick A Color", color);

	materialData_->color.x = color[0];
	materialData_->color.y = color[1];
	materialData_->color.z = color[2];
	materialData_->color.w = color[3];

	ImGui::End();
#endif // _DEBUG



}

void Sphere::Draw(Camera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().object3D.rootSignature.Get());
	dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().object3D.graphicPipelineState.Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_->constBuffer_->GetGPUVirtualAddress());
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetSrvGPUHandle(textureHandle_));
	//ライト用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(totalVertex_, 1, 0, 0);
}

Sphere* Sphere::Create(uint32_t textureHandle)
{
	Sphere* sphere = new Sphere();
	sphere->Initialize(textureHandle);
	return sphere;
}


/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

/// <summary>
/// 頂点のバッファの取得
/// </summary>
void Sphere::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * totalVertex_);	//頂点用のデータ
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * totalVertex_;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

}

/// <summary>
/// マテリアルのバッファの取得
/// </summary>
void Sphere::MaterialBuffer()
{

	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}

void Sphere::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}
