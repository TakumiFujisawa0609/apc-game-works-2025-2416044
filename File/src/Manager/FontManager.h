#pragma once
#include <map>

class FontManager {
public:
	struct DATA {
		int handle = -1;
		int size;
	};

	static void CreateInstance() { if (instance_ == nullptr) instance_ = new FontManager; }
	static FontManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	bool Init();
	bool Release();

	void AddFont(const char* id_name, const char* font_name, int size, int thick = -1, int font_type = -1, int char_set = -1, int edge_size = -1, int italic = 0);
	DATA GetFontData(const char* name);

private:
	static FontManager* instance_;

	FontManager() {}
	~FontManager() {}

	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(FontManager&&) = delete;

	std::map<const char*, DATA> fontList_;

};

