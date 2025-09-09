/*==============================================================================

   Make Animation [Sprite_Animation.h]

==============================================================================*/
#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "DirectXMath.h"
using namespace DirectX;

void SpriteAni_Initialize();
void SpriteAni_Finalize();

void SpriteAni_Update(double elapsed_time);
void SpriteAni_Draw(int PlayID, float dx, float dy, float dw, float dh, float angle);

int SpriteAni_Get_Pattern_Info(int TexID, int PMax, int HPatternMax,
	double PatternPlayTime, const XMUINT2& PSize, const XMUINT2& SPosition, bool Is_Loop);

int SpriteAni_CreatePlayer(int AniPatternID);

bool SpriteAni_IsStopped(int Index);
void SpriteAni_DestroyPlayer(int Index);

#endif // SPRITE_ANIMATION_H
