/**
*	@file Animation.h
*	@brief アニメーションクラスヘッダ
*/


#pragma once
#include <string>
#include<map>
#include<array>
#include"VectorMath.h"
#include"MathUtilty.h"
#include<vector>
#include<optional>
#include"Transform.h"
#include"ModelData.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

//キーフレームを表現
template<typename tValue>
struct Keyframe
{
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuatanion = Keyframe<Quaternion>;

template<typename tValue>
struct AnimationCurve
{
	std::vector<Keyframe<tValue>>Keyframes;
};

struct NodeAnimation
{
	AnimationCurve<Vector3>translate;
	AnimationCurve<Quaternion>rotate;
	AnimationCurve<Vector3>scale;
};


struct AnimationData
{
	bool isValid;	//アニメーションデータが存在しているかどうか
	float duration;	//アニメーション全体の尺（秒
	std::string filename{};
	//NodeAnimationの集合。Node名でひけるように
	std::map<std::string, NodeAnimation>nodeAnimations;

};

struct Joint
{
	QuaternionTransform transform; //Transform情報
	Matrix4x4 localMatrix;
	Matrix4x4 sleletonSpaceMatrix;	//sleltonSpaceでの変換行列
	std::string name;	//名前
	std::vector<int32_t>children;	//子JointのIndexのリスト。いなければ空
	int32_t index;	//自身のIndex
	std::optional<int32_t>parent;	//親JointのIndex。いなければnull
};

struct Skeleton
{
	int32_t root;	//RootjointのIndex
	std::map < std::string, int32_t>jointmap;	//Joint名とIndexとの辞書
	std::vector<Joint>joints;	//所属しているジョイント
};

/**
*   @class Animation
*	@brief  アニメーションクラス
*/
class Animation
{
public:
	/// @brief インスタンス取得
	static Animation* GetInstance();

	/// @brief アニメーションの更新
	/// @param skelton アニメーションを適用するスケルトン
	void Update(Skeleton& skelton);

	/// @brief アニメーションファイルの読み込み
	/// @param filename アニメーションファイル名
	/// @return 読み込んだアニメーションデータ
	AnimationData LoadAnimationFile(const std::string& filename);

	/// @brief キーフレームから値を計算
	/// @param keyframes キーフレームのベクター
	/// @param time 時間（秒）
	/// @return 計算されたVector3の値
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	/// @brief キーフレームから値を計算
	/// @param keyframes キーフレームのベクター
	/// @param time 時間（秒）
	/// @return 計算されたQuaternionの値
	Quaternion CalculateValue(const std::vector<KeyframeQuatanion>& keyframes, float time);

	/// @brief スケルトンの生成
	/// @param rootNode ルートノード
	/// @return 生成されたスケルトン
	Skeleton CreateSkelton(const Node& rootNode);

	/// @brief ジョイントの生成
	/// @param node ジョイントノード
	/// @param parent 親ジョイントのインデックス（存在しない場合はnull）
	/// @param joint ジョイントのリスト
	/// @return 生成したジョイントのインデックス
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joint);
	
	/// @brief スケルトンに対してアニメーションを適用
	/// @param skelton アニメーションを適用するスケルトン
	/// @param animationData 適用するアニメーションデータ
	/// @param animationTime アニメーションの経過時間
	void ApplyAnimation(Skeleton& skelton, const AnimationData& animationData, float animationTime);

private:

	static const size_t kMaxAnimation_ = 256;	//最大モデル数
	std::array<AnimationData, kMaxAnimation_> animationDatas_;
	bool isUsedAnimation_[kMaxAnimation_] = { false };

	static Animation* instance_;

};

