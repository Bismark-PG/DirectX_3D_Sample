/*==============================================================================

   Make Window [Game_Window.h]

==============================================================================*/
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

HWND GameWindowCreate(HINSTANCE hInstance);

#endif // GAME_WINDOW_H