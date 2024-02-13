#pragma once
#include "Component.h"
BEGIN(Engine)
class Cell;
class ENGINE_DLL Navigation final:public Component
{
private:
	_uint celCount = 0;
	_uint currentIndex = 0;

	vector<Cell*> cells;
	typedef vector<Cell*> CELLS;

	explicit Navigation(LPDIRECT3DDEVICE9 graphicDevice);
explicit Navigation(const Navigation& gameObject);
virtual ~Navigation() = default;
HRESULT SetNeighbor();
public:
HRESULT SetFirstPos(_vec3 pos);
	HRESULT CalculateY(_vec3* position)const;
	_bool CheckMove(const _vec3* result);
	virtual HRESULT ReadyProto(const _tchar* filePath);
	virtual HRESULT Ready(void* argument);

	static Navigation* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* filePath);
	virtual Component* Clone(void* argument);
	virtual void Free();

#ifdef _DEBUG
	HRESULT Render();
#endif
};

END