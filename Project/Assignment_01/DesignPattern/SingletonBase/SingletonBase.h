#pragma once

template<typename T>
class SingletonBase {
protected:
	static T* instance;
	SingletonBase() {}
	~SingletonBase() {}
public:
	static T* GetSingleton() {
		if (!instance) instance = new T();
		return instance;
	}
	static void ReleaseSingleton() {
		if (instance) delete instance;
		instance = nullptr;
	}
};

template<typename T>
T* SingletonBase<T>::instance = nullptr;