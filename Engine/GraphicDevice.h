#pragma once

#include "Base.h"
BEGIN(Engine)

class GraphicDevice final :public Base {
	DECLARE_SINGLETON(GraphicDevice)
private:
	LPDIRECT3D9 SDK = nullptr;
	LPDIRECT3DDEVICE9 Device = nullptr;
	explicit GraphicDevice();
	virtual ~GraphicDevice() = default;

public:
	enum MODE { FULLSCREEN, WINDOW };
	LPDIRECT3DDEVICE9 GetGraphic()
	{
		return Device;
	}
	HRESULT MakeGraphicReady(HWND hWnd, MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9* graphicDevice);
	virtual void Free();
};

END