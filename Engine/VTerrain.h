#pragma once
#include "VertexBuffer.h"
BEGIN(Engine)
class ENGINE_DLL VTerrain final:
public VertexBuffer
{
private:
	_uint terrainX = 0;
	_uint terrainZ = 0;//����Ʈ���̶�
	_float interval = 0.f;
	explicit VTerrain(LPDIRECT3DDEVICE9 graphicDevice);
	explicit VTerrain(const VTerrain& vB);
	virtual ~VTerrain() = default;
public:
	virtual HRESULT ReadyProto(_uint verX, _uint verZ, _float interval);//interval ���������� ����
	virtual HRESULT ReadyProto(const _tchar* heightMapPath, _float interval);
	virtual HRESULT Ready(void* argument);
	HRESULT Render();
	static VTerrain* Create(LPDIRECT3DDEVICE9 graphicDevice, _uint verX, _uint verZ, _float interval);
	static VTerrain* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* heightMapPath, _float interval);
	virtual Component* Clone(void* argument);

	virtual _bool Picking(const Transform* transform, _vec3* point)const;

	//�������������� ���� �������� y�� ���ϱ�
	_float CalculateY(_vec3 position)const;

	virtual void Free();
};

END