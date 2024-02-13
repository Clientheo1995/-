#pragma once
#include "Live.h"

BEGIN(Engine)
class Navigation;
class ENGINE_DLL Monster :
	public Live
{
protected:
	enum MONSTERSTATE { PEACE, AGRO, READY, ALERT, WALK, ROAMING, BACKSTEP, PARRY, ATTACK1, ATTACK2, ATTACK3, ATTACK4, ATTACK5, ATTACK6, FIGHT, DAMAGED, DEAD };



	MONSTERSTATE state, oldState;
	Navigation* naviComponent = nullptr;
	Collider* agroCollider = nullptr;

	const _matrix* playerMat = nullptr;

	explicit Monster(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Monster(const Monster& gameobject);
	virtual ~Monster() = default;

	HRESULT AddComponent();
	HRESULT SetupOnTerrain();//Y축 지형타기
	_bool CheckPlayer(_float dist = 2.f);
public:
	virtual HRESULT LateInit();
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render();

	virtual HRESULT Parsing();

	static Monster* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual GameObject* Clone(void* argument);
	virtual void Free();
};

END