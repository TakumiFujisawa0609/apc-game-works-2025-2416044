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
	volumeBGM_ = 0.3f;
	volumeSE_ = 0.8f;

	return true;
}

bool AudioManager::Release() {
	for (auto& bgm : bgmList_)
		DeleteSoundMem(bgm.second);
	bgmList_.clear();
	
	for (auto& se : seList_)
		DeleteSoundMem(se.second);
	seList_.clear();

	// インスタンスの削除
	delete instance_;

	return true;
}

void AudioManager::PlayBGM(const char* name, bool loop) {
	auto it = bgmList_.find(name);
	if (it != bgmList_.end()) {
		int ty = DX_PLAYTYPE_NORMAL;
		if (loop) ty = DX_PLAYTYPE_LOOP;

		PlaySoundMem((*it).second, ty);
	}
}

void AudioManager::PlaySE(const char* name, bool loop) {
	auto it = seList_.find(name);
	if (it != seList_.end()) {
		int ty = DX_PLAYTYPE_NORMAL;
		if (loop) ty = DX_PLAYTYPE_LOOP;

		PlaySoundMem((*it).second, ty);
	}
}

void AudioManager::LoadBGM(const char* name, const char* file_path) {
	if (bgmList_.find(name) != bgmList_.end()) return;

	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(handle, volumeBGM_);
	bgmList_.emplace(name, handle);
}

void AudioManager::LoadSE(const char* name, const char* file_path) {
	if (seList_.find(name) != seList_.end()) return;

	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(handle, volumeSE_);
	seList_.emplace(name, handle);
}

float AudioManager::GetVolumeBGM() const { return volumeBGM_; }

void AudioManager::SetVolumeBGM(float f) {
	volumeBGM_ = (std::min)((std::max)(f, 0.0f), 1.0f);

	for (auto& bgm : bgmList_) {
		SetVolumeSoundMem(bgm.second, volumeBGM_);
	}
}

float AudioManager::GetVolumeSE() const { return volumeSE_; }

void AudioManager::SetVolumeSE(float f) {
	volumeSE_ = (std::min)((std::max)(f, 0.0f), 1.0f);

	for (auto& se : seList_) {
		SetVolumeSoundMem(se.second, volumeSE_);
	}
}
