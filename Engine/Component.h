#pragma once
#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL Component abstract:
public Base
{
private:
	LPDIRECT3DDEVICE9 graphicDevice = nullptr;
	_bool isClone = false;
protected:
	explicit Component(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Component(const Component& gameObject);
	virtual ~Component() = default;

public:
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);
	virtual Component* Clone(void* argument) = 0;
	virtual void Free();
	LPDIRECT3DDEVICE9 GetGraphic()const
	{
		return graphicDevice;
	}
	_bool IsClone() {return isClone;}
};

END