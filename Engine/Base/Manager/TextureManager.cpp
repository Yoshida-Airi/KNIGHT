#include "TextureManager.h"

uint32_t TextureManager::kSRVIndexTop_ = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new TextureManager;
	}
	return instance_;
}

TextureManager::~TextureManager()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void TextureManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	srvManager_ = SrvManager::GetInstance();

	srvDescriptoHeap_ = srvManager_->GetDescriptorHeap();
	descriptorSizeSRV_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	textureDatas_.reserve(SrvManager::kMaxSRVCount);

}

//更新処理
void TextureManager::Update()
{

}


uint32_t TextureManager::LoadTexture(const std::string& filePath)
{
	uint32_t index = 0;


	////読み込み済みテクスチャを検索
	//auto it = std::find_if(
	//	textureDatas.begin(),
	//	textureDatas.end(),
	//	[&](TextureData& textureData) {return textureData.filename == filePath; }
	//);
	//if (it != textureDatas.end())
	//{
	//	//読み込み済みなら戻す
	//	uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
	//	return textureIndex;
	//}

	//読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath))
	{
		uint32_t textureIndex = textureDatas_[filePath].textureHandle;
		return textureIndex;
	}


	for (int i = 0; i < kMaxTexture_; i++)
	{
		if (isUsedTexture_[i] == false) {
			index = i;
			isUsedTexture_[i] = true;
			break;
		}
	}

	//テクスチャ枚入数上限チェック
	assert(srvManager_->SrvMaxCountCheck());

	//テクスチャ枚数上限
	assert(textureDatas_.size() + kSRVIndexTop_ < SrvManager::kMaxSRVCount);

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = ImageFileOpen(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	
	//テクスチャデータを追加して書き込む
	//textureDatas.resize(textureDatas.size() + 1);
	//TextureData& textureData = textureDatas.back();

	TextureData& textureData = textureDatas_[filePath];


	textureData.filename = filePath;
	textureData.textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);


	textureData.textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource_.at(index) = UploadTextureData(textureData.textureResource.Get(), mipImages);


	//テクスチャデータの要素数番号をSRVのインデックスとする
	//uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	//textureData.textureSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptoHeap_, descriptorSizeSRV, srvIndex);
	//textureData.textureSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptoHeap_, descriptorSizeSRV, srvIndex);

	textureData.srvIndex = srvManager_->Allocate();
	textureData.textureSrvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.textureSrvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	textureData.textureHandle = index;

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex,textureData.textureResource.Get(), metadata.format, UINT(metadata.mipLevels));

	return index;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvGPUHandle(uint32_t textureIndex)
{
	//範囲外指定違反の場合止める
	assert(textureIndex < SrvManager::kMaxSRVCount);

	std::string filePath = SearchFilepath(textureIndex);

	TextureData& textureData = textureDatas_[filePath];
	return textureData.textureSrvHandleGPU;
}



const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle)
{

	//範囲外指定違反の場合止める
	assert(textureHandle < SrvManager::kMaxSRVCount);

	std::string filePath = SearchFilepath(textureHandle);

	TextureData& textureData = textureDatas_[filePath];

	//テクスチャの情報を取得
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc = textureData.textureResource.Get()->GetDesc();

	return resourceDesc;
}

DirectX::ScratchImage TextureManager::ImageFileOpen(const std::string& filePath)
{
	//テクスチャファイルを読み込みプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));
	//ミップマップの作成
	DirectX::ScratchImage mipImage{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);

	return mipImage;
}

//ダイレクト12のテクスチャリソースを作る
Microsoft::WRL::ComPtr< ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//1.metadataを基にリソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);		//Textureの幅
	resourceDesc.Height = UINT(metadata.height);	//Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);	//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);	//奥行きor配列Textureの配列数
	resourceDesc.Format = metadata.format;	//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;	//サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	//Textureの次元数。普段使っているのは二次元


	//2.利用するヒープの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//細かい設定を行う
	

	//3.リソースを生成する
	Microsoft::WRL::ComPtr< ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,	//Heapの設定
		D3D12_HEAP_FLAG_NONE,	//Heapの特殊な設定。
		&resourceDesc,	//resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,	//初回のResourceState。Textureは基本読むだけ
		nullptr,	//Clear最適化。使わないのでNULL
		IID_PPV_ARGS(&resource));	//作成するResourceポインタへのポインタ

	assert(SUCCEEDED(hr));

	return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr< ID3D12Resource> intermediateResource = dxCommon_->CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon_->GetCommandList(), texture, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

std::string TextureManager::SearchFilepath(uint32_t index)
{

	for (const auto& pair : textureDatas_) {
		if (pair.second.textureHandle == index) {
			return pair.second.filename;
		}
	}
	assert(0);
	return 0;
}

TextureManager* TextureManager::instance_ = NULL;