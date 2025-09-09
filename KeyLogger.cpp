/*==============================================================================

   Keyboard Input Setting [KeyLogger.cpp]

==============================================================================*/
#include "KeyLogger.h"

static Keyboard_State PrevState{};
static Keyboard_State TriggerState{};
static Keyboard_State ReleaseState{};

void KeyLogger_Initialize()
{
	Keyboard_Initialize();
}

void KeyLogger_Update()
{
	// Get Now Keyboard State
	const Keyboard_State* P_State = Keyboard_GetState();
	
	LPBYTE P_Now	 = (LPBYTE)P_State;
	LPBYTE P_Prev    = (LPBYTE)&PrevState;
	LPBYTE P_Trigger = (LPBYTE)&TriggerState;
	LPBYTE P_Release = (LPBYTE)&ReleaseState;

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		P_Trigger[i] = (P_Prev[i] ^ P_Now[i]) & P_Now[i];
		P_Release[i] = (P_Prev[i] ^ P_Now[i]) & P_Prev[i];
	}

	PrevState = *P_State;
}

bool KeyLogger_IsPressed(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key);
}

bool KeyLogger_IsTrigger(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &TriggerState);
}

bool KeyLogger_IsReleased(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &ReleaseState);
}
