#include "Manager.h"
#include "Monster.h"
Monster::Monster(LPDIRECT3DDEVICE9 graphicDevice):Live(graphicDevice)
{
}

Monster::Monster(const Monster & gameobject) : Live(gameobject)
{
}

HRESULT Monster::AddComponent()
{
	return NOERROR;
}

HRESULT Monster::SetupOnTerrain()
{
	Live::SetupOnTerrain();
	return NOERROR;
}

_bool Monster::CheckPlayer(_float dist)
{
	_float length = D3DXVec3Length(&(_vec3(playerMat->m[3]) - transform->GetState(Transform::POSITION)));
	return _bool(length > dist);
}

HRESULT Monster::LateInit()
{
	return NOERROR;
}

HRESULT Monster::ReadyProto()
{
	return NOERROR;
}

_int Monster::Update(_double timeDelta)
{
	return _int();
}

_int Monster::LateUpdate(_double timeDelta)
{
	return _int();
}
HRESULT Monster::Ready(void * argument)
{
	return NOERROR;
}


HRESULT Monster::Render()
{
	return NOERROR;
}

HRESULT Monster::Parsing()
{
	return NOERROR;
}

Monster * Monster::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	Monster* instance = new Monster(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create Monster");
		SafeRelease(instance);
	}
	return instance;
}

GameObject * Monster::Clone(void * argument)
{
	Monster* instance = new Monster(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clone Monster");
		SafeRelease(instance);
	}
	return instance;
}

void Monster::Free()
{
	Live::Free();
}
