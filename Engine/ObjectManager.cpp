#include "ObjectManager.h"
#include "Layer.h"
IMPLEMENT_SINGLETON(ObjectManager)
ObjectManager::ObjectManager()
{
}

GameObject * ObjectManager::FindPrototype(const _tchar * prototypeTag)
{
	auto iter=find_if(prototypes.begin(), prototypes.end(), TagFinder(prototypeTag));//�ӽð�ü�� �������̵��� �Լ���ü�� ���� �����ڸ� ȣ��. �ӽð�ü�� �����ڶ� ���ο� ��ü�� �����ȴ�
	if(iter==prototypes.end())
	return nullptr;

	return iter->second;
}

Layer * ObjectManager::FindLayer(_uint sceneIndex, const _tchar * layerTag)
{
	if (numScene <= sceneIndex)
		return nullptr;
	auto iter = find_if(layers[sceneIndex].begin(), layers[sceneIndex].end(), TagFinder(layerTag));
	if (iter == layers[sceneIndex].end())
		return nullptr;

	return iter->second;
}

HRESULT ObjectManager::LateInit()
{
	for (size_t i = 0; i < numScene; i++)
	{
		for (auto& pair : layers[i])
		{
			if (pair.second != nullptr)
				pair.second->LateInit();
		}
	}
	return NOERROR;
}

HRESULT ObjectManager::Reserve(_uint numScene)
{
	if (layers != nullptr)
		return E_FAIL;

	layers = new LAYER[numScene];
	this->numScene = numScene;
	return NOERROR;
}

HRESULT ObjectManager::AddPrototype(const _tchar * prototypeTag, GameObject * gameObject)
{
	//��ü�� �ʱ�����(������ ����)�� �����صд�
	if (gameObject == nullptr)
		return E_FAIL;
	if (FindPrototype(prototypeTag) != nullptr)
		return E_FAIL;
	prototypes.insert(PROTOTYPE::value_type(prototypeTag, gameObject));
	return NOERROR;
}

HRESULT ObjectManager::AddGameObject(const _tchar * prototypeTag, GameObject** gameObject, void * argument)
{
	GameObject* proto = FindPrototype(prototypeTag);

	if (proto == nullptr)
		return E_FAIL;

	GameObject* game= proto->Clone(argument);
	if (game == nullptr)
		return E_FAIL;
	*gameObject = game;
	return NOERROR;
}

HRESULT ObjectManager::GameObjectToLayer(const _tchar * prototypeTag, _uint sceneIndex, const _tchar * layerTag, void * argument)
{
	GameObject* proto = FindPrototype(prototypeTag);

	if (proto == nullptr)
		return E_FAIL;

	GameObject* gameObject = proto->Clone(argument);
	if (gameObject == nullptr)
		return E_FAIL;

	Layer* layer = FindLayer(sceneIndex, layerTag);

	if (layer == nullptr)
	{
		layer = Layer::Create();
		if (layer == nullptr)
		return E_FAIL;

		//���̾ ��ü�� �߰��Ѵ�
		layer->AddGameObject(gameObject);
		//�غ�� ���̾ �ʿ� �߰��Ѵ�
		layers[sceneIndex].insert(LAYER::value_type(layerTag, layer));
	}
	else
		layer->AddGameObject(gameObject);
	return NOERROR;
}

HRESULT ObjectManager::GameObjectToLayer(const _tchar * prototypeTag, _uint sceneIndex, const _tchar * layerTag, GameObject ** gameObject, void * argument)
{
	GameObject* proto = FindPrototype(prototypeTag);

	if (proto == nullptr)
		return E_FAIL;

	GameObject* game = proto->Clone(argument);
	if (game == nullptr)
		return E_FAIL;

	*gameObject = game;

	Layer* layer = FindLayer(sceneIndex, layerTag);

	if (layer == nullptr)
	{
		layer = Layer::Create();
		if (layer == nullptr)
			return E_FAIL;

		//���̾ ��ü�� �߰��Ѵ�
		layer->AddGameObject(game);
		//�غ�� ���̾ �ʿ� �߰��Ѵ�
		layers[sceneIndex].insert(LAYER::value_type(layerTag, layer));
	}
	else
		layer->AddGameObject(game);
	return NOERROR;
}

//const GameObject * ObjectManager::GetGameObject(const _tchar * componentTag,const _tchar * layerTag, _uint sceneIndex)
//{
//	if (numScene <= sceneIndex || layers == nullptr)
//		return nullptr;
//	Layer* layer = FindLayer(sceneIndex, layerTag);
//
//	if (layer == nullptr)
//		return nullptr;
//
//	return layer->GetGameObject(componentTag, sceneIndex);
//}

const Component * ObjectManager::GetComponent(_uint scene, const _tchar * layerTag, const _tchar * componentTag, _uint index)
{

	//ã�� ���̾�� ������ ���̾� ������ ���ų�, ���̾� ����Ʈ�� ����ִٸ� ����
	if (numScene <= scene || layers == nullptr)
		return nullptr;

	Layer* layer = FindLayer(scene, layerTag);


	//ã�� ���̾ ���ٸ� ����
	if (layer == nullptr)
		return nullptr;

	return layer->GetComponent(componentTag, index);
}

Layer * ObjectManager::GetLayer(_uint sceneIndex, const _tchar * layerTag)
{
	if (numScene <= sceneIndex || layers == nullptr)
		return nullptr;
	Layer* layer = FindLayer(sceneIndex, layerTag);

	if (layer == nullptr)
		return nullptr;
	return layer;
}

_int ObjectManager::Update(_double timeDelta)
{
	for (size_t i = 0; i < numScene; i++)
	{
		for (auto& pair : layers[i])
		{
			if (pair.second != nullptr)
				pair.second->Update(timeDelta);
		}
	}
	return _int();
}

_int ObjectManager::LateUpdate(_double timeDelta)
{
	for (size_t i = 0; i < numScene; i++)
	{
		for (auto& pair : layers[i])
		{
			if (pair.second != nullptr)
				pair.second->LateUpdate(timeDelta);
		}
	}
	return _int();
}

HRESULT ObjectManager::ClearScene(_uint scene)
{
	if (numScene <= scene)
		return E_FAIL;
	for (auto& pair : layers[scene])
		SafeRelease(pair.second);
		layers[scene].clear();
	return NOERROR;
}

void ObjectManager::Free()
{
	for (auto& pair : prototypes)
	{
		SafeRelease(pair.second);
	}

	prototypes.clear();

	for (size_t i = 0; i < numScene; i++)
	{
		for (auto& pair : layers[i])
		{
			SafeRelease(pair.second);
		}

		layers[i].clear();
	}
	SafeDelete_Arr(layers);
}
