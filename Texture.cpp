/*==============================================================================

   Management Texture [Texture.cpp]

==============================================================================*/
#include "Texture.h"
#include "direct3d.h"
#include <string>
#include "DirectXTex.h"
using namespace DirectX;
#include <sstream>
#include <windows.h>

// Max Manage Texture Count
static constexpr int TEXTURE_MAX = 256;

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

struct Texture
{
	std::wstring filename;
	ID3D11ShaderResourceView* pTexture;
	unsigned int width;
	unsigned int height;
};

static Texture g_Textures[TEXTURE_MAX]{};
static int g_SetTextureIndex = -1;


// Reset Management Texture
void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (Texture& t : g_Textures)
	{
		t.pTexture = nullptr;
	}

	g_SetTextureIndex = -1;

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Texture_Finalize(void)
{
	Texture_ALLRelease();
}

int Texture_Load(const wchar_t* FileName)
{
	// If Now Reading Texture is Read Already
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// Filename Checking
		if (g_Textures[i].filename == FileName)
		{
			return i;
		}
	}

	// Find Empty Space
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture) continue; // If Using >> Continue

		// Read Texture
		TexMetadata metadata;
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(FileName, WIC_FLAGS_NONE, &metadata, image);

		// If Can`t Read File
		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"Failed read texture", FileName, MB_OK | MB_ICONERROR);
			return -1;
		}
		
#if defined(DEBUG) || defined(_DEBUG)
		// MessageBoxA(nullptr, "Texture Debug Point Reached", "Info", MB_OK);

		std::stringstream ss;
		ss << "--- Texture Load Debug ---\n";
		ss << "Filename: (pointer) " << FileName << "\n";
		ss << "Image Count: " << image.GetImageCount() << "\n";
		ss << "Image Pointer: " << image.GetImages() << "\n";
		ss << "Format: " << metadata.format << "\n";
		ss << "Device: " << g_pDevice << "\n";
		OutputDebugStringA(ss.str().c_str());
#endif	

		std::wstring Temp_Filename = FileName;
		unsigned int Temp_Width = (unsigned int)metadata.width;
		unsigned int Temp_Height = (unsigned int)metadata.height;

		// Get File Info
		g_Textures[i].filename = FileName;
		g_Textures[i].width = (unsigned int)metadata.width;
		g_Textures[i].height = (unsigned int)metadata.height;

		hr = CreateShaderResourceView(g_pDevice,
			image.GetImages(), image.GetImageCount(), metadata, &g_Textures[i].pTexture);

		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"Failed to create SRV", FileName, MB_OK | MB_ICONERROR);
			return -1;
		}
		
		g_Textures[i].filename = Temp_Filename;
		g_Textures[i].width = Temp_Width;
		g_Textures[i].height = Temp_Height;

		return i;
	}
	return -1;
}

void Texture_ALLRelease()
{
	for (Texture& t : g_Textures)
	{
		t.filename.clear();
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int TexID)
{
	// Safe Triger
	if (TexID < 0)
		return;

	// If Now Reading Texture is Read Already
	if (g_SetTextureIndex == TexID) return;

	g_SetTextureIndex = TexID;

	// Setting Texture
	g_pContext->PSSetShaderResources(0, 1, &g_Textures[TexID].pTexture);
}

unsigned int Texture_Width(int TexID)
{
	// Safe Triger
	if (TexID < 0) return 0;

	return g_Textures[TexID].width;
}

unsigned int Texture_Height(int TexID)
{
	// Safe Triger
	if (TexID < 0) return 0;

	return g_Textures[TexID].height;
}
