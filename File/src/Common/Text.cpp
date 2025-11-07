#include <vector>
#include <Windows.h>
#include "Text.h"

std::wstring StringToWideString(std::string str) {
	// Windows API
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (wchars_num == 0) return L"";
	std::vector<wchar_t> wstr(wchars_num);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), wchars_num);
	return std::wstring(wstr.data());
}

std::string WideStringToString(std::wstring wstr) {
	// Windows API
	int str_len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (str_len == 0) return "";
	std::vector<char> str(str_len);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), str_len, nullptr, nullptr);
	return std::string(str.data());
}

void WideText::Update() {
	if (wait * text.size() > timer) {
		timer++;
	}
	else if (wait * text.size() <= timer) {
		timer = wait * text.size();
	}
}

std::wstring WideText::Get() const {
	std::wstring ret = {};
	std::wstring txtClone = text;

	for (unsigned int i = 0; i < timer / wait; i++) {
		if (txtClone.size() <= i) break;
		ret.push_back(txtClone[i]);
	}

	return ret;
}
