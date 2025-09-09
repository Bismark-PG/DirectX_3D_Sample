/*==============================================================================

	Manage Audio [Audio.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef AUDIO_H
#define AUDIO_H

#include <xaudio2.h>

constexpr auto BGM_MAX = 10;
constexpr auto SFX_MAX = 50;

void InitAudio();
void Un_InitAudio();

int LoadBGM(const char* FileName);
int LoadSFX(const char* FileName);
void UnloadBGM(int Index);
void UnloadSFX(int Index);

void PlayBGM(int Index, bool Loop = true);
void StopBGM(int Index);
void PlaySFX(int Index);
void StopAllSFX();

void SetBGMVolume(int volume);
void SetSFXVolume(int volume);

void UpdateBGMVolume();

#endif // AUDIO_H