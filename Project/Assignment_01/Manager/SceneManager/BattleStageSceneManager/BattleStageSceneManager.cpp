#include "Stdafx/stdafx.h"

#include "BattleStageSceneManager.h"
#include "../../../Map/Map.h"

BattleStageSceneManager::BattleStageSceneManager(MainGame* mg)
	: SceneManager(SCENE_TYPE::BATTLE_STAGE)
{
	map = NULL;
	endBtn = Button(
		RECT{
			WINSIZE_X / 2 - 100, WINSIZE_Y / 2 - 100,
			WINSIZE_X / 2 + 100, WINSIZE_Y / 2 + 100
		}, L"",
		IMG->FindImage(KEY_UI_BUTTON_QUIT)
	);
	endBtn.SetCallBack_v_CB_v(std::bind(&MainGame::SetNextScene_TITLE, mg));
	endBtn.SetDestroy(true);

	bgImage = NULL;
	uiGunBackground = NULL;
	uiGun = NULL;
	uiBulletBase = NULL;
	uiBullet = NULL;
	uiHpFull = NULL;
	uiHpHalf = NULL;
	uiHpEmpty = NULL;
	uiSkillBackground = NULL;
	uiSkillSkill = NULL;
	uiSkillCoolBackground = NULL;
	uiSkillCoolBar = NULL;
	uiBossHpBackground = NULL;
	uiBossHpBar = NULL;
	bossCutScene = NULL;
	minimap = NULL;

	player.SetCallBack_v_CB_pMb(std::bind(&BattleStageSceneManager::AddEffect, this, std::placeholders::_1));
	msg = L": ";

	bgSpeed = 20;
	bgOffsetX = 0;
	bgOffsetY = 0;
	bossCutSceneCount = 0;
}

void BattleStageSceneManager::SetBackBuffer() {
	SAFE_RELEASE(backBuffer);
	SAFE_DELETE(backBuffer);
	backBuffer = new Image;
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);
}

void BattleStageSceneManager::Init(MainGame* mg) {
	SetBackBuffer();

	map = new Map(this);

	bgImage = IMG->FindImage(KEY_BACKGROUND_BATTLESTAGESCENE);
	uiGunBackground = IMG->FindImage(KEY_UI_GUN_BACKGROUND);
	uiGun = IMG->FindImage(KEY_UI_GUN_GUN);
	uiBulletBase = IMG->FindImage(KEY_UI_GUN_BULLET_BASE);
	uiBullet = IMG->FindImage(KEY_UI_GUN_BULLET);
	uiHpFull = IMG->FindImage(KEY_UI_HP_FULL);
	uiHpHalf = IMG->FindImage(KEY_UI_HP_HALF);
	uiHpEmpty = IMG->FindImage(KEY_UI_HP_EMPTY);
	uiSkillBackground = IMG->FindImage(KEY_UI_SKILL_BACKGROUND);
	uiSkillSkill = IMG->FindImage(KEY_UI_SKILL_SKILL);
	uiSkillCoolBackground = IMG->FindImage(KEY_UI_SKILL_COOL_BACKGROUND);
	uiSkillCoolBar = IMG->FindImage(KEY_UI_SKILL_COOL_BAR);
	uiBossHpBackground = IMG->FindImage(KEY_UI_BOSS_HP_BACKGROUND);
	uiBossHpBar = IMG->FindImage(KEY_UI_BOSS_HP_BAR);
	bossCutScene = IMG->FindImage(KEY_CUT_SCENE_BOSS);
	minimap = new Image;
	minimap->Init(WINSIZE_X, WINSIZE_Y);
	PatBlt(minimap->GetMemDC(), 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);
	minimap->SetTransColor(true, RGB(0, 0, 0));

	SOUND->Play(KEY_BATTLE_STAGE_THEME_SOUND);

	player.Init();
	player.SetPos(map->GetStartPlayerPos());
	player.SetRect(MakeRectCWH(player.GetPos(), 100, 100));
}

void BattleStageSceneManager::Update(HWND hWnd) {
	if (--bossCutSceneCount > 0) return;
	else bossCutSceneCount = 0;

	int cnt = 0;
	if (KEY->IsOnceKeyDown(VK_ESCAPE)) {
		if (endBtn.GetDestroy() == true) endBtn.SetDestroy(false);
		else endBtn.SetDestroy(true);
	}

	player.Update(hWnd, this);

	for (int i = 0; i < enemyVec.size(); ++i)
		enemyVec[i]->Update(hWnd, this);

	// bullet, muzzle flash.
	cnt = (int)effectVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		effectVec[i]->Update(hWnd, this);
	}

	// check collisions between player and walls, pillars.
	// update minimap and revise player`s position.
	RECT minimapRc{ (WINSIZE_X - 360) / 2, (WINSIZE_Y - 360) / 2, (WINSIZE_X + 360) / 2, (WINSIZE_Y + 360) / 2 };
	map->CollisionDetect(player, minimap->GetMemDC(), minimapRc);

	// check collisions between bullets and walls, pillars.
	// and set destroyed.
	cnt = (int)effectVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		if(effectVec[i]->GetType() == MONOBEHAVIOUR_TYPE::BULLET &&
			effectVec[i]->GetDestroy() == false
		) map->CollisionDetect((Bullet*)effectVec[i]);
	}

	// check collisions between player and bullets.
	cnt = (int)effectVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		if (effectVec[i]->GetDestroy() == false &&
			effectVec[i]->GetType() == MONOBEHAVIOUR_TYPE::BULLET &&
			((Bullet*)effectVec[i])->GetByPlayer() == false
		) {
			if (player.IsCollided(effectVec[i]->GetRectP()) == true) {
				player.GetAttacked();
				effectVec[i]->SetDestroy(true);
			}
		}
	}
	
	// check collisions between enemies and bullets.
	cnt = (int)effectVec.size();
	for (int i = 0; i < enemyVec.size(); ++i) {
		for (int j = cnt - 1; j >= 0; --j) {
			if (effectVec[j]->GetDestroy() == false &&
				effectVec[j]->GetType() == MONOBEHAVIOUR_TYPE::BULLET &&
				((Bullet*)effectVec[j])->GetByPlayer() == true
			) {
				if (enemyVec[i]->IsCollided(effectVec[j]->GetRectP()) == true) {
					enemyVec[i]->GetAttacked();
					effectVec[j]->SetDestroy(true);
				}
			}
		}
	}

	// delete destroyed MonoBehaviours.
	cnt = (int)effectVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		if (effectVec[i]->GetDestroy() == true) {
			SAFE_DELETE(effectVec[i]);
			effectVec.erase(effectVec.begin() + i);
		}
	}
	cnt = (int)enemyVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		if (enemyVec[i]->GetDestroy() == true) {
			SAFE_DELETE(enemyVec[i]);
			enemyVec.erase(enemyVec.begin() + i);
		}
	}
	effectVec.shrink_to_fit();
	enemyVec.shrink_to_fit();

	endBtn.Update(hWnd);

	bgOffsetX = (bgOffsetX + bgSpeed) % WINSIZE_X;

	SOUND->Update();
}

void BattleStageSceneManager::LateUpdate() { }

void BattleStageSceneManager::Release() {
	SAFE_RELEASE(backBuffer);
	SAFE_DELETE(backBuffer);
	SAFE_RELEASE(minimap);
	SAFE_DELETE(minimap);
	for (auto iter = effectVec.begin(); iter != effectVec.end(); ++iter) {
		SAFE_DELETE(*iter);
	}

	SOUND->AllStop();
}

void BattleStageSceneManager::Render(HDC hdc) {
	POINT coordinateRevision;
	HDC memDC = GetBackBuffer()->GetMemDC();

	// camera.
	if (bossCutSceneCount > 0) {
		coordinateRevision.x = player.GetPos().x;
		coordinateRevision.y = player.GetPos().y;

		for (Enemy* e : enemyVec) {
			if (e->GetType() == MONOBEHAVIOUR_TYPE::BOSS) {
				coordinateRevision.x = e->GetPos().x - WINSIZE_X / 2;
				coordinateRevision.y = e->GetPos().y - WINSIZE_Y / 2;
			}
		}
	}
	else {
		int mouseRevisionX = 0;
		int mouseRevisionY = 0;

		if (_ptMouse.x < 0) mouseRevisionX = 0;
		else if (_ptMouse.x > WINSIZE_X) mouseRevisionX = WINSIZE_X;
		else mouseRevisionX = _ptMouse.x;
		if (_ptMouse.y < 0) mouseRevisionY = 0;
		else if (_ptMouse.y > WINSIZE_Y) mouseRevisionY = WINSIZE_Y;
		else mouseRevisionY = _ptMouse.y;
		mouseRevisionX -= WINSIZE_X / 2;
		mouseRevisionY -= WINSIZE_Y / 2;

		coordinateRevision.x = player.GetPos().x - WINSIZE_X / 2 + mouseRevisionX / 3;
		coordinateRevision.y = player.GetPos().y - WINSIZE_Y / 2 + mouseRevisionY / 3;
	}

	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

	RECT loopRc{ 0, 0, WINSIZE_X, WINSIZE_Y };
	bgImage->LoopRender(memDC, &loopRc, bgOffsetX, bgOffsetY);

	map->Render(memDC, coordinateRevision);

	player.Render(memDC, coordinateRevision);

	for (auto iter = enemyVec.begin(); iter != enemyVec.end(); ++iter)
		(*iter)->Render(memDC, coordinateRevision);

	for (auto iter = effectVec.begin(); iter != effectVec.end(); ++iter)
		(*iter)->Render(memDC, coordinateRevision);

	// UIs render.
	uiGunBackground->Render(memDC, WINSIZE_X - 190, WINSIZE_Y - 110);
	uiGun->Render(memDC, WINSIZE_X - 180, WINSIZE_Y - 100);
	uiBulletBase->Render(memDC, WINSIZE_X - 25, WINSIZE_Y - 25);
	for (int i = 0; i < player.GetBulletCount(); ++i) {
		uiBullet->Render(memDC, WINSIZE_X - 25, WINSIZE_Y - 35 - (i * 10));
	}

	int hp = player.GetHp();
	for (int i = 0; i < player.GetHpMax() / 2; ++i) {
		if (hp >= 2)
			uiHpFull->Render(memDC, 10 + i * 30, 10);
		else if (hp == 1)
			uiHpHalf->Render(memDC, 10 + i * 30, 10);
		else
			uiHpEmpty->Render(memDC, 10 + i * 30, 10);
		hp -= 2;
	}

	float coolRate = (float)player.GetSkillCool() / player.GetSkillCoolMax();
	uiSkillBackground->Render(memDC, 10, WINSIZE_Y - 110);
	if (coolRate == 0)
		uiSkillSkill->Render(memDC, 10, WINSIZE_Y - 110);
	else
		uiSkillSkill->AlphaRender(memDC, 10, WINSIZE_Y - 110, 127);
	uiSkillCoolBackground->Render(memDC, 175, WINSIZE_Y - 110);
	if(coolRate != 0)
		uiSkillCoolBar->Render(memDC, 175, WINSIZE_Y - 10 - (int)(coolRate * 100), 10, (int)(coolRate * 100));

	for (Enemy* e : enemyVec) {
		if (e->GetType() == MONOBEHAVIOUR_TYPE::BOSS) {
			uiBossHpBackground->Render(memDC, WINSIZE_X / 2 - 267, WINSIZE_Y - 59);
			uiBossHpBar->Render(memDC, WINSIZE_X / 2 - 220, WINSIZE_Y - 49, 440 * (e->GetHp() / (float)e->GetHpMax()), 21);
			break;
		}
	}

	// minimap render.
	if (KEY->IsStayKeyDown(VK_TAB) && bossCutSceneCount <= 0) {
		RECT minimapRc{ (WINSIZE_X - 360) / 2, (WINSIZE_Y - 360) / 2, (WINSIZE_X + 360) / 2, (WINSIZE_Y + 360) / 2 };
		IMG->FindImage(KEY_UI_BLACK_BACKGROUND)->AlphaRender(memDC, 127);
		IMG->FindImage(KEY_UI_MINIMAP)->Render(memDC);
		minimap->Render(memDC);
		player.Render(memDC, minimapRc);
	}

	if (bossCutSceneCount > 0 && bossCutSceneCount < BOSS_CUT_SCENE_COUNT *2 / 3) {
		IMG->FindImage(KEY_UI_BLACK_BACKGROUND)->AlphaRender(memDC, 127);
		bossCutScene->Render(memDC);
	}

	endBtn.Render(memDC);

	GetBackBuffer()->Render(hdc, 0, 0);
}

void BattleStageSceneManager::ShowBossCutScene() {
	bossCutSceneCount = BOSS_CUT_SCENE_COUNT;
}

void BattleStageSceneManager::GameClear() {
	int cnt = (int)effectVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		effectVec[i]->SetDestroy(true);
	}
	cnt = (int)enemyVec.size();
	for (int i = cnt - 1; i >= 0; --i) {
		enemyVec[i]->SetDestroy(true);
	}
	endBtn.SetDestroy(false);
}
