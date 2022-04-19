#pragma once

#include <bitset>

#include "../../DesignPattern/SingletonBase/SingletonBase.h"

#define KEY_MAX 256

class KeyManager : public SingletonBase<KeyManager> {
private:
	std::bitset<KEY_MAX> keyDown;
	std::bitset<KEY_MAX> keyUp;
public:
	KeyManager();
	~KeyManager();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
	bool IsTrigger(int key);

	std::bitset<KEY_MAX> GetKeyDown() { return keyDown; }
	std::bitset<KEY_MAX> GetKeyUp() { return keyUp; }

	void SetKeyDown(int key, bool state) { keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { keyUp.set(key, state); }
};