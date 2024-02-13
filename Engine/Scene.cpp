#include "..\Headers\Scene.h"

Scene::Scene(LPDIRECT3DDEVICE9 graphicDevice):graphicDevice(graphicDevice)
{
	AddRefCount(graphicDevice);
}

HRESULT Scene::ReadyScene()
{
	return NOERROR;
}

_int Scene::Update(_double timeDelta)
{
	return _int();
}

_int Scene::LateUpdate(_double timeDelta)
{
	return _int();
}

HRESULT Scene::Render()
{
	return NOERROR;
}

void Scene::Free()
{
	SafeRelease(graphicDevice);
}
