#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Live.h"
#include "Monster.h"
#include "Layer.h"
#include "Camera.h"
#include "Item.h"
#include "UI.h"
#include "Effect.h"

//������Ÿ�� ���
BEGIN(Engine)
class Layer;
class ObjectManager :
	public Base
{
	DECLARE_SINGLETON(ObjectManager)
private:
	_uint numScene;

	unordered_map<const _tchar*, GameObject*> prototypes;
	typedef unordered_map<const _tchar*, GameObject*> PROTOTYPE;

	explicit ObjectManager();
	virtual ~ObjectManager()=default;
	//���̾�
	unordered_map<const _tchar*, Layer*>* layers=nullptr;
	typedef unordered_map<const _tchar*, Layer*> LAYER;

	GameObject* FindPrototype(const _tchar* prototypeTag);
	Layer* FindLayer(_uint sceneIndex, const _tchar* layerTag);

public:
	HRESULT LateInit();
	HRESULT Reserve(_uint numScene);
	HRESULT AddPrototype(const _tchar* prototypeTag, GameObject* gameObject);
	HRESULT AddGameObject(const _tchar* prototypeTag, GameObject** gameObject, void* argument = nullptr);
	//���̵� �����ͷ� �޾ƾ� ���� ���� �� �ִ�
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, void* argument);
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, GameObject** gameObject, void* argument);
	//const GameObject* GetGameObject(const _tchar * componentTag, const _tchar * layerTag, _uint sceneIndex);
	const Component* GetComponent(_uint scene, const _tchar * layerTag, const _tchar* componentTag, _uint index=0);
	Layer* GetLayer( _uint sceneIndex, const _tchar* layerTag);

	_int Update(_double timeDelta);
	_int LateUpdate(_double timeDelta);
	HRESULT ClearScene(_uint scene);

	virtual void Free();
};


END
