#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL StaticMesh final:
public Component
{
private:
	LPD3DXMESH mesh = nullptr;
	LPD3DXBUFFER adjacencyBuffer = nullptr;
	LPD3DXBUFFER materialBuffer = nullptr;
	_ulong materialCount = 0;

	D3DXMATERIAL* materials = nullptr;
	MESHTEXTURE* meshTexture = nullptr;

	explicit StaticMesh(LPDIRECT3DDEVICE9 graphicDevice);
	explicit StaticMesh(const StaticMesh& meshs);
	virtual ~StaticMesh() = default;

	HRESULT SetTextureFile(_tchar* out, _tchar* in, const _tchar* sourText, const _tchar * destText);
public:
	virtual HRESULT ReadyProto(const _tchar* filePath, const _tchar* fileName, const _matrix* localMatrix,
		_bool diffuse = true, _bool normal = true, _bool specular = true, _bool emissive = true);
	virtual HRESULT Ready(void* argument);
	HRESULT Render(_ulong subsetIndex);

	_ulong GetSubSetCount()const { return materialCount; }
	LPDIRECT3DTEXTURE9 GetMaterialTexture(_ulong subsetIndex, MESHTEXTURE::TYPE type)const;
	D3DMATERIAL9 GetMaterial(_ulong subsetIndex)const { return materials[subsetIndex].MatD3D; }
	static StaticMesh* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* filePath, const _tchar* fileName, const _matrix* localMatrix,
		_bool diffuse = true, _bool normal = true, _bool specular = true, _bool emissive = true);
	virtual Component* Clone(void* argument);
	virtual void Free();
};
END
