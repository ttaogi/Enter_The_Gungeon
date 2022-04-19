#include "Stdafx/stdafx.h"

#include "KeyManager.h"

KeyManager::KeyManager() {
	for (int i = 0; i < KEY_MAX; ++i) {
		SetKeyDown(i, false);
		SetKeyUp(i, false);
	}
}

KeyManager::~KeyManager() { }

bool KeyManager::IsOnceKeyDown(int key) {
	if (GetAsyncKeyState(key) & 0x8000) {
		if (GetKeyDown()[key] == false) {
			SetKeyDown(key, true);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		SetKeyDown(key, false);
		return false;
	}
}

bool KeyManager::IsOnceKeyUp(int key) {
	if (!(GetAsyncKeyState(key) & 0x8000)) {
		if (GetKeyDown()[key] == true) {
			SetKeyDown(key, false);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		SetKeyDown(key, true);
		return false;
	}
}

bool KeyManager::IsStayKeyDown(int key) {
	if (GetAsyncKeyState(key) & 0x8000) {
		if (GetKeyDown()[key] == true) {
			return true;
		}
		else {
			SetKeyDown(key, true);
			return false;
		}
	}
	else {
		SetKeyDown(key, false);
		return false;
	}
}

bool KeyManager::IsTrigger(int key) {
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}
