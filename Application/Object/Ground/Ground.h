/**
*	@file Ground.h
*	@brief 道クラスヘッダ
*/

#pragma once
#include"Model.h"
#include"GameObject.h"

/**
* @class Ground
* @brief 地面オブジェクトの管理を行うクラス
* @details 地面の初期化、更新、描画、および衝突判定を行う
*/
class Ground :public GameObject
{
public:

	/**
	* @brief 地面オブジェクトの初期化処理
	* @details 地面のモデルをロードするなど、初期化を行う
	*/
	void Initialize()override;

	/**
	* @brief 地面オブジェクトの更新処理
	* @details 毎フレーム呼ばれ、地面の状態を更新する
	*/
	void Update()override;

	/**
	* @brief 地面オブジェクトの描画処理
	* @param camera カメラ情報
	* @details 地面オブジェクトを画面に描画する
	*/
	void Draw(Camera* camera)override;

	/**
	* @brief 地面オブジェクトのワールド座標を取得
	* @return 地面の現在のワールド座標
	*/
	Vector3 GetWorldPosition()override;

	WorldTransform* GetWorldTransform() { return groundModel_->GetWorldTransform(); };

	/**
	* @brief 地面オブジェクトのAABB（軸に平行なバウンディングボックス）を取得
	* @return 地面のバウンディングボックス
	*/
	AABB GetAABB()override;

	/**
	* @brief 衝突時の処理
	* @param other 衝突したコライダー
	*/
	[[maybe_unused]] void OnCollision([[maybe_unused]] Collider* other)override;

	/**
	* @brief デバッグ用処理
	* @param name 地面オブジェクトの名前
	* @details デバッグ用途での表示や処理を行う
	*/
	void Debug(const char* name);

	/**
	* @brief 地面オブジェクトの位置を設定
	* @param position 新しいワールド座標
	*/
	void SetPosition(Vector3 position)
	{
		groundModel_->GetWorldTransform()->translation_ = position;
	}

	/**
	* @brief 地面オブジェクトのスケールを設定
	* @param scale 新しいスケール値
	*/
	void SetScale(Vector3 scale)
	{
		groundModel_->GetWorldTransform()->scale_ = scale;
	}

	/**
	* @brief 地面オブジェクトのスケールを取得
	* @return 現在のスケール値
	*/
	Vector3 GetScale()
	{
		return groundModel_->GetWorldTransform()->scale_;
	}

private:
	


	std::unique_ptr<Model>groundModel_ = nullptr;
	std::vector<Model*>groundModels_;
};

