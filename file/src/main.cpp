#include <Windows.h>
#include "App.h"

// できるだけ簡潔にコードを書くため、可能な限り #include と行数は減らすこと

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, int nCmdShow) {
	App::CreateInstance();

	if (!App::GetInstance().Init()) return -1;

	App::GetInstance().GameLoop();

	if (!App::GetInstance().Release()) return -1;
	return 0;
}