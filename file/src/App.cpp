#include <DxLib.h>
#include <stdio.h>
#include "Manager/FPSManager.h"
#include "App.h"

App* App::instance_ = nullptr;

App::App() {}

App::~App() {}

void App::CreateInstance() {
	// instance_ 変数が空ポインタならインスタンスを生成
	if (instance_ == nullptr) instance_ = new App;
}

App& App::GetInstance() {
	return *instance_;
}

bool App::Init() {
	// システム初期化
	if (SystemInit() == false) return false;

	// クラス初期化
	if (ClassInit() == false) return false;

	return true;
}

void App::GameLoop() {
	while (!ProcessMessage() && !exit_) {
		// 更新処理
		Update();

		// 描画処理
		Draw();
		
		//
		FPSManager::GetInstance().Wait();
	}
}

bool App::Release() {
	FPSManager::GetInstance().Release();

	// DxLib の解放
	if (DxLib_End() == -1) return false;

	// インスタンスの削除
	delete instance_;
	return true;
}

void App::Exit() {
	exit_ = true;
}

bool App::SystemInit() {
	// ウインドウの名称
	SetWindowText("知性の立方体");

	// 画面設定
	SetGraphMode(640, 480, 32);

	// ウィンドウモード
	ChangeWindowMode(true);

	// DirectX のバージョン
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DxLib の初期化
	if (DxLib_Init() == -1) return false;

	exit_ = false;

	return true;
}

bool App::ClassInit() {
	FPSManager::CreateInstance();

	return true;
}

void App::Update() {

}

void App::Draw() {
	// 描画先の画面
	SetDrawScreen(DX_SCREEN_BACK);

	// 描画先の画面をクリア
	ClearDrawScreen();

	FPSManager::GetInstance().Draw();

	if (!FPSManager::GetInstance().IsSkipDraw()) {

	}

	// 裏画面を表画面に転写
	ScreenFlip();
}
