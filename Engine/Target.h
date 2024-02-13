#pragma once
#include "Base.h"
#include "ViewportRect.h"
BEGIN(Engine)
class Shader;
class Target final :
	public Base
{
private:
#ifdef _DEBUG
	ViewportRect* debugBuffer = nullptr;
#endif
	D3DXCOLOR clearColor;
	LPDIRECT3DDEVICE9 graphic = nullptr;
	LPDIRECT3DTEXTURE9 targetTexture = nullptr;
	LPDIRECT3DSURFACE9 targetSurface = nullptr;
	LPDIRECT3DSURFACE9 oldSurface = nullptr;

	explicit Target(LPDIRECT3DDEVICE9 graphic);
	virtual ~Target() = default;

public:
#ifdef _DEBUG
	HRESULT ReadyDebugBuffer(_float startX, _float startY, _float sizeX, _float sizeY);
	HRESULT RenderDebugBuffer();
#endif
	HRESULT Ready(_uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor);
	HRESULT SetupGraphicDev(_uint index);
	HRESULT SetShader(Shader* shader, D3DXHANDLE constantName);
	HRESULT ReleaseGraphicDev(_uint index);
	HRESULT ClearTarget();
	static Target* Create(LPDIRECT3DDEVICE9 graphicDevice, _uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor);
	virtual void Free();
};
END