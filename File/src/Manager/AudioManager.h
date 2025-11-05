#pragma once
#include <map>

class AudioManager {
public:
	struct DATA {
		int handle;
		float volMult = 1.0f;
	};

	static void CreateInstance() { if (instance_ == nullptr) instance_ = new AudioManager; }
	static AudioManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	bool Init();
	bool Release();

	void PlayBGM(const char* name, bool loop = false);
	void PlaySE(const char* name, bool loop = false);
	void LoadBGM(const char* name, const char* file_path, float vol_mult = 1.0f);
	void LoadSE(const char* name, const char* file_path, float vol_mult = 1.0f);

	float GetVolumeBGM() const;
	void SetVolumeBGM(float);
	float GetVolumeSE() const;
	void SetVolumeSE(float);

private:
	static AudioManager* instance_;

	AudioManager() {}
	~AudioManager() {}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

	std::map<const char*, DATA> bgmList_;
	std::map<const char*, DATA> seList_;

	float volumeBGM_;
	float volumeSE_;

	int VolumeMultiplier(float);

};
