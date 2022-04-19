#pragma once

#include <random>

#include "../DesignPattern/SingletonBase/SingletonBase.h"

class RandomFunction : public SingletonBase<RandomFunction> {
private:
	std::random_device rd;
	std::mt19937_64 gen;
public:
	RandomFunction() {
		gen = std::mt19937_64(rd());
	}
	~RandomFunction() {}

	int GetInt(int num) {
		std::uniform_int_distribution<int> intUniDist(0, num - 1);
		return intUniDist(gen);
	}
	int GetIntFromTo(int from, int to) {
		std::uniform_int_distribution<int> intUniDist(from, to);
		return intUniDist(gen);
	}

	float GetFloat() {
		std::uniform_real_distribution<float> floatUniDist(0, 1);
		return floatUniDist(gen);
	}
	float GetFloat(float num) {
		std::uniform_real_distribution<float> floatUniDist(0, num);
		return floatUniDist(gen);
	}
	float GetFloatFromTo(float from, float to) {
		std::uniform_real_distribution<float> floatUniDist(from, to);
		return floatUniDist(gen);
	}
};