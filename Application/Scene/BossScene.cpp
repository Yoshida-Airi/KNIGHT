#include "BossScene.h"
#include"SceneManager.h"


BossScene::~BossScene()
{

	delete camera;
	delete levelEditor;
	delete cameraController;

	for (Enemy* enemy : enemys)
	{
		delete enemy;
	}

	for (FlyEnemy* flyEnemy : flyEnemys)
	{
		delete flyEnemy;
	}

	for (Ground* ground : grounds)
	{
		delete ground;
	}

	for (DeathEffect* deathEffects : deathEffect_) {
		delete deathEffects;
	}

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
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	//当たり判定処理の設定
	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();


	//テクスチャの読み込み
	uvTexture = texture->LoadTexture("Resources/SampleAssets/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/SampleAssets/monsterBall.png");
	Doll = texture->LoadTexture("Resources/SampleAssets/Doll.png");
	circle = texture->LoadTexture("Resources/SampleAssets/circle.png");
	configTexture = texture->LoadTexture("Resources/Scene/config.png");
	HPTexture = texture->LoadTexture("Resources/Object/Heart.png");

	camera = new Camera;
	camera->Initialize();
	camera->transform.translate={16.0f,66.3f,-42.0f};

	levelEditor = new LevelEditor();
	levelEditor->LoaderJsonFile("Resources/Level/levelEditor.json");
	levelEditor->Initialize();

	weapon = std::make_unique<Weapon>();
	weapon->Initialize();


	//SpawnBlock({ 47.8f, -1.0f, 0 }, { 50.31f, 1.0f, 1.0f });
	//SpawnBlock({ -2.15f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });
	//SpawnBlock({ 20.11f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });

	player = std::make_unique<Player>();
	player->SetWeapon(weapon.get());
	player->SetGround(grounds);
	player->Initialize();

	boss = std::make_unique<Boss>();
	boss->Initialize();
	boss->SetPlayer(player.get());

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	goal = std::make_unique<Goal>();
	goal->Initialize();

	//cameraController = new CameraController;
	//cameraController->Initialize(camera);
	//cameraController->SetTarget(player.get());
	//cameraController->Reset();

	config.reset(Sprite::Create(configTexture));

	hp1.reset(Sprite::Create(HPTexture));
	hp2.reset(Sprite::Create(HPTexture));
	hp3.reset(Sprite::Create(HPTexture));
	hp4.reset(Sprite::Create(HPTexture));
	hp5.reset(Sprite::Create(HPTexture));

	hp1->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp2->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp3->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp4->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };
	hp5->GetWorldTransform()->scale_ = { 2.0f,2.0f,2.0f };

	hp1->GetWorldTransform()->translation_ = { 20.0f,20.0f,0.0f };
	hp2->GetWorldTransform()->translation_ = { 40.0f,20.0f,0.0f };
	hp3->GetWorldTransform()->translation_ = { 60.0f,20.0f,0.0f };
	hp4->GetWorldTransform()->translation_ = { 80.0f,20.0f,0.0f };
	hp5->GetWorldTransform()->translation_ = { 100.0f,20.0f,0.0f };

	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.5f);



	phase_ = Phase::kPlay;

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/CSV/bossField.csv");

	GenerateBlocks();

	player->SetMapChipField(mapChipField_);
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


	for (Ground* ground : grounds)
	{
		ground->Draw(camera);
	}


	skydome->Draw(camera);
	player->Draw(camera);
	weapon->Draw(camera);
	for (Enemy* enemy : enemys)
	{
		enemy->Draw(camera);
	}

	for (FlyEnemy* flyEnemy : flyEnemys)
	{
		flyEnemy->Draw(camera);
	}


	for (DeathEffect* deathEffects : deathEffect_)
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

			block->Draw(camera);
		}
	}

	boss->Draw(camera);

	goal->Draw(camera);

	//colliderManager_->Draw(camera);

	config->Draw(camera);


	if (player->GetHP() == 5)
	{

		hp1->Draw(camera);
		hp2->Draw(camera);
		hp3->Draw(camera);
		hp4->Draw(camera);
		hp5->Draw(camera);
	}
	if (player->GetHP() == 4)
	{

		hp1->Draw(camera);
		hp2->Draw(camera);
		hp3->Draw(camera);
		hp4->Draw(camera);
	}
	if (player->GetHP() == 3)
	{

		hp1->Draw(camera);
		hp2->Draw(camera);
		hp3->Draw(camera);
	}
	if (player->GetHP() == 2)
	{

		hp1->Draw(camera);
		hp2->Draw(camera);
	}
	if (player->GetHP() == 1)
	{

		hp1->Draw(camera);
	}

	fade_->Draw(camera);
}

void BossScene::CheckAllCollisions()
{


	//コライダーのリストをクリア
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player.get());

	if (weapon->GetIsAttack() == true)
	{
		//攻撃中のみ
		colliderManager_->AddColliders(weapon.get());
	}
	colliderManager_->AddColliders(boss.get());

	colliderManager_->AddColliders(goal.get());

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
	grounds.push_back(ground);
}

void BossScene::CreateDeathEffect(Vector3 position)
{
	DeathEffect* newDeathEffect = new DeathEffect();
	newDeathEffect->Initialize(camera);
	newDeathEffect->SetFlag(true);

	newDeathEffect->SetPosition(position);

	deathEffect_.push_back(newDeathEffect);
}

void BossScene::ChangePhase(Phase phase)
{
	phase_ = phase;
}

void BossScene::GamePlayPhase()
{
	input->TriggerKey(DIK_0);

	if (player->GetWorldTransform()->translation_.x >= 18.0f)
	{
		player->GetWorldTransform()->translation_.x= 18.0f;
	}

	camera->UpdateMatrix();

#ifdef _DEBUG

	camera->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	colliderManager_->UpdateWorldTransform();


	if (boss->GetIsAlive() == false)
	{
		ChangePhase(Phase::kClear);
		fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	if (player->GetHP() == 0)
	{
		ChangePhase(Phase::kDeath);
		CreateDeathEffect({ player->GetWorldPosition() });
		fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	fade_->Update();

	if (fade_->IsFinished())
	{
		fade_->Stop();
	}

	config->Update();
	hp1->Update();
	hp2->Update();
	hp3->Update();
	hp4->Update();
	hp5->Update();

	boss->Update();

	if (boss->GetIsAlive() == false)
	{
		CreateDeathEffect({ boss->GetWorldPosition() });
	}


	deathEffect_.remove_if([](DeathEffect* hitEffects) {
		if (hitEffects->IsDead())
		{
			//実行時間をすぎたらメモリ削除
			delete hitEffects;
			return true;
		}
		return false;
		});

	enemys.remove_if([](Enemy* enemys) {
		if (enemys->GetIsAlive() == false) {
			delete enemys;
			return true;
		}
		return false;
		});

	flyEnemys.remove_if([](FlyEnemy* flyEnemys) {
		if (flyEnemys->GetIsAlive() == false) {
			delete flyEnemys;
			return true;
		}
		return false;
		});


	for (DeathEffect* deathEffects : deathEffect_) {
		deathEffects->Update();
	}

	levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}


	player->Update();
	//武器の更新
	weapon->Update();

	for (Enemy* enemy : enemys)
	{
		enemy->Update();
	}

	for (FlyEnemy* flyEnemy : flyEnemys)
	{
		flyEnemy->Update();
	}

	skydome->Update();
	goal->Update();

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
	input->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	//colliderManager_->UpdateWorldTransform();


	fade_->Update();



	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("CLEAR");

	}

	config->Update();
	hp1->Update();
	hp2->Update();
	hp3->Update();
	hp4->Update();
	hp5->Update();

	for (Enemy* enemy : enemys) {
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

	enemys.remove_if([](Enemy* enemys) {
		if (enemys->GetIsAlive() == false) {
			delete enemys;
			return true;
		}
		return false;
		});


	for (DeathEffect* deathEffects : deathEffect_) {
		deathEffects->Update();
	}

	levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}


	player->Update();
	//武器の更新
	weapon->Update();

	for (Enemy* enemy : enemys)
	{
		enemy->Update();
	}

	skydome->Update();
	goal->Update();

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
	input->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera->CameraDebug();

#endif // _DEBUG

	//cameraController->Update();

	//colliderManager_->UpdateWorldTransform();


	fade_->Update();



	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEOVER");

	}

	config->Update();
	hp1->Update();
	hp2->Update();
	hp3->Update();
	hp4->Update();
	hp5->Update();

	for (Enemy* enemy : enemys) {
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

	enemys.remove_if([](Enemy* enemys) {
		if (enemys->GetIsAlive() == false) {
			delete enemys;
			return true;
		}
		return false;
		});


	for (DeathEffect* deathEffects : deathEffect_) {
		deathEffects->Update();
	}

	levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}


	player->Update();
	//武器の更新
	weapon->Update();

	for (Enemy* enemy : enemys)
	{
		enemy->Update();
	}

	skydome->Update();
	goal->Update();

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