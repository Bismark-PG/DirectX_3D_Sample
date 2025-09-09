/*==============================================================================

   Collision [Collision.h]

==============================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>

struct Circle
{
	DirectX::XMFLOAT2 Center;
	float Radius;
};

struct Box
{
	DirectX::XMFLOAT2 Center;
	float Half_Width;
	float Half_Height;
};

bool Collision_IsOverlapCircle(const Circle& a, const Circle& b);
bool Collision_IsOverlapBox(const Box& a, const Box& b);

#endif // COLLISION_H