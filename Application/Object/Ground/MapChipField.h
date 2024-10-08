#pragma once
#include <stdint.h>
#include<vector>
#include<string>
#include<map>

#include<fstream>
#include<sstream>

#include<cassert>

#include<MathUtilty.h>

enum class MapChipType
{
	kBlank,
	kBlock,
};

struct MapChipData
{
	std::vector<std::vector<MapChipType>>data;
};

namespace
{
	std::map<std::string, MapChipType>mapChipTable =
	{
		{"0",MapChipType::kBlank},
		{"1",MapChipType::kBlock},
	};
}


class MapChipField
{
public:
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ種別の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVertical();
	uint32_t GetNumBlockHorizontal();

private:

	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

private:
	void ResetMapChipData();

};
