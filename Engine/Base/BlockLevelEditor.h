#pragma once
/**
*	@file LevelEditor.h
*	@brief レベルエディタークラスヘッダ
*/

#pragma once
#include"json.hpp"
#include<fstream>
#include<assert.h>
#include"VectorMath.h"
#include"Model.h"
#include<vector>
#include"WorldTransform.h"
#include"Camera.h"

#include"Object/Ground/Ground.h"

//前方宣言
class CollisionManager;



//レベルデータ
struct BlockLevelData 
{
	//オブジェクト1個分のデータ
	struct ObjectData
	{
		std::string filename;	//ファイル名
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		// colliderの情報
		std::string collisionType;	//コライダーのタイプ
		Vector3 center;  // コライダーの中心
		Vector3 size;    // コライダーのサイズ

	};

	struct GroundData
	{
		//ファイル名
		std::string filename;
		//平行移動
		Vector3 translation;
		//回転角
		Vector3 rotation;
		//スケール
		Vector3 scaling;
	};

	std::vector<GroundData> grounds;
	std::vector<ObjectData>objects;
};

/**
*   @class LevelEditor
*	@brief  レベルエディタークラス
*/
class BlockLevelEditor 
{
public:

	~BlockLevelEditor();
	void LoaderJsonFile(std::string filePath);

	void Initialize();
	void Update();
	void Draw(Camera* camera);

	std::vector<BlockLevelData::GroundData> GetGrounds() {
		return levelData_.get()->grounds;
	}

	//Vector3 GetWorldPosition();
	//AABB GetAABB();
	//void OnCollision([[maybe_unused]] Collider* other);
	//


private:

	std::map<std::string, std::unique_ptr<Ground>> modelsData_;
	std::vector<std::unique_ptr<WorldTransform>> objects_;
	std::unique_ptr<BlockLevelData> levelData_;

	std::vector<Ground*>models_;

};

