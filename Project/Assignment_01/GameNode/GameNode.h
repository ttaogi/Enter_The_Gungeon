#pragma  once

#include "../Image/Image.h"

static Image* backBuffer = NULL;

class GameNode {
private:
	HDC nodeHdc;
	bool managerInit;
public:
	GameNode();
	virtual ~GameNode();

	virtual HRESULT Init();
	virtual HRESULT Init(bool _managerInit);
	virtual void Release();

	virtual void Update(HWND hWnd);
	virtual void Render(HDC _hdc);

	Image* GetBackBuffer(void) { return backBuffer; }

	HDC getMemDC() { return backBuffer->GetMemDC(); }
	HDC getHDC() { return nodeHdc; }

	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};