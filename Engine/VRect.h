#pragma once
#include "VertexBuffer.h"
BEGIN(Engine)
class ENGINE_DLL VRect final :
public VertexBuffer
{
public:
	enum DRAWANCHOR { LEFT_TOP, CENTER, RIGHT_TOP, ANCHOR_END };
private:
	DRAWANCHOR anchor;
	explicit VRect(LPDIRECT3DDEVICE9 graphicDevice);
	explicit VRect(const VRect& vB);
	virtual ~VRect() = default;
public:
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);
	HRESULT Render();
	HRESULT SetAnchor(DRAWANCHOR anc);
	virtual _bool Picking(Transform* transform, _vec3* point);
	static VRect* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual Component* Clone(void* argument);
	virtual void Free();

	virtual void SetPos(_vec3 pos);
};

END