#pragma once

class FPSManager;

class App {
public:
	static void CreateInstance();
	static App& GetInstance();

	bool Init();
	void GameLoop();
	bool Release();

	void Quit();

private:
	static App* instance_;

	bool exit_;

	App();
	~App();

	bool SystemInit();
	bool ClassInit();
	void Update();
	void Draw();

};
