#include "GamePlayScene.h"


GamePlayScene::~GamePlayScene()
{

	delete camera_;
	//delete levelEditor;
	delete cameraController_;

	//for (Enemy* enemy : enemys_)
	//{
	//	delete enemy;
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_)
	//{
	//	delete flyEnemy;
	//}

	for (Ground* ground : grounds_)
	{
		delete ground;
	}

	//for (DeathEffect* deathEffects : deathEffect_) {
	//	delete deathEffects;
	//}

	//for (std::vector<Model*>& blockLine : blocks_)
	//{
	//	for (Model* block : blockLine)
	//	{
	//		delete block;
	//	}
	//}

	//blocks_.clear();

	//delete mapChipField_;

}

void GamePlayScene::Initialize()
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

	camera_ = new Camera;
	camera_->Initialize();

	
	//levelEditor = new LevelEditor();
	//levelEditor->LoaderJsonFile("Resources/Level/levelEditor.json");
	//levelEditor->Initialize();

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();


	SpawnBlock({ 47.8f, -1.0f, 0 }, {50.31f, 1.0f, 1.0f});
	SpawnBlock({ 6.7f, 4.5f, 0 }, { 1.0f, 1.0f, 1.0f });
	SpawnBlock({ 8.7f, 6.5f, 0 }, { 1.0f, 1.0f, 1.0f });
	SpawnBlock({ -2.15f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });
	SpawnBlock({ 50.11f, 7.8f, 0 }, { 1.0f, 9.8f, 1.0f });

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();
	//ground_->SetPosition({ 47.8f, -1.0f, 0 });
	//ground_->SetScale({ 50.31f, 1.0f, 1.0f });

	player_ = std::make_unique<Player>();
	player_->SetWeapon(weapon_.get());
	//player_->SetGround(ground_.get());
	player_->SetGround(grounds_);
	player_->Initialize();
	

	//SpawnEnemy({ 20.0f,5.5f,0.0f });
	//SpawnEnemy({ 10.0f,3.5f,0.0f });
	////SpawnEnemy({ 30.0f,1.0f,0.0f });

	//SpawnFlyEnemy({ 35.0f,8.0f,0.0f });


	//skydome_ = std::make_unique<Skydome>();
	//skydome_->Initialize();

	//goal_ = std::make_unique<Goal>();
	//goal_->Initialize();

	cameraController_ = new CameraController;
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_.get());
	cameraController_->Reset();

	/*config_.reset(Sprite::Create(configTexture_));

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
	fade_->Start(Fade::Status::FadeIn, 1.5f);*/



	phase_ = Phase::kPlay;

	//mapChipField_ = new MapChipField;
	//mapChipField_->LoadMapChipCsv("Resources/CSV/field.csv");

	//GenerateBlocks();

	//player_->SetMapChipField(mapChipField_);
	
}

void GamePlayScene::Update()
{
	switch (phase_)
	{
	case GamePlayScene::Phase::kPlay:
		//ゲームプレイフェーズの処理
		GamePlayPhase();
		break;
	case GamePlayScene::Phase::kClear:
		//自キャラ死亡時の処理
		//GameClearPhase();
		break;
	case GamePlayScene::Phase::kDeath:
		//自キャラ死亡時の処理
		//GameOverPhase();
		break;
	}

	
}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//triangle2->Draw(camera);

	//	

	//sphere->Draw(camera);

	//levelEditor->Draw(camera);

	for (Ground* ground : grounds_)
	{
		ground->Draw(camera_);
	}

	//model->Draw(camera);
	//model2->Draw(camera);


	//sprite->Draw(camera);
	//sprite2->Draw(camera);

	//particle->Draw();
	//particle2->Draw();

	//skydome_->Draw(camera_);

	ground_->Draw(camera_);

	player_->Draw(camera_);
	weapon_->Draw(camera_);
	//for (Enemy* enemy : enemys_) 
	//{
	//	enemy->Draw(camera_);
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_)
	//{
	//	flyEnemy->Draw(camera_);
	//}


	//for (DeathEffect* deathEffects : deathEffect_)
	//{
	//	deathEffects->Draw();
	//}

	// ブロックの描画処理
	//for (std::vector<Model*>& blockLine : blocks_)
	//{
	//	for (Model* block : blockLine)
	//	{
	//		if (!block)
	//		{
	//			continue;
	//		}

	//		block->Draw(camera_);
	//	}
	//}


	//goal_->Draw(camera_);

	//colliderManager_->Draw(camera);

	//config_->Draw(camera_);


	/*if (player_->GetHP() == 5)
	{

		hp1_->Draw(camera_);
		hp2_->Draw(camera_);
		hp3_->Draw(camera_);
		hp4_->Draw(camera_);
		hp5_->Draw(camera_);
	}
	if (player_->GetHP() == 4)
	{

		hp1_->Draw(camera_);
		hp2_->Draw(camera_);
		hp3_->Draw(camera_);
		hp4_->Draw(camera_);
	}
	if (player_->GetHP() == 3)
	{

		hp1_->Draw(camera_);
		hp2_->Draw(camera_);
		hp3_->Draw(camera_);
	}
	if (player_->GetHP() == 2)
	{

		hp1_->Draw(camera_);
		hp2_->Draw(camera_);
	}
	if (player_->GetHP() == 1)
	{

		hp1_->Draw(camera_);
	}

	fade_->Draw(camera_);*/



}

void GamePlayScene::CheckAllCollisions()
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

	//colliderManager_->AddColliders(ground_.get());

	for (Ground* ground : grounds_)
	{
		colliderManager_->AddColliders(ground);
	}
	//for (Enemy* enemy : enemys_) 
	//{
	//	if(enemy->GetIsAlive() == true)
	//	{
	//		//生きているときのみ
	//		colliderManager_->AddColliders(enemy);
	//	}
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_)
	//{
	//	if (flyEnemy->GetIsAlive() == true)
	//	{
	//		//生きているときのみ
	//		colliderManager_->AddColliders(flyEnemy);
	//	}
	//}

	//colliderManager_->AddColliders(goal_.get());

	//colliderManager_->AddColliders(levelEditor);
	//当たり判定
	colliderManager_->ChackAllCollisions();


}
//
//void GamePlayScene::SpawnEnemy(const Vector3& position)
//{
//	// 敵を発生させる
//	Enemy* enemy = new Enemy();
//	// 敵の初期化
//	enemy->Initialize();
//	enemy->SetPosition(position);
//
//	// リストに登録
//	enemys_.push_back(enemy);
//}
//
//void GamePlayScene::SpawnFlyEnemy(const Vector3& position)
//{
//	// 敵を発生させる
//	FlyEnemy* enemy = new FlyEnemy();
//	// 敵の初期化
//	enemy->Initialize();
//	enemy->SetPosition(position);
//
//	// リストに登録
//	flyEnemys_.push_back(enemy);
//}

void GamePlayScene::SpawnBlock(const Vector3& position, const Vector3& scale)
{
	// 敵を発生させる
	Ground* ground = new Ground();
	// 敵の初期化
	ground->Initialize();
	ground->SetPosition(position);
	ground->SetScale(scale);

	// リストに登録
	grounds_.push_back(ground);
}

//void GamePlayScene::CreateDeathEffect(Vector3 position)
//{
//	DeathEffect* newDeathEffect = new DeathEffect();
//	newDeathEffect->Initialize(camera_);
//	newDeathEffect->SetFlag(true);
//
//	newDeathEffect->SetPosition(position);
//
//	deathEffect_.push_back(newDeathEffect);
//}

void GamePlayScene::ChangePhase(Phase phase)
{
	phase_ = phase;
}

void GamePlayScene::GamePlayPhase()
{
	if (input_->TriggerKey(DIK_D))
	{
		player_->SetHP(0);
	}

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	cameraController_->Update();

	colliderManager_->UpdateWorldTransform();


	if (player_->GetHitGoal() == true)
	{
		//ChangePhase(Phase::kClear);
		//fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	if (player_->GetHP() == 0)
	{
		

		if(player_->GetEndMove())
		{
			//CreateDeathEffect({ player_->GetWorldPosition().x,player_->GetWorldPosition().y,player_->GetWorldPosition().z - 4.0f });
			//ChangePhase(Phase::kDeath);
			//fade_->Start(Fade::Status::FadeOut, 1.5f);
		}

		//player->GetWorldTransform()->translation_.y += 1.0f;

		
	}

	ground_->Update();

	//fade_->Update();

	//if (fade_->IsFinished())
	//{
	//	fade_->Stop();
	//}

	//config_->Update();
	//hp1_->Update();
	//hp2_->Update();
	//hp3_->Update();
	//hp4_->Update();
	//hp5_->Update();


	//for (Enemy* enemy : enemys_) {
	//	enemy->Update();
	//	if (enemy->GetIsAlive() == false) {
	//		CreateDeathEffect({ enemy->GetWorldPosition() });
	//	}
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_) {
	//	flyEnemy->Update();
	//	if (flyEnemy->GetIsAlive() == false) {
	//		CreateDeathEffect({ flyEnemy->GetWorldPosition() });
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

	//flyEnemys_.remove_if([](FlyEnemy* flyEnemys) {
	//	if (flyEnemys->GetIsAlive() == false) {
	//		delete flyEnemys;
	//		return true;
	//	}
	//	return false;
	//	});


	//for (DeathEffect* deathEffects : deathEffect_) {
	//	deathEffects->Update();
	//}

	//levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds_)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}

	ground_->Debug("ground");

	player_->Update();
	//武器の更新
	weapon_->Update();

	//for (Enemy* enemy : enemys_)
	//{
	//	enemy->Update();
	//}

	//for (FlyEnemy* flyEnemy : flyEnemys_)
	//{
	//	flyEnemy->Update();
	//}

	//skydome_->Update();
	//goal_->Update();

	CheckAllCollisions();


	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

	//// ブロックの更新処理
	//for (std::vector<Model*>& blockLine : blocks_)
	//{
	//	for (Model* block : blockLine)
	//	{
	//		if (!block)
	//		{
	//			continue;
	//		}

	//		block->Update();
	//	}
	//}

}

void GamePlayScene::GameClearPhase()
{
	input_->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	cameraController_->Update();

	//colliderManager_->UpdateWorldTransform();


	//fade_->Update();



	/*if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("CLEAR");

	}*/

	//config_->Update();
	//hp1_->Update();
	//hp2_->Update();
	//hp3_->Update();
	//hp4_->Update();
	//hp5_->Update();

	//for (Enemy* enemy : enemys_)
	//{
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


	//for (DeathEffect* deathEffects : deathEffect_) {
	//	deathEffects->Update();
	//}

	////levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds_)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}


	//player_->Update();
	////武器の更新
	//weapon_->Update();

	//for (Enemy* enemy : enemys_)
	//{
	//	enemy->Update();
	//}

	//skydome_->Update();
	//goal_->Update();

	//CheckAllCollisions();

	//// ブロックの更新処理
	//for (std::vector<Model*>& blockLine : blocks_)
	//{
	//	for (Model* block : blockLine)
	//	{
	//		if (!block)
	//		{
	//			continue;
	//		}

	//		block->Update();
	//	}
	//}

	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

}

void GamePlayScene::GameOverPhase()
{
	input_->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	cameraController_->Update();

	//colliderManager_->UpdateWorldTransform();


	//fade_->Update();

	

	//if (fade_->IsFinished())
	//{
	//	sceneManager_->ChangeScene("GAMEOVER");

	//}

	//config_->Update();
	//hp1_->Update();
	//hp2_->Update();
	//hp3_->Update();
	//hp4_->Update();
	//hp5_->Update();

	//for (Enemy* enemy : enemys_) {
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


	//for (DeathEffect* deathEffects : deathEffect_) {
	//	deathEffects->Update();
	//}

	////levelEditor->Update();

	int i = 0;
	for (Ground* ground : grounds_)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}


	//player_->Update();
	////武器の更新
	//weapon_->Update();

	//for (Enemy* enemy : enemys_)
	//{
	//	enemy->Update();
	//}

	//skydome_->Update();
	//goal_->Update();

	//CheckAllCollisions();


	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);

	//for (std::vector<Model*>& worldTransforBlockLine : Block_) {
	//	for (Model* Block : worldTransforBlockLine) {
	//		if (!Block)
	//			continue;
	//		Block->Update();
	//	}
	//}

	// ブロックの更新処理
	//for (std::vector<Model*>& blockLine : blocks_)
	//{
	//	for (Model* block : blockLine)
	//	{
	//		if (!block)
	//		{
	//			continue;
	//		}

	//		block->Update();
	//	}
	//}

}

//
//void GamePlayScene::GenerateBlocks()
//{
//	//要素数
//	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVertical();
//	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
//
//	//要素数を変更
//	blocks_.resize(numBlockVirtical);
//	for (uint32_t i = 0; i < numBlockVirtical; ++i)
//	{
//		blocks_[i].resize(numBlockHorizontal);
//	}
//
//	//ブロックの生成
//	for (uint32_t i = 0; i < numBlockVirtical; ++i)
//	{
//		for (uint32_t j = 0; j < numBlockHorizontal; ++j)
//		{
//			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock)
//			{
//				Model* model = new Model();
//				model->Initialize("Resources/Level/map.obj");
//				blocks_[i][j] = model;
//				blocks_[i][j]->GetWorldTransform()->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
//
//			}
//		}
//	}
//
//}
