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

#include"GameObject.h"

//レベルデータ
struct LevelData 
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
	std::vector<ObjectData>objects;
};


/**
*   @class LevelEditor
*	@brief  レベルエディタークラス
*/
class LevelEditor :public GameObject
{
public:

	~LevelEditor();
	void LoaderJsonFile(std::string filePath);

	void Initialize()override;
	void Update()override;
	void Draw(const AobaraEngine::Camera& camera)override;

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;
	

private:

	std::map<std::string, std::unique_ptr<Model>> modelsData_;
	std::vector<std::unique_ptr<WorldTransform>> objects_;
	std::unique_ptr<LevelData> levelData_;

	std::vector<Model*>models_;

};

