#include "ObjectManager.h"
#include "Layer.h"
IMPLEMENT_SINGLETON(ObjectManager)
ObjectManager::ObjectManager()
{
}

GameObject * ObjectManager::FindPrototype(const _tchar * prototypeTag)
{
	auto iter=find_if(prototypes.begin(), prototypes.end(), TagFinder(prototypeTag));//임시객체의 오버라이딩한 함수객체가 들어가서 생성자를 호출. 임시객체의 생성자라서 새로운 객체가 생성된다
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
	//객체의 초기형태(데이터 포함)를 저장해둔다
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

		//레이어에 객체를 추가한다
		layer->AddGameObject(gameObject);
		//준비된 레이어를 맵에 추가한다
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

		//레이어에 객체를 추가한다
		layer->AddGameObject(game);
		//준비된 레이어를 맵에 추가한다
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

	//찾는 레이어보다 이쪽의 레이어 갯수가 적거나, 레이어 리스트가 비어있다면 리턴
	if (numScene <= scene || layers == nullptr)
		return nullptr;

	Layer* layer = FindLayer(scene, layerTag);


	//찾는 레이어가 없다면 리턴
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
