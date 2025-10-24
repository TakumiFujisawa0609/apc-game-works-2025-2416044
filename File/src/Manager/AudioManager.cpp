#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

void AudioManager::CreateInstance() {
	if (instance_ == nullptr) instance_ = new AudioManager;
}

AudioManager& AudioManager::GetInstance() {
	return *instance_;
}

bool AudioManager::Init() {
	volumeBGM_ = 0.6f;
	volumeSE_ = 0.8f;

	return true;
}

bool AudioManager::Release() {
	for (auto& bgm : bgmList_)
		DeleteSoundMem(bgm.second.handle);
	bgmList_.clear();
	
	for (auto& se : seList_)
		DeleteSoundMem(se.second.handle);
	seList_.clear();

	// インスタンスの削除
	delete instance_;

	return true;
}

void AudioManager::PlayBGM(const char* name, bool loop) {
	auto it = bgmList_.find(name);
	if (it != bgmList_.end()) {
		int ty = DX_PLAYTYPE_BACK;
		if (loop) ty = DX_PLAYTYPE_LOOP;

		PlaySoundMem((*it).second.handle, ty);
	}
}

void AudioManager::PlaySE(const char* name, bool loop) {
	auto it = seList_.find(name);
	if (it != seList_.end()) {
		int ty = DX_PLAYTYPE_BACK;
		if (loop) ty = DX_PLAYTYPE_LOOP;

		PlaySoundMem((*it).second.handle, ty);
	}
}

void AudioManager::LoadBGM(const char* name, const char* file_path, float volume_mult) {
	if (bgmList_.find(name) != bgmList_.end()) return;

	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(VolumeMultiplier(volumeBGM_ * volume_mult), handle);
	bgmList_.emplace(name, DATA(handle, volume_mult));
}

void AudioManager::LoadSE(const char* name, const char* file_path, float volume_mult) {
	if (seList_.find(name) != seList_.end()) return;

	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(VolumeMultiplier(volumeSE_ * volume_mult), handle);
	seList_.emplace(name, DATA(handle, volume_mult));
}

float AudioManager::GetVolumeBGM() const { return volumeBGM_; }

void AudioManager::SetVolumeBGM(float f) {
	volumeBGM_ = (std::min)((std::max)(f, 0.0f), 1.0f);

	for (auto& bgm : bgmList_) {
		SetVolumeSoundMem(VolumeMultiplier(volumeBGM_ * bgm.second.volMult), bgm.second.handle);
	}
}

float AudioManager::GetVolumeSE() const { return volumeSE_; }

void AudioManager::SetVolumeSE(float f) {
	volumeSE_ = (std::min)((std::max)(f, 0.0f), 1.0f);

	for (auto& se : seList_) {
		SetVolumeSoundMem(VolumeMultiplier(volumeSE_ * se.second.volMult), se.second.handle);
	}
}

int AudioManager::VolumeMultiplier(float f) {
	return int(f * 10000);
}
