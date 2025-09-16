#include <DxLib.h>
#include <stdio.h>
#include "Manager/FPSManager.h"
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
		FPSManager::GetInstance().Wait();
	}
}

bool App::Release() {
	FPSManager::GetInstance().Release();

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
	SetGraphMode(640, 480, 32);

	// ウィンドウモード
	ChangeWindowMode(true);

	// DirectX のバージョン
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DxLib の初期化
	if (DxLib_Init() == -1) return false;

	// 3D 描画機能の有効化
	SetLightEnable(true);
	SetUseLighting(true);
	SetUseBackCulling(true);
	SetUseZBuffer3D(true);
	SetBackgroundColor(0x10, 0x00, 0x20);

	SetLightAmbColor(GetColorF(0.5F, 0.5F, 0.5F, 1.F));
	SetGlobalAmbientLight(GetColorF(0.5F, 0.5F, 0.5F, 1.F));

	MATERIALPARAM material = {};
	material.Ambient = { 0.2F, 0.2F, 0.2F, 1.F };
	DxLib::SetMaterialParam(material);

	exit_ = false;

	return true;
}

bool App::ClassInit() {
	FPSManager::CreateInstance();

	SceneManager::CreateInstance();
	SceneManager::GetInstance().Init();

	return true;
}

void App::Update() {
	SceneManager::GetInstance().Update();
}

void App::Draw() {
	if (!FPSManager::GetInstance().IsSkipDraw()) {
		// 描画先の画面をクリア
		ClearDrawScreen();

		// 描画先の画面
		SetDrawScreen(DX_SCREEN_BACK);

		SetCameraNearFar(1.F, 500.F);
		SetCameraPositionAndAngle({ 20.F, 50.F, -160.F }, DX_PI_F / 90.F * 15.F, 0.F, 0.F);

		FPSManager::GetInstance().Draw();
		SceneManager::GetInstance().Draw();
		DrawSphere3D({ 0.F, 0.F, 0.F }, 10.F, 16, 0xFFFFFF, 0xFFFFFF, true);

		// 裏画面を表画面に転写
		ScreenFlip();
	}
}
