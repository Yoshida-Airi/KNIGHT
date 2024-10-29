/**
*	@file GlobalVariables.h
*	@brief グローバル変数クラスヘッダ
*/

#pragma once
#include<variant>
#include<map>
#include<string>
#include <fstream>
#include"json.hpp"

#include"VectorMath.h"


/**
*   @class GlobalVariables
*	@brief  グローバル変数クラス
*/
class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	struct Item
	{
		std::variant<int32_t, float, Vector3>value;
	};

	struct Group
	{
		std::map<std::string, Item>items;
	};

	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);

	//項目の追加
	//int
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	//float
	void AddItem(const std::string& groupName, const std::string& key, float value);
	//Vector3
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	//値のセット
	//int
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//float
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//Vector3
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	//値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key)const;
	float GetFloatValue(const std::string& groupName, const std::string& key)const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key)const;

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;

private:
	std::map<std::string, Group>datas_;

	//グローバル変数の保存先ファイル
	const std::string kDirectoryPath_ = "Resources/GlobalVariables/";

	//静的メンバ変数の宣言と初期化
	static GlobalVariables* instance_;
};

