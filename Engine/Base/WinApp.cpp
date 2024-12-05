#include "WinApp.h"
using namespace AobaraEngine;

/*======================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*======================================*/

WinApp* WinApp::GetInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new WinApp;
	}
	return instance_;
}

/*デストラクタ*/
WinApp::~WinApp()
{
	CloseWindow(hwnd_);
	CoUninitialize();
}

/*初期化処理*/
void WinApp::Initialize()
{
	//システムタイマーの分解能をあげる
	timeBeginPeriod(1);
	//HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow();
}

/*メッセージの処理*/
bool WinApp::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//ウィンドウプロシージャにメッセージを送る
	}

	// 終了メッセージの場合、処理終了
	return (msg.message == WM_QUIT);
}

void WinApp::SetWindowTitle(const std::wstring& title)
{
	windowTitle_ = title;
	if (hwnd_ != nullptr)
	{
		// ウィンドウが既に生成されていれば、タイトルを更新する
		SetWindowText(hwnd_, windowTitle_.c_str());
	}
}

/*======================================*/
/* 　　　　   プライベートメソッド　　　   　 */
/*======================================*/

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//ウィンドウの生成
void WinApp::CreateGameWindow()
{
	SetupWindowClass();

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kCilentWidth,kCilentHeight };

	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,		//利用するクラス名
		windowTitle_.c_str(),	//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			//表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc_.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

void WinApp::SetupWindowClass()
{
	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);
}


//静的メンバ変数の宣言と初期化
WinApp* WinApp::instance_ = NULL;
