#pragma once
#include "Component.h"
#include "Navigation.h"
BEGIN(Engine)
class ENGINE_DLL Transform final :
public Component
{

public:
	typedef struct tagStateOption
	{
		_float speed;
		_float rotation;
	}STATEOPTION;
	enum STATE { RIGHT, UP, LOOK, POSITION, LAST };

	typedef struct tagMatrixOption
	{
		_vec3 right, up, look;
	}MATRIXOPTION;
private:
	_matrix worldMat;
	STATEOPTION stateOP;
	explicit Transform(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Transform(const Transform& component);
	virtual ~Transform() = default;

public:

	//Getter
	_vec3 GetState(STATE state)const { return *(_vec3*)&worldMat.m[state][0]; }
	_float GetScale(STATE state)const { return D3DXVec3Length((_vec3*)&worldMat.m[state][0]); }
	_matrix GetWorldMatInverse()const;
	_matrix GetWorld() const { return worldMat; }
	_matrix* GetWorldMatPointer() { return &worldMat; }

	//Setter
	void SetState(STATE state, _vec3 data) { memcpy(&worldMat.m[state][0], &data, sizeof(_vec3)); }
	void SetStateOption(STATEOPTION stateop);
	void SetRotate(const _vec3* axis, _float radian);
	void SetScale(_vec3 scale);//내가 지정한 사이즈로 변형

	void SetSpeed(const _float speed) { stateOP.speed = speed; };
	void SetY(const _float y);
	HRESULT SetLook(const _vec3* look);

	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);

	HRESULT Forward(_double timeDelta);
	HRESULT Forward(Navigation* navi,_double timeDelta);
	HRESULT Forward(Navigation* navi, _double timeDelta, _matrix camMat);

	HRESULT Back(_double timeDelta);
	HRESULT Back(Navigation* navi, _double timeDelta);
	HRESULT Back(Navigation* navi, _double timeDelta, _matrix camMat);

	HRESULT Left(_double timeDelta);
	HRESULT Left(Navigation* navi, _double timeDelta);
	HRESULT Left(Navigation* navi, _double timeDelta, _matrix camMat);

	HRESULT Right(_double timeDelta);
	HRESULT Right(Navigation* navi, _double timeDelta);
	HRESULT Right(Navigation* navi, _double timeDelta, _matrix camMat);

	HRESULT RotateAxis(const _vec3* axis,_double timeDelta);
	HRESULT RotateY(const _vec3* axis, _double timeDelta);
	HRESULT RotateX(const _vec3* axis, _double timeDelta);

	_float Parsing(_vec3 targetPos, _double timeDelta);

	static Transform* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual Component* Clone(void* argument);
	virtual void Free();

};

END