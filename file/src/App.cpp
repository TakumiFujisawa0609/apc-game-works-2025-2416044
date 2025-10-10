#include <DxLib.h>
#include <random>
#include <stdio.h>
#include "Manager/FPSManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
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
		FPSManager::GetInstance().CheckWait();
	}
}

bool App::Release() {
	FPSManager::GetInstance().Release();

	InputManager::GetInstance().Release();

	SceneManager::GetInstance().Release();

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
	SetGraphMode(1280, 960, 32);

	// ウィンドウモード
	ChangeWindowMode(true);

	// DirectX のバージョン
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DxLib の初期化
	if (DxLib_Init() == -1) return false;

	// 離脱変数
	exit_ = false;

	// 乱数生成処理の初期化
	// 非決定的な乱数
	std::random_device rd;
	// 乱数rdをシード値とした、擬似乱数生成（メルセンヌ・ツイスター）
	std::mt19937 mt(rd());
	// 一様分布生成器（今回はint型を指定）
	std::uniform_int_distribution<int> randDist(0, 9999);
	// 乱数をSRand関数に渡す
	SRand(randDist(mt));

	return true;
}

bool App::ClassInit() {
	FPSManager::CreateInstance();

	InputManager::CreateInstance();
	InputManager::GetInstance().Init();

	SceneManager::CreateInstance();
	SceneManager::GetInstance().Init();

	return true;
}

void App::Update() {
	InputManager::GetInstance().Update();

	SceneManager::GetInstance().Update();
}

void App::Draw() {
	// 描画先の画面をクリア
	ClearDrawScreen();

	// 描画先の画面
	SetDrawScreen(DX_SCREEN_BACK);

	FPSManager::GetInstance().Draw();
	SceneManager::GetInstance().Draw();

	// 裏画面を表画面に転写
	ScreenFlip();
}
