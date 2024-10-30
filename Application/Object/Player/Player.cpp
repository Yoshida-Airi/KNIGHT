#define NOMINMAX

#include "Player.h"
#include"Object/CollisionConfig.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"
#include"Object/Ground/MapChipField.h"

void Player::Initialize()
{
	grobalVariables_ = GlobalVariables::GetInstance();
	groupName_ = "Player";

	playerModel_.reset(Model::Create("Resources/Object/Player/player.obj"));
	playerModels_ = { playerModel_.get() };

	Collider::SetTypeID(CollisionTypeDef::kPlayer);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	GameObject::Initialize();
	GameObject::SetModel(playerModels_);

	weapon_->SetParent(playerModel_.get());

	//モデルの初期設定
	//playerModel->GetWorldTransform()->rotation_.y = std::numbers::pi_v<float> / 2.0f;
	playerModel_->GetWorldTransform()->translation_.x = 7.2f;
	playerModel_->GetWorldTransform()->translation_.y = 75.0f;
	playerModel_->GetWorldTransform()->translation_.y = 10.0f;
	//playerModel->GetWorldTransform()->rotation_.y = 1.5f;


	grobalVariables_->AddItem(groupName_, "Acceleration", kAcceleration_);
	grobalVariables_->AddItem(groupName_, "Attenuation", kAttenuation_);
	grobalVariables_->AddItem(groupName_, "LimitRunSpeed", kLimitRunSpeed_);
	grobalVariables_->AddItem(groupName_, "GravityAcceleration", kGravityAcceleration_);
	grobalVariables_->AddItem(groupName_, "LimitFallSpead", kLimitFallSpeed_);
	grobalVariables_->AddItem(groupName_, "JumpAcceleration", kJumpAcceleration_);
	grobalVariables_->AddItem(groupName_, "playerWidth", kWidth_);
	grobalVariables_->AddItem(groupName_, "playerHeight", kHeight_);


	phase_ = Phase::kPlay;

}

void Player::Update()
{


	switch (phase_)
	{
	case Player::Phase::kPlay:
		//ゲームプレイフェーズの処理
		GamePlayPhase();
		break;
	case Player::Phase::kDeath:
		//自キャラ死亡時の処理
		DeathPhase();
		break;
	}

}

void Player::Draw(Camera* camera)
{
	playerModel_->Draw(camera);

}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = playerModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = playerModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = playerModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Player::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - playerModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - playerModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - playerModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + playerModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + playerModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + playerModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Player::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kGoal))
	{
		hitGoal_ = true;
	}
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		if (!isInvincible_)
		{
			hp_ -= 1;
			isInvincible_ = true;
			invincibilityTimer_ = invincibilityDuration_;
		}
	}
}

void Player::Move()
{
	///* --プレイヤーの移動処理-- */

	velocity_ = Add(velocity_, { 0.0f, -kGravityAcceleration_, 0.0f });
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);

	//移動処理
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	{
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			//旋回処理
			if (lrDirection_ != LRDirection::kRight)
			{
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = std::numbers::pi_v<float>;
				turnTimer_ = 1.0f;
			}
			//減速処理
			if (velocity_.x < 0.0f)
			{
				velocity_.x *= (1.0f - kAttenuation_);
			}
			acceleration.x += kAcceleration_;
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			//旋回処理
			if (lrDirection_ != LRDirection::kLeft)
			{
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = 0.0f;
				turnTimer_ = 1.0f;
			}
			//減速処理
			if (velocity_.x > 0.0f)
			{
				velocity_.x *= (1.0f - kAttenuation_);
			}
			acceleration.x -= kAcceleration_;
		}
		velocity_ = Add(velocity_, acceleration);
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed_, kLimitRunSpeed_);
	} // 非按键时速度衰减
	else
	{
		velocity_.x *= (1.0f - kAttenuation_);
	}


	//ジャンプ処理
	if (onGround_)
	{

		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (!isJump_)
			{
				velocity_ = Add(velocity_, { 0.0f, kJumpAcceleration_, 0.0f });
				isJump_ = true;
			}
			else
			{
				isJump_ = false;
			}
		}
	}
	else//空中
	{
		//落下速度
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration_, 0));
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
	}
}

void Player::Turn()
{
	/* --プレイヤーの旋回処理-- */


	if (turnTimer_ > 0.0f)
	{
		turnTimer_ = 1.0f / 30.0f;

		float destinationRotationYTable[] =
		{
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float>*3.0f / 2.0f
		};
		//角度の取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//角度を変更する
		playerModel_->GetWorldTransform()->rotation_.y = LerpShortTranslate(playerModel_->GetWorldTransform()->rotation_.y, destinationRotationY, turnTimer_);
	}
}

void Player::Jump()
{
	//接地状態
	if (onGround_)
	{
		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (!isJump_)
			{
				//ジャンプ初速
				velocity_ = Add(velocity_, Vector3(0, kJumpAcceleration_, 0));
				isJump_ = true;
			}
		}
		else
		{
			isJump_ = false;
		}



	}
	else//空中
	{
		//落下速度
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration_, 0));
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
	}


}

void Player::ChangePhase(Phase phase)
{
	phase_ = phase;
}

void Player::ApplyGlobalVariables()
{
	kAcceleration_ = grobalVariables_->GetFloatValue(groupName_, "Acceleration");
	kAttenuation_ = grobalVariables_->GetFloatValue(groupName_, "Attenuation");
	kLimitRunSpeed_ = grobalVariables_->GetFloatValue(groupName_, "LimitRunSpeed");
	kGravityAcceleration_ = grobalVariables_->GetFloatValue(groupName_, "GravityAcceleration");
	kLimitFallSpeed_ = grobalVariables_->GetFloatValue(groupName_, "LimitFallSpead");
	kJumpAcceleration_ = grobalVariables_->GetFloatValue(groupName_, "JumpAcceleration");
	kWidth_ = grobalVariables_->GetFloatValue(groupName_, "playerWidth");
	kHeight_ = grobalVariables_->GetFloatValue(groupName_, "playerHeight");

}


void Player::BehaviorRootUpdate()
{
	//移動処理
	Move();
	//旋回処理
	Turn();
	//ジャンプ処理
	//Jump();
}

void Player::BehaviorAttackUpdate()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		weapon_->SetIsAttack(true);
	}
	else
	{
		weapon_->SetIsAttack(false);
	}
}

void Player::CollisionMap(CollisionMapInfo& info)
{
	CollisionMapTop(info);
	CollisionMapBottom(info);
	CollisionMapLeft(info);
	CollisionMapRight(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info)
{
	if (info.move.y <= 0)
	{
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel_->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	//左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { 0.0f, kHeight_ / 2.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(playerModel_->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.bottom - playerModel_->GetWorldTransform()->translation_.y) - (kHeight_ / 2.0f) + kBlank_;
		info.move.y = std::max(0.0f, moveY);
		info.isTop = true;
	}
	else
	{
		info.isTop = false;
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info)
{

	if (info.move.y >= 0)
	{
		return;
	}


	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel_->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}


	//左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	//右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { 0.0f, kHeight_ / 2.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(playerModel_->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.top - playerModel_->GetWorldTransform()->translation_.y) + (kHeight_ / 2.0f) + kBlank_;

		info.move.y = std::min(0.0f, moveY);
		info.isGround = true;
	}
	else
	{
		info.isGround = false;
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info)
{
	if (info.move.x >= 0)
	{
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel_->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	//左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}
	//左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { kWidth_ / 2.0f, 0.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(playerModel_->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.right - playerModel_->GetWorldTransform()->translation_.x + kWidth_ / 2 + kBlank_;
		info.move.x = std::min(0.0f, moveX);
		info.isWall = true;
	}


}

void Player::CollisionMapRight(CollisionMapInfo& info)
{
	if (info.move.x <= 0)
	{
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel_->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}
	//右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { kWidth_ / 2.0f, 0.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(playerModel_->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.left - playerModel_->GetWorldTransform()->translation_.x - kWidth_ / 2 - kBlank_;
		info.move.x = std::max(0.0f, moveX);
		info.isWall = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{

	Vector3 offsetTable[kNumCorner] =
	{
		{kWidth_ / 2.0f,-kHeight_ / 2.0f,0.0f},
		{-kWidth_ / 2.0f,-kHeight_ / 2.0f,0.0f},
		{kWidth_ / 2.0f,kHeight_ / 2.0f,0.0f},
		{-kWidth_ / 2.0f,kHeight_ / 2.0f,0.0f},
	};

	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::CollisionMove(const CollisionMapInfo& info)
{
	playerModel_->GetWorldTransform()->translation_ = Add(playerModel_->GetWorldTransform()->translation_, info.move);
}


void Player::HitTop(const CollisionMapInfo& info)
{
	if (info.isTop)
	{
		//ImGui::Text("hit");
		velocity_.y = 0;
	}
}
void Player::CollisionWall(const CollisionMapInfo& info)
{
	if (info.isWall)
	{
		velocity_.x *= (1.0f - kAttenuationWall_);
		//velocity_.x = 0;
	}
}

void Player::GamePlayPhase()
{
	GameObject::Update();
	ApplyGlobalVariables();
	//デバッグ
	playerModel_->ModelDebug("player");



	BehaviorRootUpdate();
	BehaviorAttackUpdate();

	//速度加算
	//playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, velocity_);


	collisionMapInfo_.move = velocity_;
	//マップ衝突チェック
	CollisionMap(collisionMapInfo_);

	CollisionMove(collisionMapInfo_);
	HitTop(collisionMapInfo_);
	CollisionWall(collisionMapInfo_);
	SwitchGround(collisionMapInfo_);

#ifdef _DEBUG
	ImGui::Text("x %d", collisionMapInfo_.move.x);
	ImGui::Text("y %d", collisionMapInfo_.move.y);
	ImGui::Text("z %d", collisionMapInfo_.move.z);
	ImGui::Text("isTop %d", collisionMapInfo_.isTop);
	ImGui::Text("isGround %d", collisionMapInfo_.isGround);
	ImGui::Text("isWall %d", collisionMapInfo_.isWall);
	ImGui::Text("grand %d", onGround_);
	ImGui::Text("HP %d", hp_);
#endif // _DEBUG

	//無敵時間
	if (isInvincible_)
	{
		invincibilityTimer_ -= 1.0f / 60.0f;
		if (invincibilityTimer_ <= 0.0f)
		{
			isInvincible_ = false;
		}
	}

	if (hp_ == 0)
	{
		//// アルファ値を段々と減少させる
		//alpha_ -= fadeSpeed_;
		//if (alpha_ < 0.0f) {
		//	alpha_ = 0.0f;  // アルファ値が負にならないように制限
		//}

		//playerModel_->SetMaterial({ 1.0f,1.0f,1.0f,alpha_ });

		ChangePhase(Phase::kDeath);
	}
}

void Player::DeathPhase()
{
	GameObject::Update();
	ApplyGlobalVariables();
	//デバッグ
	playerModel_->ModelDebug("player");

	bool isRising = true;
	float interpolationRate_ = 0.04f;

	Vector3 currentPosition = playerModel_->GetWorldTransform()->translation_;

	// アルファ値を段々と減少させる
	alpha_ -= fadeSpeed_;
	if (alpha_ < 0.0f) {
		alpha_ = 0.0f;  // アルファ値が負にならないように制限
	}

	playerModel_->SetMaterial({ 1.0f,1.0f,1.0f,alpha_ });

	if (isRising)
	{
		static Vector3 endPosition = { currentPosition.x,currentPosition.y + 2.0f,currentPosition.z };
		playerModel_->GetWorldTransform()->translation_ = Lerp(currentPosition, endPosition, interpolationRate_);

	

		// 目標位置に到達したかチェック
		if (fabs(currentPosition.y - endPosition.y) < 0.1f)
		{
			// 目標位置に到達したとみなす
			//playerModel_->GetWorldTransform()->translation_.y = endPosition.y;

			isRising = false; // 上昇完了
			endMove_ = true;
		}
	}
}


void Player::SwitchGround(const CollisionMapInfo& info)
{

	if (onGround_)
	{
		//接地状態の処理
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
		else
		{
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(Add(playerModel_->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
			}

			if (info.move.y <= 0)
			{
				return;
			}

			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				hit = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				hit = true;
			}

			//落下開始
			if (!hit)
			{
				//空中状態に切り替える
				onGround_ = false;
			}
		}
	}
	else
	{
		//空中状態の処理
		if (info.isGround)
		{
			//着地状態に切り替える
			onGround_ = true;
			//着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding_);
			//y座標をゼロにする
			velocity_.y = 0.0f;
		}
	}

}


