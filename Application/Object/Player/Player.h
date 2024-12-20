#pragma once



#include"Camera.h"
#include"Model.h"

#include"Input.h"
#include"VectorMath.h"
#include"MathUtilty.h"
#include"GlobalVariables.h"
#include"GameObject.h"


#include <algorithm>
#include <functional>
#include<numbers>
#include <algorithm>

class MapChipField;
class Ground;
class Weapon;
class Player :public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;
	WorldTransform* GetWorldTransform() { return playerModel->GetWorldTransform(); }

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

	bool GetHitGoal()
	{
		return hitGoal;
	}

	int GetHP()
	{
		return HP;
	}

	void SetWeapon(Weapon* weapon)
	{
		weapon_ = weapon;
	}
		
	void SetGround(std::list<Ground*>grounds)
	{
		ground_ = grounds;
	}

	struct CollisionMapInfo
	{
		bool isTop = false;
		bool isGround = false;
		bool isWall = false;
		Vector3 move;
	};

	//角
	enum Corner
	{
		kRightBottom,	//右下前
		kLeftBottom,	//左下
		kRightTop,		//右上
		kLeftTop,		//左上

		kNumCorner		//要素数
	};

	//範囲矩形
	struct Rect
	{
		float left;
		float right;
		float bottom;
		float top;
	};

	void SetMapChipField(MapChipField* mapChipField)
	{
		mapChipField_ = mapChipField;
	}

private:

	GlobalVariables* grobalVariables;
	const char* groupName;

	std::unique_ptr<Model>playerModel;
	std::vector<Model*>playerModels;


	Vector3 velocity_ = {  };

	float kAcceleration = 0.01f;	//加速度
	float kAttenuation = 0.03f;	//速度減衰率
	float kLimitRunSpeed = 0.1f;	//最大速度

	//左右
	enum class LRDirection
	{
		kRight,
		kLeft,
	};


	//向いている方向
	LRDirection lrDirection = LRDirection::kRight;
	float turnFirstRotationY = 0.0f;	//旋回開始時の角度
	float turnTimer = 0.0f;				//旋回タイマー
	static inline const float kTimeTrun = 0.3f;	//旋回時間<秒>

	bool onGround_ = false;	//接地状態フラグ
	float kGravityAcceleration = 0.05f;	//重力加速度（下方向
	float kLimitFallSpeed = 0.5f;		//最大落下速度（下方向
	float kJumpAcceleration = 0.5f;	//ジャンプ初速（上方向
	float kAttenuationLanding = 0.1f;//着地時の速度減衰率
	float kAttenuationWall = 0.05f;	

	Weapon* weapon_;
	std::list<Ground*> ground_;

	CollisionMapInfo collisionMapInfo;

	float kWidth = 0.8f;
	float kHeight = 0.8f;
	float kDepth = 0.8f;

	float kBlank = 0.5f;

	bool isJump = false;

	bool hitGoal = false;


	int HP = 5;
	bool isInvincible;
	float invincibilityTimer;
	static constexpr float invincibilityDuration = 2.0f; // 無敵時間の長さ（秒）

	float alpha = 1.0f;       // 初期のアルファ値（不透明）
	float fadeSpeed = 0.01f;  // 透明になる速度

	MapChipField* mapChipField_ = nullptr;

private:

	/// <summary>
	///　移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 振り向き
	/// </summary>
	void Turn();

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void CollisionMap(CollisionMapInfo& info);

	//各方向のマップ衝突判定
	void CollisionMapTop(CollisionMapInfo& info);
	void CollisionMapBottom(CollisionMapInfo& info);
	void CollisionMapLeft(CollisionMapInfo& info);
	void CollisionMapRight(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void CollisionMove(const CollisionMapInfo& info);

	//Rect GetRect(Ground* ground);
	void HitTop(const CollisionMapInfo& info);

	/// <summary>
	/// 設置状態の切り替え処理
	/// </summary>
	void SwitchGround(const CollisionMapInfo& info);

	void CollisionWall(const CollisionMapInfo& info);


};



