#include <DxLib.h>
#include <random>
#include <stdio.h>
#include "Common/Geometry.h"
#include "Manager/AudioManager.h"
#include "Manager/FontManager.h"
#include "Manager/FPSManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "App.h"

App* App::instance_ = nullptr;

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
	AudioManager::DeleteInstance();

	FontManager::GetInstance().Release();
	FontManager::DeleteInstance();

	FPSManager::GetInstance().Release();
	FPSManager::DeleteInstance();

	InputManager::GetInstance().Release();
	InputManager::DeleteInstance();

	SceneManager::GetInstance().Release();
	SceneManager::DeleteInstance();

	// DxLib の解放
	DxLib_End();

	return true;
}

void App::Quit() {
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
	// AudioManager
	AudioManager::CreateInstance();

	AudioManager::GetInstance().LoadSE("回転", "Data/Sound/rollrock.wav");
	AudioManager::GetInstance().LoadSE("トラップ設置", "Data/Sound/AS_817822_ポン（デジタル・ボタン・決定・クリック）.wav");
	AudioManager::GetInstance().LoadSE("トラップ起動", "Data/Sound/AS_792784_ビン（エラー音、アラート音）.wav");
	AudioManager::GetInstance().LoadSE("強化トラップ起動", "Data/Sound/AS_459510_SF／UI／ドゥーン／電子.wav", 1.1f);
	AudioManager::GetInstance().LoadSE("キューブ消滅", "Data/Sound/AS_251915_カットイン／バシュッ／場面転換.wav");
	AudioManager::GetInstance().LoadSE("足場崩壊", "Data/Sound/AS_857401_ビル崩壊_崩れる音.wav", 1.15f);
	AudioManager::GetInstance().LoadSE("パーフェクト", "Data/Sound/AS_1239388_perfect（低音）.wav");
	AudioManager::GetInstance().LoadSE("足音", "Data/Sound/AS_369837_革靴足音：走り：リバーブ.wav");
	
	// FontManager
	FontManager::CreateInstance();
	FontManager::GetInstance().Init();

	FontManager::GetInstance().AddFont("ロゴ", "ＭＳ 明朝", 80, 6, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontManager::GetInstance().AddFont("汎用", "ＭＳ ゴシック", 48, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontManager::GetInstance().AddFont("汎用（小）", "ＭＳ ゴシック", 24, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
	FontManager::GetInstance().AddFont("汎用（大）", "ＭＳ ゴシック", 64, 5, DX_FONTTYPE_ANTIALIASING_EDGE);

	// FPSManager
	FPSManager::CreateInstance();

	// InputManager
	InputManager::CreateInstance();
	InputManager::GetInstance().Init();

	InputManager::GetInstance().AddInputMap("移動上",
		InputManager::BUTTONS::DPAD_U, InputManager::BUTTONS::LSTICK_U,
		KEY_INPUT_W, KEY_INPUT_UP);
	InputManager::GetInstance().AddInputMap("移動下",
		InputManager::BUTTONS::DPAD_D, InputManager::BUTTONS::LSTICK_D,
		KEY_INPUT_S, KEY_INPUT_DOWN);
	InputManager::GetInstance().AddInputMap("移動左",
		InputManager::BUTTONS::DPAD_L, InputManager::BUTTONS::LSTICK_L,
		KEY_INPUT_A, KEY_INPUT_LEFT);
	InputManager::GetInstance().AddInputMap("移動右",
		InputManager::BUTTONS::DPAD_R, InputManager::BUTTONS::LSTICK_R,
		KEY_INPUT_D, KEY_INPUT_RIGHT);

	InputManager::GetInstance().AddInputMap("ワナ",
		InputManager::BUTTONS::BUTTON_0, InputManager::BUTTONS::NONE,
		KEY_INPUT_J, KEY_INPUT_C);
	InputManager::GetInstance().AddInputMap("スーパーワナ",
		InputManager::BUTTONS::BUTTON_2, InputManager::BUTTONS::NONE,
		KEY_INPUT_K, KEY_INPUT_X);
	InputManager::GetInstance().AddInputMap("高速送り",
		InputManager::BUTTONS::BUTTON_3, InputManager::BUTTONS::NONE,
		KEY_INPUT_L, KEY_INPUT_Z);
	InputManager::GetInstance().AddInputMap("ポーズ",
		InputManager::BUTTONS::BUTTON_6, InputManager::BUTTONS::BUTTON_7,
		KEY_INPUT_BACK, KEY_INPUT_ESCAPE);

	InputManager::GetInstance().AddInputMap("決定",
		InputManager::BUTTONS::BUTTON_7, InputManager::BUTTONS::NONE,
		KEY_INPUT_RETURN, 0x00);
	InputManager::GetInstance().AddInputMap("戻る",
		InputManager::BUTTONS::BUTTON_1, InputManager::BUTTONS::NONE,
		KEY_INPUT_BACK, KEY_INPUT_ESCAPE);

	// SceneManager
	SceneManager::CreateInstance();
	SceneManager::GetInstance().Init();

	return true;
}

void App::Update() {
	InputManager::GetInstance().Update();

	FPSManager::GetInstance().Update(
		InputManager::GetInstance().DownKey(KEY_INPUT_INSERT));

	SceneManager::GetInstance().Update();
}

void App::Draw() {
	// 描画先の画面をクリア
	ClearDrawScreen();

	// 描画先の画面
	SetDrawScreen(DX_SCREEN_BACK);

	FPSManager::GetInstance().Draw(
		FontManager::GetInstance().GetFontData("汎用（小）").handle);

	SceneManager::GetInstance().Draw();

#ifdef _DEBUG
	Vector2 center = { 1280 / 2.0f, 960 / 2.0f };
	DrawCircleAA(center.x, center.y, 3.0f, 12, 0x00FFFFU, true);
#endif

	// 裏画面を表画面に転写
	ScreenFlip();
}
