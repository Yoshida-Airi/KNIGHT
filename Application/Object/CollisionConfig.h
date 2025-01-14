/**
*	@file CollisionConfig.h
*	@brief 当たり判定用列挙型クラスヘッダ
*/

#pragma once

#include<cstdint>

/**
 * @enum CollisionTypeDef
 * @brief 各オブジェクトの当たり判定カテゴリを定義する列挙型
 * @details 各ゲームオブジェクトに割り当てる当たり判定の種類を定義します。
 */
enum class CollisionTypeDef : uint32_t
{
	kDefault,  ///< デフォルトの当たり判定
	kPlayer,   ///< プレイヤーの当たり判定
	kWeapon,   ///< 武器の当たり判定
	kEnemy,    ///< 敵の当たり判定
	kEnemyBullet,    ///< 敵の当たり判定
	kMap,      ///< マップの当たり判定
	kGoal,     ///< ゴールの当たり判定
};

/**
 * @enum ColliderType
 * @brief 当たり判定の形状を定義する列挙型
 * @details 衝突判定の際に使用する形状（SPHEREかAABB）を指定します。
 */
enum class ColliderType : uint32_t
{
	SPHERE,  ///< 球形の当たり判定
	AABB,    ///< 軸に沿った境界ボックス (Axis-Aligned Bounding Box) の当たり判定
};