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
	volumeBGM_ = 0.3F;
	volumeSE_ = 0.8F;

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

void AudioManager::LoadBGM(const char* file_path, const char* name) {
	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(handle, volumeBGM_);
	bgmList_.emplace(name, handle);
}

void AudioManager::LoadSE(const char* file_path, const char* name) {
	int handle = LoadSoundMem(file_path);
	SetVolumeSoundMem(handle, volumeSE_);
	seList_.emplace(name, handle);
}

float AudioManager::GetVolumeBGM() const { return volumeBGM_; }

void AudioManager::SetVolumeBGM(float f) {
	volumeBGM_ = f;
	min(max(0.0F, volumeBGM_), 1.0F);

	for (auto& bgm : bgmList_) {
		SetVolumeSoundMem(bgm.second, volumeBGM_);
	}
}

float AudioManager::GetVolumeSE() const { return volumeSE_; }

void AudioManager::SetVolumeSE(float f) {
	volumeSE_ = f;
	min(max(0.0F, volumeSE_), 1.0F);

	for (auto& se : seList_) {
		SetVolumeSoundMem(se.second, volumeSE_);
	}
}
