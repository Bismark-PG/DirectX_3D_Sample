/*==============================================================================

   Keyboard Input Set [KeyLogger.h]

==============================================================================*/

#ifndef KEY_LOGGER_H
#define KEY_LOGGER_H

#include "keyboard.h"

void KeyLogger_Initialize();

void KeyLogger_Update();

bool KeyLogger_IsPressed(Keyboard_Keys key);
bool KeyLogger_IsTrigger(Keyboard_Keys key);
bool KeyLogger_IsReleased(Keyboard_Keys key);

#endif // KEY_LOGGER_H