/**
*	@file ImguiManager.h
*	@brief imguiクラスヘッダ
*/

#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SrvManager.h"

#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"


/**
*   @class ImGuiManager
*	@brief Imgui管理クラス
*/
class ImGuiManager
{
public:
	/// @brief シングルトンインスタンスを取得します。
	/// @return ImGuiManagerのインスタンス
	static ImGuiManager* GetInstance();

	/// @brief デストラクタ
	~ImGuiManager();

	/// @brief 初期化関数
	void Initialize();

	/// @brief 開始処理
	void Begin();

	/// @brief 終了処理
	void End();

	/// @brief 描画処理
	void Draw();

private:
	WinApp* winApp_;
	DirectXCommon* dxCommon_;
	SrvManager* srvManager_;

	static ImGuiManager* instance_;

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;

};

