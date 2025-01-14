/**
*	@file AbstractSceneFactory.h
*	@brief シーン工場クラスヘッダ
*/

#pragma once
#include"BaseScene.h"
#include<string>

namespace AobaraEngine
{
	/**
	*   @class AbstractSceneFactory
	*	@brief シーン工場（概念）クラス
	*/
	class AbstractSceneFactory
	{
	public:

		virtual ~AbstractSceneFactory() = default;

		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns>生成したシーン</returns>
		virtual AobaraEngine::BaseScene* CreateScene(const std::string& sceneName) = 0;
	};
}