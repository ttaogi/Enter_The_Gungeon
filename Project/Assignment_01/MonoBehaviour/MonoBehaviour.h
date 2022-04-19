#pragma once
#pragma warning(disable: 26495)

#include <functional>
#include <Windows.h>
#include <time.h>
#define _USE_MATH_DEFINES // M_PI available.
#include <math.h>

#include "../Image/Animator/Animator.h"
#include "../StateMachine/StateMachine.h"
#include "../Utility/CommonMacroFunction.h"
#include "../Utility/EnumClasses.h"

class SceneManager;

struct F_POINT {
	float x;
	float y;

	F_POINT& operator = (const POINT& p) {
		x = (float)p.x;
		y = (float)p.y;
		return *this;
	}

	F_POINT operator + (const F_POINT& p) {
		return F_POINT{ x + p.x , y + p.y };
	}

	F_POINT operator - (const F_POINT& p) {
		return F_POINT{ x - p.x, y - p.y };
	}

	F_POINT operator * (const float& f) {
		return F_POINT{ x * f, y * f };
	}

	F_POINT operator / (const int& i) {
		return F_POINT{ x / 2, y / 2 };
	}

	POINT ToPoint() {
		return POINT{ (LONG)x, (LONG)y };
	}

	float length() {
		return sqrt(x * x + y * y);
	}
};

struct VELOCITY {
	float x;
	float y;

	void SetX(float speed) { x = speed; }
	float GetX() const { return x; }
	void SetY(float speed) { y = speed; }
	float GetY() const { return y; }

	void Accelerate(F_POINT acc) { x += acc.x; y += acc.y; }
	float Length() { return sqrt(x * x + y * y); }
	F_POINT ToFPoint() { return F_POINT{ x, y }; }
};

class MonoBehaviour {
protected:
	MONOBEHAVIOUR_TYPE type;
	F_POINT pos;
	VELOCITY vel;
	RECT rc;
	Image* stripe;
	Animator* animator;
	StateMachine* stateMachine;
	bool destroy;
public:
	virtual ~MonoBehaviour() {}

	virtual void Init() = 0;
	virtual void Update(HWND hWnd) = 0;
	virtual void Update(HWND hWnd, SceneManager* _scnMgr) = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC _hdc) = 0;
	virtual void Render(HDC _hdc, POINT _coordinateRevision) = 0;

	void Move() {
		pos.x += vel.x;
		pos.y += vel.y;
		rc = MakeRectCWH(pos.ToPoint(), rc.right - rc.left, rc.bottom - rc.top);
	}
	void Move(POINT _deltaPos) {
		pos.x += _deltaPos.x;
		pos.y += _deltaPos.y;
		rc = MakeRectCWH(pos.ToPoint(), rc.right - rc.left, rc.bottom - rc.top);
	}

	bool IsCollided(const RECT* _rc) const {
		RECT tmpRc;
		if (IntersectRect(&tmpRc, &rc, _rc) != 0)
			return true;
		else
			return false;
	}

	void MoveCollisionCheck(RECT* _rc) {
		RECT tmpRc;
		POINT tmpPos = pos.ToPoint();

		if (IntersectRect(&tmpRc, &rc, _rc) != 0) {
			::SetRect(&tmpRc, 0, 0, tmpRc.right - tmpRc.left, tmpRc.bottom - tmpRc.top);

			if (tmpRc.right > tmpRc.bottom) {
				if ((rc.top + rc.bottom) < (_rc->top + _rc->bottom)) {
					tmpPos.y -= (tmpRc.bottom + 1);
					SetPos(tmpPos);
					SetRect(MakeRectCWH(tmpPos, rc.right - rc.left, rc.bottom - rc.top));
				}
				else {
					tmpPos.y += (tmpRc.bottom + 1);
					SetPos(tmpPos);
					SetRect(MakeRectCWH(tmpPos, rc.right - rc.left, rc.bottom - rc.top));
				}
			}
			else {
				if ((rc.left + rc.right) < (_rc->left + _rc->right)) {
					tmpPos.x -= (tmpRc.right + 1);
					SetPos(tmpPos);
					SetRect(MakeRectCWH(tmpPos, rc.right - rc.left, rc.bottom - rc.top));
				}
				else {
					tmpPos.x += (tmpRc.right + 1);
					SetPos(tmpPos);
					SetRect(MakeRectCWH(tmpPos, rc.right - rc.left, rc.bottom - rc.top));
				}
			}
		}
	}

	MONOBEHAVIOUR_TYPE GetType() const { return type; }
	void SetPos(POINT _pos) { pos = _pos; }
	POINT GetPos() const { return POINT{ (LONG)pos.x, (LONG)pos.y }; }
	void SetFPos(F_POINT _pos) { pos = _pos; }
	F_POINT GetFPos() const { return pos; }
	VELOCITY GetVelocity() const { return vel; }
	VELOCITY* GetVelocityP() { return &vel; }
	void SetVelocity(VELOCITY _vel) { vel = _vel; }
	void SetRect(RECT _rc) { rc = _rc; }
	RECT GetRect() const { return rc; }
	const RECT* GetRectP() const { return &rc; }
	void SetDestroy(bool _state) { destroy = _state; }
	Animator* GetAnimator() const { return animator; }
	bool GetDestroy() const { return destroy; }
};