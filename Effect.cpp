/*==============================================================================

   Effect Draw [Effect.h]

==============================================================================*/
#include "Effect.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "Texture.h"
#include "Sprite_Animation.h"

struct Effect
{
	XMFLOAT2 position;
	// XMFLOAT2 velocity;
	// double lifeTime;
	int Sprite_Ani_ID;
	bool isEnable;
};

static constexpr int EFFECT_MAX = 256;
static Effect Effects[EFFECT_MAX]{};
// If You Have Many Effect, Make Matrix
static int EffectTexID = -1;
static int AniPatternID = -1;

void Effect_Initialize()
{
	for (Effect& Eff : Effects)
	{
		Eff.isEnable = false;
	}

	EffectTexID = Texture_Load(L"Resource/Texture/Explosion.png");
	AniPatternID = SpriteAni_Get_Pattern_Info(EffectTexID, 16, 4, 0.01, { 256, 256 }, { 0, 0 }, false);
}

void Effect_Finalize()
{

}

void Effect_Update(double elapsed_time)
{
	for (Effect& Eff : Effects)
	{
		if (!Eff.isEnable)	continue;

		if (SpriteAni_IsStopped(Eff.Sprite_Ani_ID))
		{
			SpriteAni_DestroyPlayer(Eff.Sprite_Ani_ID);
			Eff.isEnable = false;
		}
	}
}

void Effect_Draw()
{
	for (Effect& Eff : Effects)
	{
		if (!Eff.isEnable)	continue;

		//SpriteAni_Draw(Eff.Sprite_Ani_ID, Eff.position.x, Eff.position.y, ENEMY_WIDTH, ENEMY_HEIGHT, 0.0f);
	}
}

void Effect_Create(const XMFLOAT2& position)
{
	for (Effect& Eff : Effects)
	{
		if (Eff.isEnable) continue;

		Eff.isEnable = true;
		Eff.position = position;
		Eff.Sprite_Ani_ID = SpriteAni_CreatePlayer(AniPatternID);
		break;
	}
}
