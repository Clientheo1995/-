#include "..\Headers\ComponentMNG.h"
#include "Component.h"
IMPLEMENT_SINGLETON(ComponentMNG)
ComponentMNG::ComponentMNG()
{
}

Component * ComponentMNG::FindProto(_uint scene, const _tchar * protoTag)
{
	if (numScene <= scene)
		return nullptr;

	auto iter = find_if(prototypes[scene].begin(), prototypes[scene].end(), TagFinder(protoTag));//�ӽð�ü�� �������̵��� �Լ���ü�� ���� �����ڸ� ȣ��. �ӽð�ü�� �����ڶ� ���ο� ��ü�� �����ȴ�
	if (iter == prototypes[scene].end())
		return nullptr;

	return iter->second;
}

HRESULT ComponentMNG::LateInit()
{
	return NOERROR;
}

Component * ComponentMNG::GetComponent(_uint scene, const _tchar * protoTag)
{
	if (numScene <= scene)
		return nullptr;

	auto iter = find_if(prototypes[scene].begin(), prototypes[scene].end(), TagFinder(protoTag));
	if (iter == prototypes[scene].end())
		return nullptr;

	return iter->second;
}

HRESULT ComponentMNG::Reserve(_uint numScene)
{
	if (prototypes != nullptr)
		return E_FAIL;

	prototypes = new PROTOTYPE[numScene];
	this->numScene = numScene;
	return NOERROR;
}

HRESULT ComponentMNG::AddPrototype(_uint scene, const _tchar * prototypeTag, Component * component)
{
	if (component == nullptr)
		return E_FAIL;
	Component* proto = FindProto(scene, prototypeTag);
	if (proto != nullptr)
		return E_FAIL;
	prototypes[scene].insert(PROTOTYPE::value_type(prototypeTag, component));
	return NOERROR;
}

HRESULT ComponentMNG::LateRender()
{
	return NOERROR;
}

Component * ComponentMNG::Clone(_uint scene, const _tchar * protoTag, void * argument)
{
	Component* proto = FindProto(scene, protoTag);
	if (proto == nullptr)
		return nullptr;

	Component* component = proto->Clone(argument);
	if (component == nullptr)
		return nullptr;
	return component;
}

HRESULT ComponentMNG::ClearScene(_uint scene)
{
	if (numScene <= scene)
		return E_FAIL;

	for (auto& pair : prototypes[scene])
		SafeRelease(pair.second);

	prototypes[scene].clear();
	return NOERROR;
}

void ComponentMNG::Free()
{
	for (size_t i = 0; i < numScene; i++)
	{
		for (auto& pair : prototypes[i])
		{
			SafeRelease(pair.second);
		}

		prototypes[i].clear();
	}
	SafeDelete_Arr(prototypes);
}
