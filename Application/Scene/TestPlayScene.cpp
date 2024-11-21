#include "TestPlayScene.h"

#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera_;
	delete levelEditor_;

	for (std::vector<Model*>& blockLine : blocks_)
	{
		for (Model* block : blockLine)
		{
			delete block;
		}
	}

	blocks_.clear();

	delete mapChipField_;

}

void TestPlayScene::Initialize()
{
	camera_ = new Camera;
	camera_->Initialize();

	walkAnimation_ = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/walk.gltf");

	walk_.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	walk_->SetisInvisible(true);
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	model2_->GetWorldTransform()->translation_.x = 5.0f;
	model2_->SetisInvisible(true);

	model3_.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	//model3->SetAnimation(walkAnimation);
	model3_->SetisInvisible(true);

	model4_.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4_->GetWorldTransform()->rotation_.y = 3.14f;
	model4_->SetisInvisible(true);



	levelEditor_ = new LevelEditor();
	levelEditor_->LoaderJsonFile("Resources/Level/levelEditor.json");



	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/CSV/field.csv");


	GenerateBlocks();
}

void TestPlayScene::Update()
{
	camera_->CameraDebug();
	walk_->Update();
	model2_->Update();
	model3_->Update();
	model4_->Update();

	

	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		walk_->MoveAnimation(false);
	}

	walk_->ModelDebug("model");
	model2_->ModelDebug("cube");
	model3_->ModelDebug("walk");
	model4_->ModelDebug("simpleSkin");

	levelEditor_->Update();

	for (std::vector<Model*>& blockLine : blocks_)
	{
		for (Model* block : blockLine)
		{
			if (!block)
			{
				continue;
			}

			block->Update();
		}
	}
}

void TestPlayScene::Draw()
{
	levelEditor_->Draw(*camera_);

	walk_->Draw(*camera_);
	model2_->Draw(*camera_);
	model3_->Draw(*camera_);
	model4_->Draw(*camera_);

	for (std::vector<Model*>& blockLine : blocks_)
	{
		for (Model* block : blockLine)
		{
			if (!block)
			{
				continue;
			}

			block->Draw(*camera_);
		}
	}

}

void TestPlayScene::GenerateBlocks()
{
	//要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//要素数を変更
	blocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i)
	{
		blocks_[i].resize(numBlockHorizontal);
	}

	//ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i)
	{
		for (uint32_t j = 0; j < numBlockHorizontal; ++j)
		{
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock)
			{
				Model* model = new Model();
				model->Initialize("Resources/SampleAssets/cube.obj");
				blocks_[i][j] = model;
				blocks_[i][j]->GetWorldTransform()->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

			}
		}
	}

}

