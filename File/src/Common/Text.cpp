#include <cassert>
#include <vector>
#include <Windows.h>
#include "Text.h"

void WideText::Update() {
	if (wait_ * text_.size() > timer_) {
		timer_++;
	}
	else if (wait_ * text_.size() <= timer_) {
		timer_ = wait_ * (unsigned int)text_.size();
	}
}

std::wstring WideText::Get() const {
	std::wstring ret = {};
	std::wstring txtClone = text_;

	for (unsigned int i = 0; i < timer_ / wait_; i++) {
		if (txtClone.size() <= i) break;
		ret.push_back(txtClone[i]);
	}

	return ret;
}

std::wstring WideText::StringToWideString(std::string str) {
	std::wstring ret;

	int result = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);

	assert(result >= 0);

	ret.resize(result);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), ret.data(), ret.size());

	return ret;
}

std::string WideText::WideStringToString(std::wstring wstr) {
	std::string ret;

	int result = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), nullptr, 0, nullptr, nullptr);
	
	assert(result >= 0);

	ret.resize(result);
	
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), ret.data(), ret.size(), nullptr, nullptr);
	
	return ret;
}

std::string WideText::WideStringToString() {
	std::string ret;

	int result = WideCharToMultiByte(CP_ACP, 0, text_.c_str(), text_.length(), nullptr, 0, nullptr, nullptr);

	assert(result >= 0);

	ret.resize(result);

	WideCharToMultiByte(CP_ACP, 0, text_.c_str(), text_.length(), ret.data(), ret.size(), nullptr, nullptr);

	return ret;
}
