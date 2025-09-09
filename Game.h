/*==============================================================================

   Game [Game.h]

==============================================================================*/
#ifndef GAME_H
#define GAME_H

const constexpr int SCREEN_WIDTH = 1920;
const constexpr int SCREEN_HEIGHT = 1080;

void Game_Initialize();
void Game_Finalize();

void Game_Update(double elapsed_time);
void Game_Draw();

void AABB_Bullet_VS_Enemy();
void AABB_Player_VS_Enemy();

#endif // GAME_H