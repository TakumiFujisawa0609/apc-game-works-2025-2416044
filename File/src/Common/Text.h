#pragma once
#include <string>

class WideText {
public:
	std::wstring text_ = L"";
	unsigned int wait_ = 0;

	WideText() {}
	WideText(std::wstring text, unsigned int wait) : text_(text), wait_(wait) {}

	~WideText() {}

	void Update();
	std::wstring Get() const;

	static std::wstring StringToWideString(std::string);

	static std::string WideStringToString(std::wstring);
	std::string WideStringToString();

private:
	unsigned int timer_ = 0;

};
