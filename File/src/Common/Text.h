#pragma once
#include <string>

std::wstring StringToWideString(std::string);
std::string WideStringToString(std::wstring);

struct WideText {
public:
	std::wstring text = L"";
	unsigned int wait = 0;

	void Update();
	std::wstring Get() const;

private:
	unsigned int timer = 0;

};
