/**
*	@file CameraController.h
*	@brief カメラコントローラークラスヘッダ
*/

#pragma once
#include"Camera.h"
#include"GlobalVariables.h"

//前方宣言
class Player;

/**
* @class CameraController
* @brief プレイヤーに追従するカメラを制御するクラス
* @details プレイヤーの位置に基づいてカメラの位置を更新し、シーン全体の視点を調整する
*/
class CameraController
{
public:
	/**
	* @brief デストラクタ
	* @details カメラコントローラが破棄される際に必要な後処理を行う
	*/
	~CameraController();

	/**
	* @brief カメラコントローラーの初期化
	* @param camera カメラオブジェクト
	* @details カメラの初期位置や動作を設定し、追従機能を有効にする
	*/
	void Initialize(AobaraEngine::Camera* camera);

	/**
	* @brief カメラの更新処理
	* @details 毎フレーム呼び出され、プレイヤーの位置に基づいてカメラの位置を補間して追従する
	*/
	void Update();

	/**
	* @brief カメラの追従対象を設定する
	* @param target プレイヤーオブジェクト
	* @details カメラが追従するプレイヤーオブジェクトを設定する
	*/
	void SetTarget(Player* target)
	{
		target_ = target;
	}

	/**
	* @brief カメラ設定のリセット
	* @details カメラの位置や設定を初期状態にリセットする
	*/
	void Reset();

	/**
	* @brief グローバル変数のカメラ設定を適用する
	* @details `GlobalVariables`内のカメラ設定を適用して動作を調整する
	*/
	void ApplyGlobalVariables();

private:
	AobaraEngine::Camera* camera_;
	const char* groupName_;
	Player* target_ = nullptr;
	GlobalVariables* globalVariables_ = nullptr;

	//追従対象とカメラの座標の差
	Vector3 targetOffset_ = { 0,0.0f,-30.0f };
	//カメラの目標座標
	Vector3 targetPosition_ = {};
	//座標補間割合
	float interpolationRate_ = 0.04f;
	


};

