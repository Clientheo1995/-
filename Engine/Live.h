#pragma once
#include "GameObject.h"

BEGIN(Engine)
class Shader;
class Texture;
class Renderer;
class Transform;
class VRect;
class VTerrain;
class DynamicMesh;
class Navigation;
class Manager;
class Collider;
class UI;
class ENGINE_DLL Live abstract:
public GameObject
{
protected:
	Shader* shader = nullptr;
	Renderer* render = nullptr;
	Transform* transform = nullptr;
	DynamicMesh* dMesh = nullptr;
	Manager* manager = nullptr;
	VTerrain* terrain = nullptr;
	list<Collider*> colliders;
	Collider* bodyColl = nullptr;
	//콜라이더 각각 관리해주기위해서(무적 등) 따로따로 선언해두는것이 필요한것 같다
	Collider* weaponColl = nullptr;

	_int soundCount;
	_bool isMoving = false;
	_vec3 targetPos;
	_double timeDelta;
	STATUS stat;

	explicit Live(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Live(const Live& gameobject);
	virtual ~Live() = default;

	HRESULT AddComponent();
	HRESULT SetupConstantTable();
	HRESULT SetupOnTerrain();//Y축 지형타기
public:
	virtual HRESULT LateInit();
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render();

	virtual _matrix RemoveScale(_matrix* mat);
	const STATUS GetStatus() { return stat; };
	virtual HRESULT Parsing();

	static Live* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual GameObject* Clone(void* argument);
	virtual void Free();
};

END