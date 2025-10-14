#pragma once
#include <map>

class AudioManager {
public:
	static void CreateInstance();
	static AudioManager& GetInstance();

	bool Init();
	bool Release();

	void PlayBGM(const char* name, bool loop = false);
	void PlaySE(const char* name, bool loop = false);
	void LoadBGM(const char* file_path, const char* name);
	void LoadSE(const char* file_path, const char* name);

	float GetVolumeBGM() const;
	void SetVolumeBGM(float);
	float GetVolumeSE() const;
	void SetVolumeSE(float);

private:
	static AudioManager* instance_;

	std::map<const char*, int> bgmList_;
	std::map<const char*, int> seList_;

	float volumeBGM_;
	float volumeSE_;

};
