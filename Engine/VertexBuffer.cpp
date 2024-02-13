#include "..\Headers\VertexBuffer.h"
#include "Picking.h"
VertexBuffer::VertexBuffer(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice), vBuffer(nullptr)
{
}

VertexBuffer::VertexBuffer(const VertexBuffer & vB)
	: Component(vB),
	vBuffer(vB.vBuffer),
	stride(vB.stride),
	vertices(vB.vertices),
	fvf(vB.fvf),
	numPrimitive(vB.numPrimitive),
	iBuffer(vB.iBuffer),
	polygonIndicesSize(vB.polygonIndicesSize),
	picking(vB.picking),
	position(vB.position),
	format(vB.format)
{
	AddRefCount(picking);
	AddRefCount(vBuffer);
	AddRefCount(iBuffer);
}

_bool VertexBuffer::Picking(Transform * transform, _vec3 * point)
{
	return _bool();
}

void VertexBuffer::SetPos(_vec3 pos)
{
}

HRESULT VertexBuffer::ReadyProto()
{
	LPDIRECT3DDEVICE9 graphic = GetGraphic();
	if (graphic == nullptr)
		return E_FAIL;

	//피킹정보를 생성
	picking = Picking::Create(graphic);
	if (picking == nullptr)
		return E_FAIL;

	AddRefCount(graphic);

	//정점배열버퍼 생성
	if (FAILED(graphic->CreateVertexBuffer(stride*vertices, 0, fvf, D3DPOOL_MANAGED, &vBuffer, nullptr)))
		return E_FAIL;

	//피킹 포지션 설정(범위)
	position = new _vec3[vertices];
	ZeroMemory(position, sizeof(_vec3)*vertices);

	//인덱스배열버퍼 생성
	if (FAILED(graphic->CreateIndexBuffer(polygonIndicesSize*numPrimitive, 0, format, D3DPOOL_MANAGED, &iBuffer, nullptr)))
		return E_FAIL;

	SafeRelease(graphic);
	return NOERROR;
}

HRESULT VertexBuffer::Ready(void* argument)
{
	return NOERROR;
}

void VertexBuffer::Free()
{
	if (IsClone() == false)
		SafeDelete_Arr(position);
	SafeRelease(picking);
	SafeRelease(vBuffer);
	SafeRelease(iBuffer);
	Component::Free();
}
