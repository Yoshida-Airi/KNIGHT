/**
*	@file SceneFactory.h
*	@brief シーン生成クラスヘッダ
*/

#pragma once
#include "AbstractSceneFactory.h"

/**
*   @class SceneFactory
*	@brief シーン工場クラス
*/
class SceneFactory :
	public AobaraEngine::AbstractSceneFactory
{
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーンの名前</param>
	/// <returns>生成したシーン</returns>
	AobaraEngine::BaseScene* CreateScene(const std::string& sceneName)override;
};

