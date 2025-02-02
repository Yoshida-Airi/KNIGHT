#include "Enemy.h"
#include"Object/CollisionConfig.h"
#include "Object/Player/Player.h"
using namespace AobaraEngine;
Enemy::~Enemy()
{
}

void Enemy::Initialize()
{ 
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	enemyModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels_ = { enemyModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels_);
	enemyModel_->SetMaterial({ 1.0f,0.0,0.0f,1.0f });



	
}

void Enemy::Update()
{
	GameObject::Update();

	// 終了したタイマーを削除
	timedCalls_.remove_if([](const std::unique_ptr<TimedCall>& timecall) {
		return timecall->isFinished(); // デスフラグが立っているか確認
		});


	//デスフラグの立った弾を削除
	bullets_.remove_if([](const std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead(); // デスフラグが立っているか確認
		});


	enemyModel_->ModelDebug("enemy");
	//enemyModel->GetWorldTransform()->translation_.x += 0.03f;

	// 移動距離を更新
	if (movingRight_) {
		enemyModel_->GetWorldTransform()->translation_.x += moveSpeed_;
		traveledDistance_ += moveSpeed_;
	}
	else {
		enemyModel_->GetWorldTransform()->translation_.x -= moveSpeed_;
		traveledDistance_ -= moveSpeed_;
	}

	// 一定の距離を移動したら方向を反転
	if (traveledDistance_ >= moveDistance_) {
		movingRight_ = false;
	}
	else if (traveledDistance_ <= 0.0f) {
		movingRight_ = true;
	}

	//タイマーの更新
	for (std::unique_ptr<TimedCall>& timedCall : this->timedCalls_)
	{
		timedCall->Update();
	}

	//弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (isHit == true)
	{
		enemyModel_->GetWorldTransform()->translation_.z += 0.3f;
		enemyModel_->GetWorldTransform()->translation_.y += 0.1f;
		enemyModel_->GetWorldTransform()->rotation_.y += 0.1f;

		if (enemyModel_->GetWorldTransform()->translation_.y >= 10.0f)
		{
			isAlive_ = false;
			isHit = false;
		}
	}

}

void Enemy::Draw(const AobaraEngine::Camera& camera)
{
	if (isAlive_ == false)
	{



		//死んでいたら描画しない
		//enemyModel_->SetisInvisible(true);
	}

	enemyModel_->Draw(camera);

	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(camera);
	}
}


Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Enemy::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Enemy::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		isHit = true;
	}
}

void Enemy::AttackReset()
{

	Fire();
	// 発射タイマーをセットする
	timedCalls_.push_back(std::move(std::make_unique<TimedCall>(std::bind(&Enemy::AttackReset, this), kFireInterval)));
}

void Enemy::DeleteBullet()
{
	timedCalls_.clear();
}

void Enemy::Fire()
{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	/*Vector3 velocity(0, 0, kBulletSpeed);*/

	// 自キャラのワールド座標を取得する
	Vector3 worldPlayer = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 worldEnemy = GetWorldPosition();

	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 differenceVector;
	differenceVector.x = worldPlayer.x - worldEnemy.x;
	differenceVector.y = worldPlayer.y - worldEnemy.y;
	differenceVector.z = worldPlayer.z - worldEnemy.z;

	// 正規化
	Vector3 normarizeVector;
	normarizeVector = Normalize(differenceVector);

	// ベクトルの長さを速さに合わせる
	Vector3 velocity;
	velocity.x = normarizeVector.x * kBulletSpeed;
	velocity.y = normarizeVector.y * kBulletSpeed;
	velocity.z = normarizeVector.z * kBulletSpeed;
	//球を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique< EnemyBullet>();
	newBullet->SetPlayer(player_);
	newBullet->Initialize();
	newBullet->SetPosition(GetWorldPosition());

	//弾の登録
	bullets_.push_back(std::move(newBullet));

}