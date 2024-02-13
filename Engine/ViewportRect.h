#pragma once
#include "VertexBuffer.h"
BEGIN(Engine)
class ENGINE_DLL ViewportRect final :
public VertexBuffer
{
private:
	LPDIRECT3DVERTEXDECLARATION9 declare = nullptr;

	explicit ViewportRect(LPDIRECT3DDEVICE9 graphicDevice);
	explicit ViewportRect(const ViewportRect& vB);
	virtual ~ViewportRect() = default;
public:
	virtual HRESULT ReadyProto(_float startX, _float startY, _float sizeX, _float sizeY);
	virtual HRESULT Ready(void* argument);

	HRESULT Render();

	static ViewportRect* Create(LPDIRECT3DDEVICE9 graphicDevice, _float startX, _float startY, _float sizeX, _float sizeY);
	virtual Component* Clone(void* argument);
	virtual void Free();
};

END