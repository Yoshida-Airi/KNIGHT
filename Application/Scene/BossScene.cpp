#include "BossScene.h"
#include"SceneManager.h"


BossScene::~BossScene()
{

	//delete cameraController_;

	//for (Enemy* enemy : enemys_)
	//{
	//	delete enemy;
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_)
	//{
	//	delete flyEnemy;
	//}

	//for (Ground* ground : grounds_)
	//{
	//	delete ground;
	//}

	//for (DeathEffect* deathEffects : deathEffect_) {
	//	delete deathEffects;
	//}

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

void BossScene::Initialize()
{
	texture_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	//当たり判定処理の設定
	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();


	//テクスチャの読み込み
	configTexture_ = texture_->LoadTexture("Resources/Scene/config.png");
	HPTexture_ = texture_->LoadTexture("Resources/Object/Heart.png");

	//camera_ = new Camera;
	camera_->Initialize();
	camera_->transform_.translate={16.0f,66.3f,-42.0f};


	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();


	//SpawnBlock({ 47.8f, -1.0f, 0 }, { 50.31f, 1.0f, 1.0f });
	//SpawnBlock({ -2.15f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });
	//SpawnBlock({ 20.11f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });

	player_ = std::make_unique<Player>();
	player_->SetWeapon(weapon_.get());
	//player_->SetGround(grounds_);
	player_->Initialize();

	boss_ = std::make_unique<Boss>();
	boss_->Initialize();
	boss_->SetPlayer(player_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize();

	//cameraController = new CameraController;
	//cameraController->Initialize(camera);
	//cameraController->SetTarget(player.get());
	//cameraController->Reset();

	config_.reset(Sprite::Create(configTexture_));

	hp1_.reset(Sprite::Create(HPTexture_));
	hp2_.reset(Sprite::Create(HPTexture_));
	hp3_.reset(Sprite::Create(HPTexture_));
	hp4_.reset(Sprite::Create(HPTexture_));
	hp5_.reset(Sprite::Create(HPTexture_));

	hp1_->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp2_->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp3_->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp4_->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp5_->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };

	hp1_->GetWorldTransform()->translation_ = { 20.0f,20.0f,0.0f };
	hp2_->GetWorldTransform()->translation_ = { 40.0f,20.0f,0.0f };
	hp3_->GetWorldTransform()->translation_ = { 60.0f,20.0f,0.0f };
	hp4_->GetWorldTransform()->translation_ = { 80.0f,20.0f,0.0f };
	hp5_->GetWorldTransform()->translation_ = { 100.0f,20.0f,0.0f };

	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.5f);



	phase_ = Phase::kPlay;

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/CSV/bossField.csv");

	GenerateBlocks();

	//player_->SetMapChipField(mapChipField_);
}

void BossScene::Update()
{
	switch (phase_)
	{
	case BossScene::Phase::kPlay:
		//ゲームプレイフェーズの処理
		GamePlayPhase();
		break;
	case BossScene::Phase::kClear:
		//自キャラ死亡時の処理
		GameClearPhase();
		break;
	case BossScene::Phase::kDeath:
		//自キャラ死亡時の処理
		GameOverPhase();
		break;
	}

}

void BossScene::Draw()
{


	/*for (Ground* ground : grounds_)
	{
		ground->Draw(camera_.get());
	}*/


	skydome_->Draw(camera_.get());
	player_->Draw(camera_.get());
	weapon_->Draw(camera_.get());
	for (const auto& enemy : enemys_)
	{
		enemy->Draw(camera_.get());
	}

	for (const auto& flyEnemy : flyEnemys_)
	{
		flyEnemy->Draw(camera_.get());
	}


	for (const auto& deathEffects : deathEffect_)
	{
		deathEffects->Draw();
	}

	// ブロックの描画処理
	for (std::vector<Model*>& blockLine : blocks_)
	{
		for (Model* block : blockLine)
		{
			if (!block)
			{
				continue;
			}

			block->Draw(camera_.get());
		}
	}

	boss_->Draw(camera_.get());

	goal_->Draw(camera_.get());

	//colliderManager_->Draw(camera);

	config_->Draw(camera_.get());


	if (player_->GetHP() == 5)
	{

		hp1_->Draw(camera_.get());
		hp2_->Draw(camera_.get());
		hp3_->Draw(camera_.get());
		hp4_->Draw(camera_.get());
		hp5_->Draw(camera_.get());
	}
	if (player_->GetHP() == 4)
	{

		hp1_->Draw(camera_.get());
		hp2_->Draw(camera_.get());
		hp3_->Draw(camera_.get());
		hp4_->Draw(camera_.get());
	}
	if (player_->GetHP() == 3)
	{

		hp1_->Draw(camera_.get());
		hp2_->Draw(camera_.get());
		hp3_->Draw(camera_.get());
	}
	if (player_->GetHP() == 2)
	{

		hp1_->Draw(camera_.get());
		hp2_->Draw(camera_.get());
	}
	if (player_->GetHP() == 1)
	{

		hp1_->Draw(camera_.get());
	}

	fade_->Draw(camera_.get());
}

void BossScene::CheckAllCollisions()
{


	//コライダーのリストをクリア
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player_.get());

	if (weapon_->GetIsAttack() == true)
	{
		//攻撃中のみ
		colliderManager_->AddColliders(weapon_.get());
	}
	colliderManager_->AddColliders(boss_.get());

	colliderManager_->AddColliders(goal_.get());

	//colliderManager_->AddColliders(levelEditor);
	//当たり判定
	colliderManager_->ChackAllCollisions();


}


void BossScene::SpawnBlock(const Vector3& position, const Vector3& scale)
{
	// 敵を発生させる
	Ground* ground = new Ground();
	// 敵の初期化
	ground->Initialize();
	ground->SetPosition(position);
	ground->SetScale(scale);

	// リストに登録
	//grounds_.push_back(ground);
}

void BossScene::CreateDeathEffect(Vector3 position)
{
	std::unique_ptr < DeathEffect> newDeathEffect = std::make_unique<DeathEffect>();
	newDeathEffect->Initialize(camera_.get());
	newDeathEffect->SetFlag(true);

	newDeathEffect->SetPosition(position);

	deathEffect_.push_back(std::move(newDeathEffect));
}

void BossScene::ChangePhase(Phase phase)
{
	phase_ = phase;
}

void BossScene::GamePlayPhase()
{
	input_->TriggerKey(DIK_0);

	if (player_->GetWorldTransform()->translation_.x >= 18.0f)
	{
		player_->GetWorldTransform()->translation_.x= 18.0f;
	}

	camera_->UpdateMatrix();

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	colliderManager_->UpdateWorldTransform();


	if (boss_->GetIsAlive() == false)
	{
		ChangePhase(Phase::kClear);
		fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	if (player_->GetHP() == 0)
	{
		ChangePhase(Phase::kDeath);
		CreateDeathEffect({ player_->GetWorldPosition() });
		fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	fade_->Update();

	if (fade_->IsFinished())
	{
		fade_->Stop();
	}

	config_->Update();
	hp1_->Update();
	hp2_->Update();
	hp3_->Update();
	hp4_->Update();
	hp5_->Update();

	boss_->Update();

	if (boss_->GetIsAlive() == false)
	{
		CreateDeathEffect({ boss_->GetWorldPosition() });
	}


	//deathEffect_.remove_if([](DeathEffect* hitEffects) {
	//	if (hitEffects->IsDead())
	//	{
	//		//実行時間をすぎたらメモリ削除
	//		delete hitEffects;
	//		return true;
	//	}
	//	return false;
	//	});

	//enemys_.remove_if([](Enemy* enemys) {
	//	if (enemys->GetIsAlive() == false) {
	//		delete enemys;
	//		return true;
	//	}
	//	return false;
	//	});

	//flyEnemys_.remove_if([](FlyEnemy* flyEnemys) {
	//	if (flyEnemys->GetIsAlive() == false) {
	//		delete flyEnemys;
	//		return true;
	//	}
	//	return false;
	//	});


	for (const auto& deathEffects : deathEffect_) {
		deathEffects->Update();
	}


	//int i = 0;
	//for (Ground* ground : grounds_)
	//{
	//	i++;
	//	ground->Update();
	//	ground->Debug("ground" + i);
	//}


	player_->Update();
	//武器の更新
	weapon_->Update();

	for (const auto& enemy : enemys_)
	{
		enemy->Update();
	}

	for (const auto&  flyEnemy : flyEnemys_)
	{
		flyEnemy->Update();
	}

	skydome_->Update();
	goal_->Update();

	CheckAllCollisions();

	// ブロックの更新処理
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

	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

}

void BossScene::GameClearPhase()
{
	input_->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	//colliderManager_->UpdateWorldTransform();


	fade_->Update();



	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("CLEAR");

	}

	config_->Update();
	hp1_->Update();
	hp2_->Update();
	hp3_->Update();
	hp4_->Update();
	hp5_->Update();

	//for (const auto& enemy : enemys_) {
	//	enemy->Update();
	//	if (enemy->GetIsAlive() == false) {
	//		CreateDeathEffect({ enemy->GetWorldPosition() });
	//	}
	//}

	//deathEffect_.remove_if([](DeathEffect* hitEffects) {
	//	if (hitEffects->IsDead())
	//	{
	//		//実行時間をすぎたらメモリ削除
	//		delete hitEffects;
	//		return true;
	//	}
	//	return false;
	//	});

	//enemys_.remove_if([](Enemy* enemys) {
	//	if (enemys->GetIsAlive() == false) {
	//		delete enemys;
	//		return true;
	//	}
	//	return false;
	//	});


	for (const auto& deathEffects : deathEffect_) {
		deathEffects->Update();
	}


	//int i = 0;
	//for (Ground* ground : grounds_)
	//{
	//	i++;
	//	ground->Update();
	//	ground->Debug("ground" + i);
	//}


	player_->Update();
	//武器の更新
	weapon_->Update();

	for (const auto& enemy : enemys_)
	{
		enemy->Update();
	}

	skydome_->Update();
	goal_->Update();

	CheckAllCollisions();

	// ブロックの更新処理
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

	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

}

void BossScene::GameOverPhase()
{
	input_->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	//colliderManager_->UpdateWorldTransform();


	fade_->Update();



	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEOVER");

	}

	config_->Update();
	hp1_->Update();
	hp2_->Update();
	hp3_->Update();
	hp4_->Update();
	hp5_->Update();

	for (const auto& enemy : enemys_) {
		enemy->Update();
		if (enemy->GetIsAlive() == false) {
			CreateDeathEffect({ enemy->GetWorldPosition() });
		}
	}

	//deathEffect_.remove_if([](DeathEffect* hitEffects) {
	//	if (hitEffects->IsDead())
	//	{
	//		//実行時間をすぎたらメモリ削除
	//		delete hitEffects;
	//		return true;
	//	}
	//	return false;
	//	});

	//enemys_.remove_if([](Enemy* enemys) {
	//	if (enemys->GetIsAlive() == false) {
	//		delete enemys;
	//		return true;
	//	}
	//	return false;
	//	});


	for (const auto& deathEffects : deathEffect_) {
		deathEffects->Update();
	}

	//int i = 0;
	//for (Ground* ground : grounds_)
	//{
	//	i++;
	//	ground->Update();
	//	ground->Debug("ground" + i);
	//}


	player_->Update();
	//武器の更新
	weapon_->Update();

	for (const auto& enemy : enemys_)
	{
		enemy->Update();
	}

	skydome_->Update();
	goal_->Update();

	CheckAllCollisions();

	// ブロックの更新処理
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

	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

}


void BossScene::GenerateBlocks()
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
				model->Initialize("Resources/Level/map.obj");
				blocks_[i][j] = model;
				blocks_[i][j]->GetWorldTransform()->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

			}
		}
	}

}