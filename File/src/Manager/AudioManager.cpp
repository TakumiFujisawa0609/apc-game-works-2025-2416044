#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager::AudioManager()
	:
	volumeBGM_(0.6f),
	volumeSE_(0.8f)
{
}

AudioManager::~AudioManager()
{
	for (auto& bgm : bgmList_) DeleteSoundMem(bgm.second.handle);
	bgmList_.clear();

	for (auto& se : seList_) DeleteSoundMem(se.second.handle);
	seList_.clear();
}

void AudioManager::PlayBGM(const char* name, bool loop)
{
	auto it = bgmList_.find(name);

	if (it != bgmList_.end())
	{
		if (!loop || !CheckSoundMem((*it).second.handle)) {
			int ty = DX_PLAYTYPE_BACK;
			if (loop) ty = DX_PLAYTYPE_LOOP;

			PlaySoundMem((*it).second.handle, ty);
		}
	}
}

void AudioManager::PlaySE(const char* name, bool loop)
{
	auto it = seList_.find(name);

	if (it != seList_.end())
	{
		if (!loop || !CheckSoundMem((*it).second.handle)) {
			int ty = DX_PLAYTYPE_BACK;
			if (loop) ty = DX_PLAYTYPE_LOOP;

			PlaySoundMem((*it).second.handle, ty);
		}
	}
}

void AudioManager::StopBGM(const char* name)
{
	auto it = bgmList_.find(name);

	if (it != bgmList_.end())
	{
		StopSoundMem((*it).second.handle);
	}
}

void AudioManager::StopSE(const char* name)
{
	auto it = seList_.find(name);

	if (it != seList_.end())
	{
		StopSoundMem((*it).second.handle);
	}
}

void AudioManager::LoadBGM(const char* name, const char* file_path, float vol_mult)
{
	if (bgmList_.count(name) > 0) return;

	int handle = LoadSoundMem(file_path);

	if (handle == -1) return;

	bgmList_.emplace(name, AUDIO_DATA(handle, vol_mult));

	SetVolumeSoundMem(VolumeMultiple(volumeBGM_ * vol_mult), handle);
}

void AudioManager::LoadSE(const char* name, const char* file_path, float vol_mult)
{
	if (seList_.count(name) > 0) return;

	int handle = LoadSoundMem(file_path);

	if (handle == -1) return;

	seList_.emplace(name, AUDIO_DATA(handle, vol_mult));

	SetVolumeSoundMem(VolumeMultiple(volumeSE_ * vol_mult), handle);
}

float AudioManager::GetVolumeBGM() const
{
	return volumeBGM_;
}

void AudioManager::SetVolumeBGM(float f)
{
	volumeBGM_ = std::clamp(f, 0.0f, 1.0f);

	for (auto& bgm : bgmList_)
	{
		SetVolumeSoundMem(VolumeMultiple(volumeBGM_ * bgm.second.volMult), bgm.second.handle);
	}
}

float AudioManager::GetVolumeSE() const
{
	return volumeSE_;
}

void AudioManager::SetVolumeSE(float f)
{
	volumeSE_ = std::clamp(f, 0.0f, 1.0f);

	for (auto& se : seList_)
	{
		SetVolumeSoundMem(VolumeMultiple(volumeSE_ * se.second.volMult), se.second.handle);
	}
}

int AudioManager::VolumeMultiple(float f)
{
	return int(f * VOLUME_MULTIPLIER);
}
