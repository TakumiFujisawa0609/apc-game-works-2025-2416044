#pragma once
#include <map>

class AudioManager {
private:
	static AudioManager* instance_;

	AudioManager();
	~AudioManager();

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

public:
	static void CreateInstance() { if (instance_ == nullptr) instance_ = new AudioManager; }
	static AudioManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	struct AUDIO_DATA {
		int handle;
		float volMult = 1.0f;
	};

	void PlayBGM(const char* name, bool loop = false);
	void PlaySE(const char* name, bool loop = false);
	void StopBGM(const char* name);
	void StopSE(const char* name);

	void LoadBGM(const char* name, const char* file_path, float vol_mult = 1.0f);
	void LoadSE(const char* name, const char* file_path, float vol_mult = 1.0f);

	float GetVolumeBGM() const;
	void SetVolumeBGM(float);
	float GetVolumeSE() const;
	void SetVolumeSE(float);

private:
	static constexpr float VOLUME_MULTIPLIER = 10000.0f;

	std::map<const char*, AUDIO_DATA> bgmList_;
	std::map<const char*, AUDIO_DATA> seList_;

	float volumeBGM_;
	float volumeSE_;

	int VolumeMultiple(float);

};
