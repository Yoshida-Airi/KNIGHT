/**
*	@file BaseScene.h
*	@brief シーン基底クラスヘッダ
*/

#pragma once

class SceneManager;

namespace AobaraEngine
{
	/**
	*   @class BaseScene
	*	@brief  シーン基礎クラス
	*/
	class BaseScene
	{
	public:

		virtual ~BaseScene() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;

	};
}

