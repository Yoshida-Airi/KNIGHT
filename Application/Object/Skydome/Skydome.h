/**
*	@file Skydome.h
*	@brief 天球クラスヘッダ
*/

#pragma once
#include"Model.h"

/**
 * @class Skydome
 * @brief 天球クラス
 * @details シーンの背景としての天球のモデルを管理し、描画および更新を行う。
 */
class Skydome
{
public:

	/**
	 * @brief 初期化処理
	 * @details 天球の初期化を行い、モデルのロードなどを行う。
	 */
	void Initialize();

	/**
	 * @brief 更新処理
	 * @details 天球の更新処理を行う。アニメーションや時間経過による変更がある場合に使用。
	 */
	void Update();

	/**
	 * @brief 描画処理
	 * @param camera カメラ
	 * @details カメラ情報を使用して天球の描画を行う。
	 */
	void Draw(const AobaraEngine::Camera& camera);

	/**
	 * @brief ライトの有効化/無効化
	 * @param isLight ライトを有効にするかどうか
	 * @details 天球にライトを適用するかどうかを設定する。
	 */
	void SetLight(bool isLight)
	{
		skydomeModel_->SetLight(isLight);
	}

private:
	std::unique_ptr<AobaraEngine::Model> skydomeModel_ = nullptr;
};