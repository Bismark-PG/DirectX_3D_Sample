/*==============================================================================

	Draw Sprite [Sprite.h]

==============================================================================*/
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);

void Sprite_Begin();

// Show All Texture
void Sprite_Draw(int Tex_ID, float dx, float dy,
				 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f }); // White Color

// Show All Texture (Can Change Size)
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh, float angle,
				 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// Cut UV
void Sprite_Draw(int Tex_ID, float dx, float dy, 
				 // Cut UV Range (Pixel Range)
				 int px, int py, int pw, int ph,  
				 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// Cut UV (Change Texture Size)
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh,
				 // Cut UV Range (Pixel Range)
			     int px, int py, int pw, int ph, 
				 const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// Rotation
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh,
			   	 float px, float py, float pw, float ph, float angle = 0,
			     const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

#endif // SPRITE_H