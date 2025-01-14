#include "Triangle.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

AobaraEngine::Triangle::~Triangle()
{
	//delete worldTransform_;
}

void AobaraEngine::Triangle::Initialize(const uint32_t& textureHandle)
{

	dxCommon_ = DirectXCommon::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	texture_ = TextureManager::GetInstance();
	worldTransform_ = std::make_unique< WorldTransform>();
	worldTransform_->Initialize();
	textureHandle_ = textureHandle;

	VertexBuffer();
	MaterialBuffer();
	LightBuffer();

	TriangleData initData;
	initData.vertex[0] = { -0.5f,-0.5f,0.0f,1.0f };
	initData.vertex[1] = { 0.0f,0.5f,0.0f,1.0f };
	initData.vertex[2] = { 0.5f,-0.5f,0.0f,1.0f };

	initData.color = { 1.0f,1.0f,1.0f,1.0f };

	//頂点の設定
	vertexData_[0].position = initData.vertex[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = initData.vertex[1];
	vertexData_[1].texcoord = { 0.5f,0.0f };

	vertexData_[2].position = initData.vertex[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

	SetMaterialData(initData.color);

	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 70.0f;

	//ライトのデフォルト値
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;

}

void AobaraEngine::Triangle::Update()
{
	worldTransform_->UpdateWorldMatrix();

}

void AobaraEngine::Triangle::Draw(const Camera& camera)
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
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetSrvGPUHandle(textureHandle_));
	//ライト用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}


void AobaraEngine::Triangle::SetMaterialData(const Vector4& color)
{
	materialData_[0].color = color;
}

AobaraEngine::Triangle* AobaraEngine::Triangle::Create(const uint32_t& textureHandle)
{
	Triangle* triangle = new Triangle();
	triangle->Initialize(textureHandle);
	return triangle;
}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

void AobaraEngine::Triangle::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 3);	//頂点用のデータ

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void AobaraEngine::Triangle::MaterialBuffer()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}

void AobaraEngine::Triangle::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}
