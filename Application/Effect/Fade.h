/**
*	@file Fade.h
*	@brief フェードクラスヘッダ
*/

#pragma once
#include"Camera.h"
#include"Sprite.h"

/**
* @class Fade
* @brief 画面のフェードイン・フェードアウトを制御するクラス
* @details フェードの開始、停止、進行状況の管理を行う
*/
class Fade
{
public:

	/**
	* @brief フェードの初期化処理
	* @details 必要なリソースの読み込みや、初期状態の設定を行う
	*/
	void Initialize();

	/**
	* @brief フェードの更新処理
	* @details フェードの進行状況を毎フレーム更新する
	*/
	void Update();

	/**
	* @brief フェードの描画処理
	* @param camera カメラオブジェクト
	* @details フェードの状態に応じて、画面にフェード効果を描画する
	*/
	void Draw(const AobaraEngine::Camera& camera);

	/**
	* @enum Status
	* @brief フェードの状態を表す列挙型
	*/
	enum class Status
	{
		None,
		FadeIn,
		FadeOut,
	};

	/**
	* @brief フェード開始処理
	* @param status フェードの状態（FadeInまたはFadeOut）
	* @param duration フェードの持続時間（秒）
	* @details 指定した状態と時間でフェードを開始する
	*/
	void Start(Status status, float duration);

	/**
	* @brief フェードを停止する
	* @details フェードの進行を停止し、フェード効果をクリアする
	*/
	void Stop();

	/**
	* @brief フェードが終了したかどうかを判定する
	* @return フェードが終了している場合はtrue、進行中の場合はfalse
	* @details フェードの進行が終了しているかを確認するための関数
	*/
	bool IsFinished()const;

private:

	uint32_t fadeTex_;
	std::unique_ptr< AobaraEngine::Sprite> sprite_ = nullptr;

	Status status_ = Status::None;

	float duration_ = 0.0f;		//フェードの持続時間
	float counter_ = 0.0f;		//経過時間カウンター


};

