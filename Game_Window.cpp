/*==============================================================================

   Make Window [Game_Window.cpp]

==============================================================================*/
#include "Game_Window.h"
#include <algorithm>
#include "keyboard.h"
#include "mouse.h"

static constexpr char WINDOW_CLASS[] = "GameWindow";
static constexpr char TITLE[] = "Show Windows";

const constexpr int SCREEN_WIDTH = 1920;
const constexpr int SCREEN_HEIGHT = 1080;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND GameWindowCreate(HINSTANCE hInstance)
{
	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr; // no menu
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	//const constexpr int SCREEN_WIDTH = 1600;
	//const constexpr int SCREEN_HEIGHT = 900;

	RECT WindowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	DWORD WinStyle = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);

	AdjustWindowRect(&WindowRect, WinStyle, FALSE);

	const int WINDOW_WIDTH = WindowRect.right - WindowRect.left;
	const int WINDOW_HEIGTH = WindowRect.bottom - WindowRect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	const int windowX = std::max((desktop_width - WINDOW_WIDTH) / 2, 0);
	const int windowY = std::max((desktop_height - WINDOW_HEIGTH) / 2, 0);

	HWND hWnd = CreateWindow(WINDOW_CLASS,
		TITLE,
		WinStyle,
		windowX,
		windowY,
		WINDOW_WIDTH,
		WINDOW_HEIGTH,
		nullptr, nullptr, hInstance, nullptr);

	return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	    Mouse_ProcessMessage(message, wParam, lParam);
	    break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	    Keyboard_ProcessMessage(message, wParam, lParam);
	    break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "Really Want Exit Game?", "WARNING.", MB_OKCANCEL) == IDOK)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}