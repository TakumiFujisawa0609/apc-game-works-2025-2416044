#pragma once

class FPSManager;

class App {
public:
	static void CreateInstance() { if (instance_ == nullptr) instance_ = new App; }
	static App& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	bool Init();
	void GameLoop();
	bool Release();

	void Quit();

private:
	static App* instance_;

	App() {}
	~App() {}

	App(const App&) = delete;
	App& operator=(const App&) = delete;
	App(App&&) = delete;
	App& operator=(App&&) = delete;

	bool exit_;

	bool SystemInit();
	bool ClassInit();
	void Update();
	void Draw();

};
