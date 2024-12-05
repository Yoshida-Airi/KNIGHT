#include "Animation.h"
using namespace AobaraEngine;

/**
*	@file Sphere.h
*	@brief 球体クラスヘッダ
*/


#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"MathUtilty.h"
#include"VectorMath.h"
#include"Transform.h"
#include"TextureManager.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"

#include<numbers>
#include<cmath>
#include<assert.h>

#include"Transform.h"

/**
*   @class Sphere
*	@brief  球体クラス
*/

namespace AobaraEngine
{
	class Sphere
	{
	public:

		~Sphere();///< デストラクタ

		/// @brief 初期化処理
		/// @param textureHandle テクスチャのハンドル
		void Initialize(uint32_t textureHandle);

		/// @brief 更新処理
		void Update();

		/// @brief 描画処理
		/// @param camera カメラオブジェクト
		void Draw(AobaraEngine::Camera* camera);



		/// @brief 表示設定
	/// @param isInvisible true の場合、球体を非表示にする
		void SetisInvisible(bool isInvisible)
		{
			isInvisible_ = isInvisible;
		}

		/// @brief ワールドトランスフォームを取得
		/// @return ワールドトランスフォームへのポインタ
		WorldTransform* GetWorldTransform()const { return worldTransform_; };

		/// @brief 球体の生成
		/// @param textureHandle テクスチャ番号
		/// @return 生成した球体のポインタ
		static Sphere* Create(uint32_t textureHandle);

	private:

		DirectXCommon* dxCommon_;
		GraphicsPipelineManager* psoManager_;
		TextureManager* texture_;

		Microsoft::WRL::ComPtr < ID3D12Resource>vertexResource_;	//頂点リソース
		Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;	//マテリアルリソース
		Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

		VertexData* vertexData_ = nullptr;
		Material* materialData_ = nullptr;	//マテリアルデータ
		DirectionalLight* lightData_ = nullptr;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
		D3D12_VERTEX_BUFFER_VIEW materialBufferView_;
		D3D12_VERTEX_BUFFER_VIEW wvpBufferView_;

		uint32_t textureHandle_;

		WorldTransform* worldTransform_ = {};

		//分割数
		const uint32_t kSubdivision_ = 16;

		//経度分割1つ分の角度
		const float kLonEvery_ = static_cast<float>(std::numbers::pi) * 2.0f / float(kSubdivision_);
		//緯度分割1つ分の角度
		const float kLatEvery_ = static_cast<float>(std::numbers::pi) / float(kSubdivision_);

		//uint32_t latIndex = 0;
		//uint32_t lonIndex = 0;

		//球の全ての頂点
		const uint32_t totalVertex_ = 1536;

		bool isInvisible_ = false;

	private:
		/// @brief 頂点バッファの取得
		void VertexBuffer();

		/// @brief マテリアルバッファの取得
		void MaterialBuffer();

		/// @brief ライトバッファの取得
		void LightBuffer();
	};

}

Animation* Animation::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Animation;
	}
	return instance_;
}

void Animation::Update(Skeleton& skelton)
{
	//すべてのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skelton.joints)
	{
		joint.localMatrix = MakeAffinMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent)
		{
			//親がいれば親の行列を掛ける
			joint.sleletonSpaceMatrix = Multiply(joint.localMatrix, skelton.joints[*joint.parent].sleletonSpaceMatrix);
		}
		else
		{
			//親がいないのでlocalMatrixとskeltonSpaceMatrixは一致する
			joint.sleletonSpaceMatrix = joint.localMatrix;
		}
	}
}


//アニメーションファイル読み込み
AnimationData Animation::LoadAnimationFile(const std::string& filename)
{
	uint32_t index = 0;
	//AnimationData animation;	//今回作るアニメーション

	for (int i = 0; i < kMaxAnimation_; i++)
	{
		//同じモデルがあった場合
		if (animationDatas_[i].filename == filename)
		{
			return animationDatas_[i];
		}

		if (isUsedAnimation_[i] == false) {
			index = i;
			isUsedAnimation_[i] = true;
			animationDatas_.at(index).filename = filename;
			break;
		}
	}


	Assimp::Importer importer;
	std::string filePath = filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	if (scene->mNumAnimations == 0)//アニメーションがない
	{
		animationDatas_.at(index).isValid = false;
		return animationDatas_[index];
	}
	else
	{
		animationDatas_.at(index).isValid = true;
	}

	aiAnimation* animationAssimp = scene->mAnimations[0];	//最初のアニメーションだけ採用
	animationDatas_.at(index).duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	//時間の単位を病に変換


	//NodeAnimationを解析
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex)
	{
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animationDatas_.at(index).nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		//translate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };	//右手から左手
			nodeAnimation.translate.Keyframes.push_back(keyframe);
		}


		//rotate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex)
		{
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuatanion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
			nodeAnimation.rotate.Keyframes.push_back(keyframe);
		}

		//scale
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };	//右手から左手
			nodeAnimation.scale.Keyframes.push_back(keyframe);
		}


	}

	return animationDatas_[index];
}


//3次元ベクトルの線形補完
Vector3 Animation::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	assert(!keyframes.empty());	//キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time)
	{
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index)
	{
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
		{
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.begin()).value;
}

//クォータニオンの線形補間
Quaternion Animation::CalculateValue(const std::vector<KeyframeQuatanion>& keyframes, float time)
{
	assert(!keyframes.empty());	//キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time)
	{
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index)
	{
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
		{
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.begin()).value;
}

Skeleton Animation::CreateSkelton(const Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints)
	{
		skeleton.jointmap.emplace(joint.name, joint.index);
	}


	return skeleton;
}

int32_t Animation::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.sleletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());	//現在登録されている数をindexに
	joint.parent = parent;
	joints.push_back(joint);	//skeletonのJoint列に追加
	
	for (const Node& child : node.children)
	{
		//子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	//自身のIndexを返す
	return joint.index;

}

void Animation::ApplyAnimation(Skeleton& skelton, const AnimationData& animationData, float animationTime)
{
	for (Joint& joint : skelton.joints)
	{
		//対象のJointのAnimationがあれば、値の運用を行う。
		if (auto it = animationData.nodeAnimations.find(joint.name); it != animationData.nodeAnimations.end())
		{
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.Keyframes, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.Keyframes, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.Keyframes, animationTime);
		}
	}
}

Animation* Animation::instance_ = NULL;