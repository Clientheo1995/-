#pragma once
#include "Component.h"
#include "Transform.h"
#include "Picking.h"

BEGIN(Engine)
class ENGINE_DLL VertexBuffer abstract:
	public Component
{
protected:

	//버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9 vBuffer = nullptr;
	_uint stride = 0;
	_uint vertices = 0;
	_ulong fvf = 0;

	//인덱스 버퍼
	LPDIRECT3DINDEXBUFFER9 iBuffer = nullptr;
	D3DFORMAT format;
	_uint polygonIndicesSize = 0;
	_uint numPrimitive = 0;

	//피킹
	Picking* picking = nullptr;
	_vec3* position = nullptr;

	explicit VertexBuffer(LPDIRECT3DDEVICE9 graphicDevice);
	explicit VertexBuffer(const VertexBuffer& vB);
	virtual ~VertexBuffer() = default;
	
public:
	virtual _bool Picking(Transform* transform, _vec3* point);
	virtual void SetPos(_vec3 pos);

	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);
	virtual Component* Clone(void* argument) = 0;
	virtual void Free();
};

END