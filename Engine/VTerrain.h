#pragma once
#include "VertexBuffer.h"
BEGIN(Engine)
class ENGINE_DLL VTerrain final:
public VertexBuffer
{
private:
	_uint terrainX = 0;
	_uint terrainZ = 0;//하이트맵이라서
	_float interval = 0.f;
	explicit VTerrain(LPDIRECT3DDEVICE9 graphicDevice);
	explicit VTerrain(const VTerrain& vB);
	virtual ~VTerrain() = default;
public:
	virtual HRESULT ReadyProto(_uint verX, _uint verZ, _float interval);//interval 정점사이의 간격
	virtual HRESULT ReadyProto(const _tchar* heightMapPath, _float interval);
	virtual HRESULT Ready(void* argument);
	HRESULT Render();
	static VTerrain* Create(LPDIRECT3DDEVICE9 graphicDevice, _uint verX, _uint verZ, _float interval);
	static VTerrain* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* heightMapPath, _float interval);
	virtual Component* Clone(void* argument);

	virtual _bool Picking(const Transform* transform, _vec3* point)const;

	//선형보간법으로 현재 폴리곤의 y축 구하기
	_float CalculateY(_vec3 position)const;

	virtual void Free();
};

END