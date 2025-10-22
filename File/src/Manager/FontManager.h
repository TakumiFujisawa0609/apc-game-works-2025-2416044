#pragma once
#include <map>

class FontManager {
public:
	struct DATA {
		int handle = -1;
		int size;
	};

	static void CreateInstance();
	static FontManager& GetInstance();

	bool Init();
	bool Release();

	void AddFont(const char* id_name, const char* font_name, int size, int thick = -1, int font_type = -1, int char_set = -1, int edge_size = -1, int italic = 0);
	DATA GetFontData(const char* name);

private:
	static FontManager* instance_;

	std::map<const char*, DATA> fontList_;

};

