/**
*	@file Goal.h
*	@brief ゴールクラスヘッダ
*/

#pragma once
#include"GameObject.h"
#include"Object/CollisionConfig.h"
#include"Model.h"

/**
* @class Goal
* @brief ゴールオブジェクトの管理を行うクラス
* @details ゴールの初期化、更新、描画、および衝突判定を行う
*/
class Goal :public GameObject
{
public:

	/**
	* @brief ゴールオブジェクトの初期化処理
	* @details ゴールオブジェクトのモデルをロードするなど、初期化を行う
	*/
	void Initialize()override;

	/**
	* @brief ゴールオブジェクトの更新処理
	* @details 毎フレーム呼ばれ、ゴールの状態を更新する
	*/
	void Update()override;

	/**
	* @brief ゴールオブジェクトの描画処理
	* @param camera カメラ情報
	* @details ゴールオブジェクトを画面に描画する
	*/
	void Draw(const Camera& camera)override;

	/**
	* @brief ゴールオブジェクトのワールド座標を取得
	* @return ゴールの現在のワールド座標
	*/
	Vector3 GetWorldPosition()override;

	/**
	* @brief ゴールオブジェクトのAABB（軸に平行なバウンディングボックス）を取得
	* @return ゴールのバウンディングボックス
	*/
	AABB GetAABB()override;

	/**
	* @brief 衝突時の処理
	* @param other 衝突したコライダー
	* @details プレイヤーや他のオブジェクトとゴールが衝突した際の処理を行う
	*/
	void OnCollision(Collider* other)override;

private:
	std::unique_ptr<Model>goalModel_;
	std::vector<Model*>goalModels_;
};