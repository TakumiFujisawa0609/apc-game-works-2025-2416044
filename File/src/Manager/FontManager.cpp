#include <DxLib.h>
#include "FontManager.h"

FontManager* FontManager::instance_ = nullptr;

void FontManager::CreateInstance() {
	if (instance_ == nullptr) instance_ = new FontManager;
}

FontManager& FontManager::GetInstance() {
	return *instance_;
}

bool FontManager::Init() {
	return true;
}

bool FontManager::Release() {
	for (auto& f : fontList_)
		DeleteFontToHandle(f.second.handle);

	fontList_.clear();

	return true;
}

void FontManager::AddFont(const char* id_name, const char* font_name, int size, int thick, int font_type, int char_set, int edge_size, int italic) {
	if (fontList_.find(id_name) != fontList_.end()) return;

	int h = CreateFontToHandle(font_name, size, thick, font_type, char_set, edge_size, italic);

	DATA data = { h, size };

	fontList_.emplace(id_name, data);
}

FontManager::DATA FontManager::GetFontData(const char* name) {
	auto it = fontList_.find(name);

	if (it != fontList_.end())
		return (*it).second;
	else
		return DATA();
}
