/**
*	@file GamePlayScene.h
*	@brief ゲームプレイシーンクラスヘッダ
*/

#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ParticleSystem.h"
#include"Camera.h"

#include"CollisionManager.h"

#include"Input.h"

#include"Object/Player/Player.h"
#include"Object/Enemy/Enemy.h"
#include"Object/Enemy/FlyEnemy.h"
#include"Object/Skydome/Skydome.h"
#include"Object/CameraController/CameraController.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"
#include"Object/Goal/Goal.h"
#include"Effect/DeathEffect.h"
#include"Effect/Fade.h"
#include"Object/Ground/MapChipField.h"


#include"BlockLevelEditor.h"

/**
*   @class GamePlayScene
*	@brief  ゲームプレイシーンクラス
*/
class GamePlayScene:public AobaraEngine::BaseScene
{
public:
	/// @brief デストラクタ
	~GamePlayScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

	/// @brief 全てのオブジェクトの当たり判定のチェック
	void CheckAllCollisions();

	/**
	* @brief 敵をスポーンします
	* @param[in] position 敵の位置
	*/
	void SpawnEnemy(const Vector3& position);

	/**
	* @brief 空飛ぶ敵をスポーンします
	* @param[in] position 敵の位置
	*/
	void SpawnFlyEnemy(const Vector3& position);

	/**
	* @brief ブロックをスポーンします
	* @param[in] position ブロックの位置
	* @param[in] scale ブロックのスケール
	*/
	void SpawnBlock(const Vector3& position, const Vector3& scale);

	/**
	* @brief 敵の消滅エフェクトを作成します
	* @param[in] position エフェクトの位置
	*/
	void CreateDeathEffect(Vector3 position);

	enum class Phase
	{
		kPlay,
		kClear,
		kDeath,
	};


	/**
	* @brief フェーズを変更します
	* @param[in] phase 変更するフェーズ
	*/
	void ChangePhase(Phase phase);


private:
	TextureManager* texture_;
	SceneManager* sceneManager_ = nullptr;

	//当たり判定処理
	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;

	uint32_t configTexture_;
	uint32_t HPTexture_;
	

	std::unique_ptr< AobaraEngine::Camera> camera_;
	Input* input_;


	std::unique_ptr<Fade> fade_ = nullptr;

	BlockLevelEditor* blockLevelEditor = nullptr;

	std::unique_ptr <Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<FlyEnemy>> flyEnemys_;
	std::unique_ptr <Skydome>skydome_;
	std::unique_ptr <Goal>goal_;
	std::unique_ptr<CameraController> cameraController_;
	std::unique_ptr<Weapon>weapon_;
	std::vector<Ground*>grounds_;
	std::list<std::unique_ptr<DeathEffect>>deathEffect_;	//敵消滅エフェクト
	//　std::unique_ptr<DeathEffect> playerDeathEffect = nullptr;

	std::unique_ptr<AobaraEngine::Sprite> config_ = nullptr;
	std::array<std::unique_ptr<AobaraEngine::Sprite>, 5> hpTextures_;


	Phase phase_;

	//std::vector<std::vector<std::unique_ptr<Model>>>blocks_;
	//std::unique_ptr<MapChipField> mapChipField_;

	bool configInstruction = true;
	bool configDraw = false;

	std::chrono::steady_clock::time_point lastInputTime;

private:

	/// @brief ゲームプレイフェーズ
	void GamePlayPhase();
	/// @brief ゲームクリアフェーズ
	void GameClearPhase();
	/// @brief ゲームオーバーフェーズ
	void GameOverPhase();
	/// @brief ブロック生成
	//void GenerateBlocks();

	void ConfigDraw();
};

