/**
*	@file Input.h
*	@brief 入力クラスヘッダ
*/

#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include<Xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XInput.lib")

#include"WinApp.h"
#include<cassert>

namespace AobaraEngine
{
	/**
	*   @class Input
	*	@brief  入力クラス
	*/
	class Input
	{
	public:
		static Input* GetInstance();

		void Initialize();

		void Update();

		/*void Draw();*/

		/// <summary>
		/// キーの押下をチェック
		/// </summary>
		/// <param name="keyNumber">キー番号</param>
		/// <returns>押されているか</returns>
		bool PushKey(BYTE keyNumber);

		/// <summary>
		/// キーのトリガーをチェック
		/// </summary>
		/// <param name="keyNumber">キー番号</param>
		/// <returns>トリガーかどうか</returns>
		bool TriggerKey(BYTE keyNumber);

		// ジョイスティックのデッドゾーンを適用する関数
		SHORT ApplyDeadzone(SHORT value, SHORT deadzone);

		bool GetJoystickState(int32_t stickNo, XINPUT_STATE& state);

	private:

		WinApp* winApp_;

		IDirectInput8* directInput_ = nullptr;
		IDirectInputDevice8* keyboard_ = nullptr;	//キーボードデバイス
		BYTE key_[256] = {};
		BYTE keyPre_[256] = {};	//前回の全キーの状態

		// デッドゾーンの設定
		const int DEADZONE_THRESHOLD = 8000;


		static Input* instance_;

	};
}