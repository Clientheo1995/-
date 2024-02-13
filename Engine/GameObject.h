#pragma once
#include "Base.h"

BEGIN(Engine)
class Component;
class ENGINE_DLL GameObject abstract:public Base
{
protected:
	_bool isRender = true;
	explicit GameObject(LPDIRECT3DDEVICE9 graphicDevice);
	explicit GameObject(const GameObject& gameObject);
	virtual ~GameObject() = default;
	HRESULT AddComponent(_uint sceneID, const _tchar* prototypeTag, const _tchar* componentTag, Component** component, void* argument=nullptr);//원형 검색 후 복제, 멤버변수에 대입, 컨테이너에 보관
//리턴을 쓰는것보단 ** 로 보내버리는게 훨씬 편하다
	//프로토태그는 저장되어있는 태그고 컴포태그는 내가 저장할 태그
	HRESULT AddComponent(_uint sceneID, const _tchar* prototypeTag, Component** component, void* argument = nullptr);
	HRESULT SortZ(_vec3 worldPos);

private:
	_float viewZ = 0.f;
	unordered_map<const _tchar*, Component*> components;
	typedef unordered_map<const _tchar*, Component*> COMPONENTS;

	LPDIRECT3DDEVICE9 graphicDevice = nullptr;
	Component* FindComponent(const _tchar* componentTag)const;
public:
	virtual HRESULT LateInit();
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);

	virtual void SetRender(_bool isRender) { this->isRender = isRender; }
	const Component* GetComponent(const _tchar* componentTag) const;
	_float GetViewZ()const { return viewZ; };
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render();
	//virtual HRESULT LateRender();
	virtual GameObject* Clone(void* argument) = 0;
	virtual void Free();
	LPDIRECT3DDEVICE9 GetGraphic() const {
		return graphicDevice;
	}
};

END