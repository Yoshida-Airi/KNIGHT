/**
*	@file WorldTransform.h
*	@brief ワールドトランスフォームクラスヘッダ
*/


#pragma once
#include"MathUtilty.h"
#include"DirectXCommon.h"
#include"MatrixMath.h"

struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
};

struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap_ = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	DirectXCommon* dxCommon_;

	~WorldTransform();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void UpdateWorldMatrix();
};