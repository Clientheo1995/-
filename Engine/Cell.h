#pragma once
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL Cell final:
public Base
{
public:
	enum POINT { A, B, C, POINTLAST };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINELAST };
	enum NEIGHBOR { AB, BC, CA, NEIGHBORLAST };

private:
	LPDIRECT3DDEVICE9 graphicDev = nullptr;
	Cell* neigh[NEIGHBORLAST];
	_uint index = 0;

	_vec3 point[POINTLAST];
	_vec3 direction[LINELAST];
	_vec3 normal[LINELAST];

#ifdef _DEBUG
	LPD3DXLINE line = nullptr;
#endif

	explicit Cell(LPDIRECT3DDEVICE9 graphicDevice);
	virtual ~Cell() = default;

public:
	_uint GetIndex() const { return index; }
	_vec3 GetPoint(POINT p) { return point[p]; }
	void SetNeighbor(NEIGHBOR type, Cell* cell) {
		neigh[type] = cell;
	}

#ifdef _DEBUG
		HRESULT Render();
#endif

	HRESULT Ready(_vec3 pointa, _vec3 pointb, _vec3 pointc, _uint index);
	_bool ComparePoint(_vec3 sour, _vec3 dest);
	_bool CheckNext(const _vec3* result, Cell** neigh);
	_bool Inside(const _vec3* result, LINE* lineID);
	_bool Inside(_vec3 result);

	static Cell* Create(LPDIRECT3DDEVICE9 graphicDevice,_vec3 pointa, _vec3 pointb, _vec3 pointc, _uint index);
	virtual void Free();
};

END