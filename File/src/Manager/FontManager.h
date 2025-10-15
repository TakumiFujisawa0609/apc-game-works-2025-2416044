#pragma once
#include <map>

class FontManager {
public:
	static void CreateInstance();
	static FontManager& GetInstance();

	bool Init();
	bool Release();

	void AddFont(const char* id_name, const char* font_name, int size, int thick = -1, int font_type = -1, int char_set = -1, int edge_size = -1, int italic = 0);
	int GetFontHandle(const char* name);

private:
	static FontManager* instance_;

	std::map<const char*, int> fontList_;

};

