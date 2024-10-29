/**
*	@file WinApp.h
*	@brief Windows初期化クラスヘッダ
*/

#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>

#include"imgui.h"

#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
*   @class WinApp
*	@brief  Windows初期化クラス
*/
class WinApp
{
public:

	~WinApp();

	void Initialize();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();

	//シングルトン
	static WinApp* GetInstance();

	/// <summary>
	/// タイトルバーの変更
	/// </summary>
	/// <param name="title">タイトルバー名</param>
	void SetWindowTitle(const std::wstring& title);

	/*=======　　　ゲッター	=======*/
	HWND GetHwnd()const { return hwnd_; };
	HINSTANCE GetHInstance()const { return wc_.hInstance; };

public:

	//クライアント領域のサイズ
	static const int32_t kCilentWidth = 1280;
	static const int32_t kCilentHeight = 720;


private:

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウを表す識別子</param>
	/// <param name="msg">メッセージ</param>
	/// <param name="wparam">ウィンドウメッセージのパラメータ</param>
	/// <param name="lparam">ウィンドウメッセージのパラメータ</param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ウィンドウの生成
	/// </summary>
	/// <returns>生成できたかどうか true : できた</returns>
	void CreateGameWindow();

	/// <summary>
	/// ウィンドウクラスの登録
	/// </summary>
	void SetupWindowClass();

private:

	HWND hwnd_ = nullptr;	//ウィンドウを表す識別子
	WNDCLASS wc_{};			//ウィンドウクラスの設定
	static WinApp* instance_;	//シングルトン

	std::wstring windowTitle_ = L"KNIGHT";	//タイトル

};

