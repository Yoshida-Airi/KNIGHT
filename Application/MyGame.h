/**
*	@file MyGame.h
*	@brief ゲームクラスヘッダ
*/

#pragma once
#include"Framework.h"

#include"BaseScene.h"

/**
*   @class MyGame
*	@brief  固有クラス
*/
class MyGame :public Framework
{
public:
	/// @brief デストラクタ
	~MyGame();
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

private:
	std::unique_ptr< AobaraEngine::BaseScene> scene_;

};

