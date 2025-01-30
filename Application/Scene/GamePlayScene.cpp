#include "GamePlayScene.h"
using namespace AobaraEngine;



GamePlayScene::~GamePlayScene()
{
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

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	player_ = std::make_unique<Player>();
	player_->SetWeapon(weapon_.get());
	player_->SetGround(grounds_);
	player_->Initialize();


	SpawnEnemy({ 20.0f,5.5f,0.0f });
	SpawnEnemy({ 10.0f,3.5f,0.0f });

	SpawnFlyEnemy({ 35.0f,8.0f,0.0f });


	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize();

	cameraController_ = std::make_unique<CameraController>();
	cameraController_->Initialize(camera_.get());
	cameraController_->SetTarget(player_.get());
	cameraController_->Reset();

	config_.reset(AobaraEngine::Sprite::Create(configTexture_));

	//初期化
	for (int i = 0; i < hpTextures_.size(); ++i) {
		hpTextures_[i].reset(AobaraEngine::Sprite::Create(HPTexture_));
	}

	// スケールと位置を一括設定
	for (size_t i = 0; i < hpTextures_.size(); ++i) {
		hpTextures_[i]->GetWorldTransform()->scale_ = { 2.0f, 2.0f, 2.0f };
		hpTextures_[i]->GetWorldTransform()->translation_ = { 20.0f + static_cast<float>(i) * 20.0f, 20.0f, 0.0f };
	}


	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.5f);



	phase_ = Phase::kPlay;

	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("Resources/CSV/field.csv");

	GenerateBlocks();

	player_->SetMapChipField(mapChipField_.get());

	lastInputTime = std::chrono::steady_clock::now();


}

void GamePlayScene::Update()
{
	switch (phase_)
	{
	case GamePlayScene::Phase::kPlay:
		// ゲームプレイフェーズの処理
		GamePlayPhase();
		break;
	case GamePlayScene::Phase::kClear:
		// ゲームクリアフェーズの処理
		GameClearPhase();
		break;
	case GamePlayScene::Phase::kDeath:
		// 自キャラ死亡時の処理
		GameOverPhase();
		break;
	}

#ifdef _DEBUG

	camera_->CameraDebug();

#endif // _DEBUG

	cameraController_->Update();

	player_->Update();
	//武器の更新
	weapon_->Update();

	skydome_->Update();
	goal_->Update();

	//HP(UI)の更新
	for (int i = 0; i < hpTextures_.size(); ++i) {
		hpTextures_[i]->Update();
	}

	/*敵の更新処理*/
	for (const auto& enemy : enemys_) {
		enemy->Update();
		if (enemy->GetIsAlive() == false) {
			CreateDeathEffect({ enemy->GetWorldPosition() });
		}
	}

	for (const auto& flyEnemy : flyEnemys_) {
		flyEnemy->Update();
		if (flyEnemy->GetIsAlive() == false) {
			CreateDeathEffect({ flyEnemy->GetWorldPosition() });
		}
	}

	int i = 0;
	for (Ground* ground : grounds_)
	{
		i++;
		ground->Update();
		ground->Debug("ground" + i);
	}

	// ブロックの更新処理
	for (std::vector<std::unique_ptr<Model>>& blockLine : blocks_)
	{
		for (std::unique_ptr<Model>& block : blockLine)
		{
			if (!block)
			{
				continue;
			}

			block->Update();
		}
	}

	//ゲーム中に死んだ敵の後処理
	enemys_.remove_if(
		[](const std::unique_ptr<Enemy>& enemy) {
			return !enemy->GetIsAlive();
		});

	flyEnemys_.remove_if(
		[](const std::unique_ptr<FlyEnemy>& enemy) {
			return !enemy->GetIsAlive();
		});

	deathEffect_.remove_if([](const std::unique_ptr<DeathEffect>& hitEffect) {
		return hitEffect->IsDead(); // IsDead() が true なら削除
		});

	for (const auto& deathEffects : deathEffect_) {
		deathEffects->Update();
	}

	//当たり判定
	CheckAllCollisions();
}

void GamePlayScene::Draw()
{

	for (Ground* ground : grounds_)
	{
		ground->Draw(*camera_);
	}

	skydome_->Draw(*camera_);
	player_->Draw(*camera_);
	weapon_->Draw(*camera_);
	for (const auto& enemy : enemys_)
	{
		enemy->Draw(*camera_);
	}

	for (const auto& flyEnemy : flyEnemys_)
	{
		flyEnemy->Draw(*camera_);
	}


	for (const auto& deathEffects : deathEffect_)
	{
		deathEffects->Draw();
	}

	// ブロックの描画処理
	for (std::vector<std::unique_ptr<Model>>& blockLine : blocks_)
	{
		for (std::unique_ptr<Model>& block : blockLine)
		{
			if (!block)
			{
				continue;
			}

			block->Draw(*camera_);
		}
	}


	goal_->Draw(*camera_);

	//colliderManager_->Draw(camera);

	ConfigDraw();

	if (configInstruction == true)
	{
		config_->Draw(*camera_);
	}

	// HP に応じて描画
	int hp = player_->GetHP();
	for (int i = 0; i < hp; ++i) {
		if (hpTextures_[i]) {
			hpTextures_[i]->Draw(*camera_);
		}
	}

	fade_->Draw(*camera_);



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
	for (const auto& enemy : enemys_)
	{
		if (enemy->GetIsAlive() == true)
		{
			//生きているときのみ
			colliderManager_->AddColliders(enemy.get());
		}
	}

	for (const auto& flyEnemy : flyEnemys_)
	{
		if (flyEnemy->GetIsAlive() == true)
		{
			//生きているときのみ
			colliderManager_->AddColliders(flyEnemy.get());
		}
	}

	colliderManager_->AddColliders(goal_.get());
	//当たり判定
	colliderManager_->ChackAllCollisions();


}

void GamePlayScene::SpawnEnemy(const Vector3& position)
{
	// 敵を発生させる
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	// 敵の初期化
	enemy->Initialize();
	enemy->SetPosition(position);
	enemy->SetPlayer(player_.get());

	enemy->AttackReset();

	// リストに登録
	enemys_.push_back(std::move(enemy));
}

void GamePlayScene::SpawnFlyEnemy(const Vector3& position)
{
	// 敵を発生させる
	std::unique_ptr<FlyEnemy> enemy = std::make_unique<FlyEnemy>();
	// 敵の初期化
	enemy->Initialize();
	enemy->SetPosition(position);

	// リストに登録
	flyEnemys_.push_back(std::move(enemy));
}

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

void GamePlayScene::CreateDeathEffect(Vector3 position)
{
	std::unique_ptr<DeathEffect> newDeathEffect = std::make_unique< DeathEffect>();
	newDeathEffect->Initialize(camera_.get());
	newDeathEffect->SetFlag(true);

	newDeathEffect->SetPosition(position);

	deathEffect_.push_back(std::move(newDeathEffect));
}

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

	colliderManager_->UpdateWorldTransform();


	if (player_->GetHitGoal() == true)
	{
		ChangePhase(Phase::kClear);
		fade_->Start(Fade::Status::FadeOut, 1.5f);
	}

	if (player_->GetHP() == 0)
	{
		if (player_->GetEndMove())
		{
			CreateDeathEffect({ player_->GetWorldPosition().x,player_->GetWorldPosition().y,player_->GetWorldPosition().z - 4.0f });
			ChangePhase(Phase::kDeath);
			fade_->Start(Fade::Status::FadeOut, 1.5f);
		}
	}

	fade_->Update();

	if (fade_->IsFinished())
	{
		fade_->Stop();
	}

	config_->Update();

}

void GamePlayScene::GameClearPhase()
{
	fade_->Update();

	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("CLEAR");

	}
}

void GamePlayScene::GameOverPhase()
{

	fade_->Update();

	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEOVER");

	}
}


void GamePlayScene::GenerateBlocks()
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
				std::unique_ptr<Model> model = std::make_unique< Model>();
				model->Initialize("Resources/Level/map.obj");
				blocks_[i][j] = std::move(model);
				blocks_[i][j]->GetWorldTransform()->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

			}
		}
	}

}

void GamePlayScene::ConfigDraw()
{
	auto now = std::chrono::steady_clock::now();

	// キー入力があるかチェック
	if (input_->GetInstance()->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT) ||
		input_->GetInstance()->PushKey(DIK_UP) || input_->GetInstance()->PushKey(DIK_SPACE))
	{
		if (!configDraw)
		{
			// 起動後初めてキーが押された
			configDraw = true;
		}
		configInstruction = false;      // 描画を停止
		lastInputTime = now;            // 最後の入力時間を更新
	}

	// 入力が5秒間なかった場合に描画を再開
	const float noInputDuration = 5.0f; // 秒
	float elapsedTime = std::chrono::duration<float>(now - lastInputTime).count();
	if (elapsedTime >= noInputDuration) {
		configInstruction = true;       // 描画フラグを再設定
	}

	// 描画の制御
	if (configInstruction) {
		config_->SetisInvisible(false); // テクスチャを表示
	}
	else {
		config_->SetisInvisible(true);  // テクスチャを非表示
	}
}
