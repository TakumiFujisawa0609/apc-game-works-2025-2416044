#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

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

void AudioManager::LoadBGM(const char* name, const char* file_path, float vol_mult) {
	if (bgmList_.count(name) > 0) return;

	int handle = LoadSoundMem(file_path);

	bgmList_.emplace(name, DATA(handle, vol_mult));

	SetVolumeSoundMem(VolumeMultiplier(volumeBGM_ * vol_mult), handle);
}

void AudioManager::LoadSE(const char* name, const char* file_path, float vol_mult) {
	if (seList_.count(name) > 0) return;

	int handle = LoadSoundMem(file_path);

	seList_.emplace(name, DATA(handle, vol_mult));

	SetVolumeSoundMem(VolumeMultiplier(volumeSE_ * vol_mult), handle);
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
