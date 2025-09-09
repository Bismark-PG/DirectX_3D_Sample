/*==============================================================================

   Management Texture [Texture.h]

==============================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize(void);

// Read Texture 
// Return >> Manage Texture Number
// It work == Manage Number (i) || If can`t read (or) No More Space == -1
int Texture_Load(const wchar_t* FileName);

void Texture_ALLRelease();

void Texture_SetTexture(int TexID);
unsigned int Texture_Width(int TexID);
unsigned int Texture_Height(int TexID);

#endif // TEXTURE_H
