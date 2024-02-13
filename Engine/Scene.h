#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL Scene abstract:
public Base
{
protected:
	explicit Scene(LPDIRECT3DDEVICE9 graphicDevice);
	virtual ~Scene() = default;

private:
	LPDIRECT3DDEVICE9 graphicDevice = nullptr;
public:
	LPDIRECT3DDEVICE9 GetGraphic() const { return graphicDevice; }
	virtual HRESULT ReadyScene();
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render();

	virtual void Free();
};

END