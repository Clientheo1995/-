#pragma once
#include "Base.h"
BEGIN(Engine)
class HierarchyLoader final :
	public ID3DXAllocateHierarchy, public Base
{
private:
	LPDIRECT3DDEVICE9 graphicDev = nullptr;
	const _tchar* filePath = nullptr;


	explicit HierarchyLoader(LPDIRECT3DDEVICE9 graphicDevice);
	virtual ~HierarchyLoader() = default;

	HRESULT DynamicName(char** sourName, const char* destName);
	HRESULT SetTextureFile(_tchar* out, _tchar* in, const _tchar* sourText, const _tchar * destText);
public:
	HRESULT Ready(const _tchar* filePath);
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,	LPD3DXFRAME *ppNewFrame) ;
	STDMETHOD(CreateMeshContainer)(THIS_	LPCSTR Name,	CONST D3DXMESHDATA *pMeshData,	CONST D3DXMATERIAL *pMaterials,	CONST D3DXEFFECTINSTANCE *pEffectInstances,	DWORD NumMaterials,	CONST DWORD *pAdjacency,	LPD3DXSKININFO pSkinInfo,	LPD3DXMESHCONTAINER *ppNewMeshContainer) ;
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) ;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) ;

	static HierarchyLoader* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * filePath);
	virtual void Free();
};

END