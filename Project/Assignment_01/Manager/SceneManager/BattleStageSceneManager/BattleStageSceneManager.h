#pragma once

#include "../SceneManager.h"
#include "../../../MonoBehaviour/Enemy/Enemy.h"
#include "../../../MonoBehaviour/Player/Player.h"
#include "../../../Map/Map.h"

#define BOSS_CUT_SCENE_COUNT	72

class BattleStageSceneManager : public SceneManager {
private:
	Map* map;
	Button endBtn;

	Image* bgImage;
	Image* uiGunBackground;
	Image* uiGun;
	Image* uiBulletBase;
	Image* uiBullet;
	Image* uiHpFull;
	Image* uiHpHalf;
	Image* uiHpEmpty;
	Image* uiSkillBackground;
	Image* uiSkillSkill;
	Image* uiSkillCoolBackground;
	Image* uiSkillCoolBar;
	Image* uiBossHpBackground;
	Image* uiBossHpBar;
	Image* bossCutScene;
	Image* minimap;

	Player player;
	std::vector<Enemy*> enemyVec;
	Room* curRoom;
	std::wstring msg;

	int bgSpeed;
	int bgOffsetX;
	int bgOffsetY;
	int bossCutSceneCount;

	void SetBackBuffer();
public:
	BattleStageSceneManager(MainGame* mg);
	~BattleStageSceneManager() {}

	void Init(MainGame* mg);
	void Update(HWND hWnd);
	void LateUpdate();
	void Release();
	void Render(HDC hdc);

	Player* GetPlayerP() { return &player; }

	void AddEnemy(Enemy* _enemy, Room* _room) { enemyVec.push_back(_enemy); curRoom = _room; }
	void ShowBossCutScene();
	void GameClear();
};