#pragma once
#include "Component.h"
BEGIN(Engine)
class Shader;
class PipeLine;
class Transform;
class ENGINE_DLL Collider final:
public Component
{
public:
	typedef struct tagColliderOption
	{
		_matrix* targetMat;//Ʈ������ ������Ʈ�� �����޾ƿ��ԵǸ� �ּҸ� �������°Ŷ� ī��Ʈ, ����������� ����� �� ����. ����ϸ� ����
		_vec3 localScale;
		_vec3 localPos;
		_float radius;//������
		_float diameter;//����
	}COLLIDEROPTION;

	typedef struct tagOBBOption
	{
		_vec3 projAxis[3];
		_vec3 alignAxis[3];
		_vec3 centerPos;
	}OBBOPTION;
	enum COLLIDERTYPE { AABB, OBB, SPHERE, LAST };

private:
	LPD3DXMESH mesh = nullptr;
	COLLIDERTYPE type;
	COLLIDEROPTION colliderOP;

	Shader* shader = nullptr;
	Transform* transform = nullptr;//������Ķ����� ������
	PipeLine* pipeLine = nullptr;

	//_bool damaged = false;
	//_bool invince = false;
	_bool collision = false;
	_bool attack = false;
	_vec3 min, max;
	OBBOPTION* obbOP = nullptr;

	explicit Collider(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Collider(const Collider& col);
	virtual ~Collider() = default;

	HRESULT ReadyBox();
	HRESULT ReadySphere();

public:
//#ifdef _DEBUG
	HRESULT Render();
//#endif

	HRESULT GetSuperWorld(_matrix* mat);
	_bool AABBCollision(Collider* target, _bool checkSource, _bool checkDest);
	_bool OBBCollision(Collider* target, _bool checkSource, _bool checkDest);
	_bool SphereCollision(Collider* target);
	_bool GetItem(Collider* target);
	_bool SphereCollision_Invince(Collider* target);

	void SetCollision(_bool collision) { this->collision = collision; }
	void SetAttack(_bool attack) { this->attack = attack; }
	_int Update(_double timeDelta);
	_bool IsCollision() { return collision; };
	virtual HRESULT ReadyProto(COLLIDERTYPE type);
	virtual HRESULT Ready(void* argument);
	static Collider* Create(LPDIRECT3DDEVICE9 graphicDevice, COLLIDERTYPE type);
	virtual Component* Clone(void* argument);
	virtual void Free();
};
END
