#pragma once
#include "Base.h"
BEGIN(Engine)
class Shader;
class ViewportRect;
class ENGINE_DLL Light final:
public Base
{
private:
	LPDIRECT3DDEVICE9 graphicDev = nullptr;
	_LIGHT light;
	ViewportRect* buffer = nullptr;

	explicit Light(LPDIRECT3DDEVICE9 graphicDev);
virtual ~Light() = default;

public:
	void SetPos(_vec3 pos) { light.Position = pos; };
	_LIGHT GetLight() const { return light; }
	HRESULT Ready(const _LIGHT* light);
	_int Update(_double timeDelta);
	HRESULT Render(Shader* shader);

	static Light* Create(LPDIRECT3DDEVICE9 graphicDevice, const _LIGHT* light);
	virtual void Free();
};

END