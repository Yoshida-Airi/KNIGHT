#include "BlockLevelEditor.h"
#include<numbers>


BlockLevelEditor::~BlockLevelEditor()
{
}

void BlockLevelEditor::LoaderJsonFile(std::string filePath)
{
	const std::string fullpath = filePath;
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData_.reset(new BlockLevelData());

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string objectType = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (objectType.compare("MESH") == 0)
		{
			//要素追加
			levelData_->grounds.emplace_back(BlockLevelData::GroundData{});
			//今追加した要素の参照を得る
			BlockLevelData::GroundData& objectData = levelData_->grounds.back();

			if (object.contains("file_name"))
			{
				const std::string path = "Resources/Level/";
				//ファイル名
				objectData.filename = object["file_name"];
				objectData.filename = path + objectData.filename;
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0] * (float)std::numbers::pi / 180.0f;
			objectData.rotation.y = -(float)transform["rotation"][2] * (float)std::numbers::pi / 180.0f;
			objectData.rotation.z = -(float)transform["rotation"][1] * (float)std::numbers::pi / 180.0f;

			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			//コライダーのパラメータ読み込み
			//nlohmann::json& collider = object["collider"];

			//if (collider.contains("type"))
			//{
			//	//コライダー情報があったら取得
			//	std::string colliderType = collider["type"].get<std::string>();
			//	objectData.collisionType = colliderType;

			//	objectData.center.x = (float)collider["center"][0];
			//	objectData.center.y = (float)collider["center"][2];
			//	objectData.center.z = (float)collider["center"][1];

			//	objectData.size.x = (float)collider["size"][0];
			//	objectData.size.y = (float)collider["size"][2];
			//	objectData.size.z = (float)collider["size"][1];
			//}
		}

		//オブジェクト走査を再帰関数にまとめ、再起呼出で枝を走査する
		if (object.contains("children"))
		{

		}

	}

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		decltype(modelsData_)::iterator it = modelsData_.find(objectData.filename);
		if (it == modelsData_.end())
		{
			//Model* model = Model::Create(objectData.filename);

			modelsData_[objectData.filename] = (std::make_unique<Ground>());

		}
	}

	for (auto& objectData : levelData_->objects)
	{
		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<WorldTransform> newObject = std::make_unique<WorldTransform>();
		newObject->Initialize();
		newObject->translation_ = objectData.translation;
		newObject->rotation_ = objectData.rotation;
		newObject->scale_ = objectData.scaling;
		newObject->UpdateWorldMatrix();
		//配列に登録
		objects_.push_back(std::move(newObject));

	}


}

void BlockLevelEditor::Initialize()
{
	//メモリの確保
	models_.reserve(this->modelsData_.size());

	for (auto& pair : this->modelsData_) {
		models_.push_back(std::move(pair.second.get()));
	}

	for (Ground* ground : models_)
	{
		ground->Initialize();
	}


}


