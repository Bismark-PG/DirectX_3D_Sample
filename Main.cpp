/*==============================================================================

   Main Game [main.cpp]

==============================================================================*/
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#include "Game_Window.h"
#include "direct3d.h"
#include "shader.h"
#include "Sprite.h"
#include "Texture.h"
#include "Sprite_Animation.h"
#include "debug_text.h"
#include <sstream>
#include "system_timer.h"
#include "KeyLogger.h"
#include "mouse.h"
#include "Game.h"

float angle{ 0.0f };
float Position_X = 0.0f;
float Position_Y = 0.0f;
float SPEED = 200;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR IpCmdline, _In_ int nCmdShow)
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED); // (void)

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HWND hWnd = GameWindowCreate(hInstance);

	// Initialize System Tools
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd);

	// Initialize Draw Tools
	Direct3D_Initialize(hWnd);
	Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	// Game Draw
	SpriteAni_Initialize();
	Game_Initialize();

	int w = Texture_Load(L"Resource/Texture/Black.png");

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"Resource/consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.f, 0.f, 0, 0, 0.f, 0.f);

	// Show Mouse (True = Show // False = Don`t Show)
	Mouse_SetVisible(true);
	// Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Set Frame
	double Exec_Last_Time = SystemTimer_GetTime();
	double Fps_Last_Time = Exec_Last_Time;
	double Current_Time = 0.0;
	ULONG Frame_Count = 0;
	double FPS = 0.0; // Frame Per Second

	// Make Game Loop & Message Loop
	MSG msg;

	do
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// Set Game
		else
		{
			// Time Set
			Current_Time = SystemTimer_GetTime(); // Get System Time
			double Elapsed_Time = Current_Time - Fps_Last_Time; // Get Time For FPS

			//  Set FPS
			if (Elapsed_Time >= 1.0)
			{
				FPS = Frame_Count / Elapsed_Time;
				Fps_Last_Time = Current_Time; // Save FPS
				Frame_Count = 0; // Clear Count
			}

			// Set Elapsed Time
			Elapsed_Time = Current_Time - Exec_Last_Time;
			// If you want to limit FPS, set condition here
			if (Elapsed_Time >= (1.0 / 60.0)) // 60 FPS
			{
				Exec_Last_Time = Current_Time; // Save Last Time
				
				// Set Key Logger With FPS
				KeyLogger_Update();

				// Update Game Texture
				Game_Update(Elapsed_Time);
				SpriteAni_Update(Elapsed_Time);

				// Draw Texture
				Direct3D_Clear();
				Sprite_Begin();

				Sprite_Draw(w, 0.f, 0.f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.f);

				Game_Draw();

				// Show FPS
#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream ss;
				ss << "FPS : " << FPS << std::endl;
				dt.SetText(ss.str().c_str(), { .5f, .8f, .3f, 1.f });

				dt.Draw();
				dt.Clear();
#endif	

				Direct3D_Present();

				Frame_Count++;
			}	
		}
	} while (msg.message != WM_QUIT);
	
	// Read End Initialize Function
	SpriteAni_Finalize();
	Sprite_Finalize();
	Texture_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();
	Mouse_Finalize();

	return (int)msg.wParam;
}