/**
*	@file TestPlayScene.h
*	@brief テストシーンクラスヘッダ
*/

#pragma once
#include"BaseScene.h"

#include"Animation.h"
#include"Model.h"
#include"Camera.h"

#include"Input.h"

#include"LevelEditor.h"

#include<vector>

#include"Object/Ground/MapChipField.h"

/**
*   @class TestPlayScene
*	@brief  テストシーンクラス
*/
class TestPlayScene:public BaseScene
{
public:
	/// @brief デストラクタ
	~TestPlayScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;


private:
	AobaraEngine::Camera* camera_;
	
	std::unique_ptr<Model> walk_ = nullptr;
	std::unique_ptr<Model> model2_ = nullptr;
	std::unique_ptr<Model> model3_ = nullptr;
	std::unique_ptr<Model> model4_ = nullptr;

	

	AnimationData walkAnimation_;

	LevelEditor* levelEditor_;


	std::vector<std::vector<Model*>>blocks_;

	MapChipField* mapChipField_;


private:

	/**
	 * @brief ブロックの生成
	 */
	void GenerateBlocks();

};

