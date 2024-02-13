#include "..\Headers\GameObject.h"
#include "Manager.h"
#include "Component.h"


GameObject::GameObject(LPDIRECT3DDEVICE9 graphicDevice) :graphicDevice(graphicDevice)
{
	AddRefCount(this->graphicDevice);
}

GameObject::GameObject(const GameObject & gameObject) : graphicDevice(gameObject.graphicDevice)
{
	AddRefCount(graphicDevice);
}

HRESULT GameObject::AddComponent(_uint sceneID, const _tchar* prototypeTag, const _tchar* componentTag, Component** component, void* argument)
{
	Manager* manager = Manager::GetInstance();
	if (manager == nullptr)
		return E_FAIL;

	AddRefCount(manager);

	if (FindComponent(componentTag)!=nullptr)
		return E_FAIL;
	Component* _component = manager->Clone(sceneID, prototypeTag, argument);
	if (_component == nullptr)
		return E_FAIL;
	components.insert(COMPONENTS::value_type(componentTag, _component));
	*component = _component;
	AddRefCount(_component);
	SafeRelease(manager);
	return NOERROR;
}

HRESULT GameObject::AddComponent(_uint sceneID, const _tchar* prototypeTag,Component** component, void* argument)
{
	Manager* manager = Manager::GetInstance();
	if (manager == nullptr)
		return E_FAIL;

	AddRefCount(manager);

	Component* _component = manager->Clone(sceneID, prototypeTag, argument);
	if (_component == nullptr)
		return E_FAIL;
	*component = _component;
	SafeRelease(manager);
	return NOERROR;
}

HRESULT GameObject::SortZ(_vec3 worldPos)
{
	PipeLine* pl = PipeLine::GetInstance();

	if (pl == nullptr)
		return E_FAIL;
	AddRefCount(pl);

	_matrix view;

	D3DXMatrixInverse(&view, nullptr, &pl->GetTransform(D3DTS_VIEW));
	viewZ = D3DXVec3Length(&(worldPos - *(_vec3*)&view.m[3][0]));
	SafeRelease(pl);
	return NOERROR;
}

Component * GameObject::FindComponent(const _tchar * componentTag)const
{
	auto iter = find_if(components.begin(), components.end(), TagFinder(componentTag));
	if (iter == components.end())
		return nullptr;

	return iter->second;
}

HRESULT GameObject::LateInit()
{
	return NOERROR;
}

HRESULT GameObject::ReadyProto()
{
	return NOERROR;
}

HRESULT GameObject::Ready(void* argument)
{
	return NOERROR;
}

const Component * GameObject::GetComponent(const _tchar * componentTag) const
{
	return FindComponent(componentTag);
}

_int GameObject::Update(_double timeDelta)
{
	return _int();
}

_int GameObject::LateUpdate(_double timeDelta)
{
	return _int();
}

HRESULT GameObject::Render()
{
	return NOERROR;
}

GameObject * GameObject::Clone(void * argument)
{
	return nullptr;
}

void GameObject::Free()
{
	for (auto& pair : components)
		SafeRelease(pair.second);
	components.clear();
	SafeRelease(graphicDevice);
}
