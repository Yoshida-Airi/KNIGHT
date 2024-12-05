#include "ModelLoader.h"
#include"MathUtilty.h"

using namespace AobaraEngine;

ModelLoader* ModelLoader::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ModelLoader;
	}
	return instance_;
}

ModelLoader::~ModelLoader()
{

}

ModelData ModelLoader::LoadModelFile(const std::string& filename)
{

	uint32_t index = 0;


	std::filesystem::path path(filename);
	std::string folderPath = path.parent_path().string();


	for (int i = 0; i < kMaxModel_; i++)
	{
		//同じモデルがあった場合
		if (model_[i].filename == filename)
		{
			return model_[i];
		}

		if (isUsedModel_[i] == false) {
			index = i;
			isUsedModel_[i] = true;
			model_.at(index).filename = filename;
			break;
		}
	}

	//indexが不正な値だった場合止める
	if (index < 0 || kMaxModel_ <= index) {
		assert(false);
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	//meshの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));
		model_.at(index).vertices.resize(mesh->mNumVertices);//最初に頂点数分のメモリを確保しておく

		//vertexの解析
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
		{
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			model_.at(index).vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
			model_.at(index).vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			model_.at(index).vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
		}
		
		//faceの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element)
			{
				uint32_t vertexIndex = face.mIndices[element];
				model_.at(index).indices.push_back(vertexIndex);
			}
		}

		//SkinCluster構築用のデータ取得
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			//Jointごとの格納領域を作る
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeigthData = model_.at(index).skinClusterData[jointName];

			//InverseBindPoseMatrixの抽出
			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffinMatrix({ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			jointWeigthData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			//Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
			{
				jointWeigthData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
			}

		}
	}

	//テクスチャの解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			model_.at(index).material.textureFilePath = folderPath + "/" + textureFilePath.C_Str();
		}
	}

	model_.at(index).rootNode = ReadNode(scene->mRootNode);

	return model_[index];
}



Node ModelLoader::ReadNode(aiNode* node)
{
	Node result;

	aiVector3D scale, translate;
	aiQuaternion rotate;

	node->mTransformation.Decompose(scale, rotate, translate);	//assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x,scale.y,scale.z };	//scaleはそのまま
	result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w };	//x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.translate = { -translate.x,translate.y,translate.z };	//x軸を反転
	result.localMatrix = MakeAffinMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	//aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	//aiLocalMatrix.Transpose();
	//result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	//result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	//result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	//result.localMatrix.m[0][3] = aiLocalMatrix[0][3];

	//result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	//result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	//result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	//result.localMatrix.m[1][3] = aiLocalMatrix[1][3];

	//result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	//result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	//result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	//result.localMatrix.m[2][3] = aiLocalMatrix[2][3];

	//result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	//result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	//result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	//result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	result.name = node->mName.C_Str();//Node名を格納
	result.children.resize(node->mNumChildren);	//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}



ModelLoader* ModelLoader::instance_ = NULL;