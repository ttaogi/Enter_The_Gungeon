#pragma once

#pragma region		STRIPE
// background.
extern const std::wstring KEY_BACKGROUND_BACKBUFFER;
extern const wchar_t* BACKGROUND_BACKBUFFER;
extern const std::wstring KEY_BACKGROUND_ENDSCENE;
extern const wchar_t* BACKGROUND_ENDSCENE;
extern const std::wstring KEY_BACKGROUND_TITLESCENE;
extern const wchar_t* BACKGROUND_TITLESCENE;
extern const std::wstring KEY_BACKGROUND_BATTLESTAGESCENE;
extern const wchar_t* BACKGROUND_BATTLESTAGESCENE;


// tile.
extern const std::wstring KEY_TILE_GROUND;
extern const wchar_t* TILE_GROUND;
extern const std::wstring KEY_TILE_PILLAR;
extern const wchar_t* TILE_PILLAR;
extern const std::wstring KEY_TILE_PILLAR_TOP;
extern const wchar_t* TILE_PILLAR_TOP;
extern const std::wstring KEY_TILE_WALL;
extern const wchar_t* TILE_WALL;


// ui - minimap
extern const std::wstring KEY_UI_BLACK_BACKGROUND;
extern const wchar_t* UI_BLACK_BACKGROUND;
extern const std::wstring KEY_UI_MINIMAP;
extern const wchar_t* UI_MINIMAP;
extern const std::wstring KEY_UI_PLAYER_MARKER;
extern const wchar_t* UI_PLAYER_MARKER;

// ui - gun and bullet.
extern const std::wstring KEY_UI_GUN_BACKGROUND;
extern const wchar_t* UI_GUN_BACKGROUND;
extern const std::wstring KEY_UI_GUN_GUN;
extern const wchar_t* UI_GUN_GUN;
extern const std::wstring KEY_UI_GUN_BULLET_BASE;
extern const wchar_t* UI_GUN_BULLET_BASE;
extern const std::wstring KEY_UI_GUN_BULLET;
extern const wchar_t* UI_GUN_BULLET;


// ui - hp.
extern const std::wstring KEY_UI_HP_FULL;
extern const wchar_t* UI_HP_FULL;
extern const std::wstring KEY_UI_HP_HALF;
extern const wchar_t* UI_HP_HALF;
extern const std::wstring KEY_UI_HP_EMPTY;
extern const wchar_t* UI_HP_EMPTY;


// ui - boss hp.
extern const std::wstring KEY_UI_BOSS_HP_BACKGROUND;
extern const wchar_t* UI_BOSS_HP_BACKGROUND;
extern const std::wstring KEY_UI_BOSS_HP_BAR;
extern const wchar_t* UI_BOSS_HP_BAR;


// ui - skill.
extern const std::wstring KEY_UI_SKILL_BACKGROUND;
extern const wchar_t* UI_SKILL_BACKGROUND;
extern const std::wstring KEY_UI_SKILL_SKILL;
extern const wchar_t* UI_SKILL_SKILL;
extern const std::wstring KEY_UI_SKILL_COOL_BACKGROUND;
extern const wchar_t* UI_SKILL_COOL_BACKGROUND;
extern const std::wstring KEY_UI_SKILL_COOL_BAR;
extern const wchar_t* UI_SKILL_COOL_BAR;


// ui - button.
extern const std::wstring KEY_UI_BUTTON_QUIT;
extern const wchar_t* UI_BUTTON_QUIT;


// cut scene.
extern const std::wstring KEY_CUT_SCENE_BOSS;
extern const wchar_t* CUT_SCENE_BOSS;


// player stripe.
extern const wchar_t* PLAYER_IDLE_LEFT_STRIPE;
extern const wchar_t* PLAYER_IDLE_TOP_STRIPE;
extern const wchar_t* PLAYER_IDLE_RIGHT_STRIPE;
extern const wchar_t* PLAYER_IDLE_BOTTOM_STRIPE;
extern const wchar_t* PLAYER_MOVE_LEFT_STRIPE;
extern const wchar_t* PLAYER_MOVE_TOP_STRIPE;
extern const wchar_t* PLAYER_MOVE_RIGHT_STRIPE;
extern const wchar_t* PLAYER_MOVE_BOTTOM_STRIPE;
extern const wchar_t* PLAYER_DODGE_LEFT_STRIPE;
extern const wchar_t* PLAYER_DODGE_TOP_STRIPE;
extern const wchar_t* PLAYER_DODGE_RIGHT_STRIPE;
extern const wchar_t* PLAYER_DODGE_BOTTOM_STRIPE;
extern const wchar_t* PLAYER_DEAD_STRIPE;
extern const std::wstring KEY_PLAYER_SHADOW;
extern const wchar_t* PLAYER_SHADOW;


// player gun stripe.
extern const wchar_t* PLAYER_GUN_IDLE_LEFT_STRIPE;
extern const wchar_t* PLAYER_GUN_IDLE_TOP_STRIPE;
extern const wchar_t* PLAYER_GUN_IDLE_RIGHT_STRIPE;
extern const wchar_t* PLAYER_GUN_IDLE_BOTTOM_STRIPE;
extern const wchar_t* PLAYER_GUN_MOVE_LEFT_STRIPE;
extern const wchar_t* PLAYER_GUN_MOVE_TOP_STRIPE;
extern const wchar_t* PLAYER_GUN_MOVE_RIGHT_STRIPE;
extern const wchar_t* PLAYER_GUN_MOVE_BOTTOM_STRIPE;
extern const wchar_t* PLAYER_GUN_DODGE_LEFT_STRIPE;


// player reloading stripe.
extern const wchar_t* RELOADING_RELOADING_STRIPE;
extern const wchar_t* RELOADING_DEFAULT_STRIPE;


// enemy stripe.
extern const wchar_t* ENEMY_IDLE_LEFT_STRIPE;
extern const wchar_t* ENEMY_IDLE_TOP_STRIPE;
extern const wchar_t* ENEMY_IDLE_RIGHT_STRIPE;
extern const wchar_t* ENEMY_IDLE_BOTTOM_STRIPE;
extern const wchar_t* ENEMY_MOVE_LEFT_STRIPE;
extern const wchar_t* ENEMY_MOVE_TOP_STRIPE;
extern const wchar_t* ENEMY_MOVE_RIGHT_STRIPE;
extern const wchar_t* ENEMY_MOVE_BOTTOM_STRIPE;
extern const std::wstring KEY_ENEMY_SHADOW;
extern const wchar_t* ENEMY_SHADOW;


// enemy gun stripe.
extern const wchar_t* ENEMY_GUN_IDLE_LEFT_STRIPE;
extern const wchar_t* ENEMY_GUN_IDLE_TOP_STRIPE;
extern const wchar_t* ENEMY_GUN_IDLE_RIGHT_STRIPE;
extern const wchar_t* ENEMY_GUN_IDLE_BOTTOM_STRIPE;
extern const wchar_t* ENEMY_GUN_MOVE_LEFT_STRIPE;
extern const wchar_t* ENEMY_GUN_MOVE_TOP_STRIPE;
extern const wchar_t* ENEMY_GUN_MOVE_RIGHT_STRIPE;
extern const wchar_t* ENEMY_GUN_MOVE_BOTTOM_STRIPE;


// boss stripe.
extern const wchar_t* BOSS_IDLE_LEFT_STRIPE;
extern const wchar_t* BOSS_IDLE_TOP_STRIPE;
extern const wchar_t* BOSS_IDLE_RIGHT_STRIPE;
extern const wchar_t* BOSS_IDLE_BOTTOM_STRIPE;
extern const wchar_t* BOSS_MOVE_LEFT_STRIPE;
extern const wchar_t* BOSS_MOVE_TOP_STRIPE;
extern const wchar_t* BOSS_MOVE_RIGHT_STRIPE;
extern const wchar_t* BOSS_MOVE_BOTTOM_STRIPE;
extern const std::wstring KEY_BOSS_SHADOW;
extern const wchar_t* BOSS_SHADOW;


// enemy gun stripe.
extern const wchar_t* BOSS_GUN_IDLE_LEFT_STRIPE;
extern const wchar_t* BOSS_GUN_IDLE_TOP_STRIPE;
extern const wchar_t* BOSS_GUN_IDLE_RIGHT_STRIPE;
extern const wchar_t* BOSS_GUN_IDLE_BOTTOM_STRIPE;
extern const wchar_t* BOSS_GUN_MOVE_LEFT_STRIPE;
extern const wchar_t* BOSS_GUN_MOVE_TOP_STRIPE;
extern const wchar_t* BOSS_GUN_MOVE_RIGHT_STRIPE;
extern const wchar_t* BOSS_GUN_MOVE_BOTTOM_STRIPE;


extern const wchar_t* MUZZLE_FLASH_LEFT_STRIPE;
extern const wchar_t* MUZZLE_FLASH_TOP_STRIPE;
extern const wchar_t* MUZZLE_FLASH_RIGHT_STRIPE;
extern const wchar_t* MUZZLE_FLASH_BOTTOM_STRIPE;


extern const std::wstring KEY_BULLET_STRIPE;
extern const wchar_t* BULLET_STRIPE;
#pragma endregion	STRIPE


#pragma region		SOUND
extern const std::wstring KEY_TITLE_THEME_SOUND;
extern const wchar_t* TITLE_THEME_SOUND;
extern const std::wstring KEY_BATTLE_STAGE_THEME_SOUND;
extern const wchar_t* BATTLE_STAGE_THEME_SOUND;
extern const std::wstring KEY_BOSS_THEME_SOUND;
extern const wchar_t* BOSS_THEME_SOUND;


extern const std::wstring KEY_DEFAULT_GUN_SOUND;
extern const wchar_t* DEFAULT_GUN_SOUND;
extern const std::wstring KEY_PLAYER_SKILL_SOUND;
extern const wchar_t* PLAYER_SKILL_SOUND;
extern const std::wstring KEY_PLAYER_RELOAD_SOUND;
extern const wchar_t* PLAYER_RELOAD_SOUND;
#pragma endregion	SOUND

