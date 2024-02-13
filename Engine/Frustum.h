#pragma once
#include "Component.h"
#include "Transform.h"
#include "PipeLine.h"

BEGIN(Engine)
class ENGINE_DLL Frustum final:public Component
{
private:
	_vec3 projPoint[8];//투영스페이스 상의 절두체의 정점
	PipeLine* pipeLine = nullptr;
	D3DXPLANE plane[6];//절두체의 평면 6개

	explicit Frustum(LPDIRECT3DDEVICE9 graphicDevice);
	virtual ~Frustum() = default;

public:
	_bool FrustumCulling(_matrix* mat, _float radius=0.f);
	_bool FrustumCulling(Transform* transform, _float radius = 0.f);
	_bool InFrustum(_vec3 pointInLocal, _float radius);

	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);

	static Frustum* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual Component* Clone(void* argument);
	virtual void Free();
};

END