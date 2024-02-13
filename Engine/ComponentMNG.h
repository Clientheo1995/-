#pragma once
#include "Base.h"
#include "Shader.h"
#include "Frustum.h"
#include "Texture.h"
#include "Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "VRect.h"
#include "VTerrain.h"
#include "VCube.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Text.h"
BEGIN(Engine)
class ComponentMNG :
	public Base
{
	DECLARE_SINGLETON(ComponentMNG)
private:
	_uint numScene;

	unordered_map<const _tchar*, Component*>* prototypes =nullptr;
	typedef unordered_map<const _tchar*, Component*> PROTOTYPE;

	explicit ComponentMNG();
	virtual ~ComponentMNG()=default;
	Component* FindProto(_uint scene, const _tchar* protoTag);
public:
	virtual HRESULT LateInit();
	Component* GetComponent(_uint scene, const _tchar* protoTag);
	HRESULT Reserve(_uint numScene);
	HRESULT AddPrototype(_uint scene, const _tchar* prototypeTag, Component* component);
	HRESULT LateRender();
	Component* Clone(_uint scene, const _tchar* protoTag,void* argument);
	HRESULT ClearScene(_uint scene);
	virtual void Free();
};


END
