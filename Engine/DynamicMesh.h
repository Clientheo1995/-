#pragma once
#include "Component.h"
BEGIN(Engine)
class AnimationControl;
class HierarchyLoader;
class ENGINE_DLL DynamicMesh final:public Component
{
public:
	enum DIRECTION { FRONT, BACK, LEFT, RIGHT, LF, RF, LB, RB, LAST };

private:
	D3DXFRAME* rootFrame = nullptr;
	D3DXFRAME* copyFrame = nullptr;
	HierarchyLoader* loader = nullptr;
	AnimationControl* aniControl = nullptr;
	_matrix localMat, originMat;

	DIRECTION direction;
	vector<D3DXMESHCONTAINER_DERIVED*> meshContainers;

	explicit DynamicMesh(LPDIRECT3DDEVICE9 graphicDevice);
	explicit DynamicMesh(const DynamicMesh& meshs);
	virtual ~DynamicMesh() = default;

	HRESULT UpdateTransform(D3DXFRAME_DERIVED * frame, _matrix parent);
	HRESULT SetTransformPointer(D3DXFRAME_DERIVED* frame);
	D3DXFRAME_DERIVED* CopyFrame(D3DXFRAME_DERIVED* frame);

public:

	virtual HRESULT ReadyProto(const _tchar* filePath, const _tchar* fileName, const _matrix* localMatrix=nullptr);
	virtual HRESULT Ready(void* argument);

	void SetTimeSpeed(_double timeSpeed);
	HRESULT UpdateSkinned(_uint meshContainerIndex);
	HRESULT Render(_uint meshContainerIndex, _ulong subsetIndex);
	HRESULT AnimationSet(_uint index, _double period=0.0);
	HRESULT Play(_double timeDelta);
	HRESULT Stop(_double timeDelta);
	_bool Finished()const;

	HRESULT Rotate(DIRECTION dir, _matrix cameraMat);
	D3DXFRAME_DERIVED* GetDerivedFrame(const char* frameName) {
		return (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame, frameName);
	}

	_uint GetMeshConSize() const { return meshContainers.size(); }
	_uint GetSubSetCount(_uint meshConIndex) const { return meshContainers[meshConIndex]->NumMaterials; }
	LPDIRECT3DTEXTURE9 GetMaterialTexture(_uint meshConIndex,_ulong subsetIndex, MESHTEXTURE::TYPE type)const;
	D3DMATERIAL9 GetMaterial(_ulong subsetIndex)const { return meshContainers[subsetIndex]->pMaterials[subsetIndex].MatD3D; }

	static DynamicMesh* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* filePath, const _tchar* fileName, const _matrix* localMatrix=nullptr);
	virtual Component* Clone(void* argument);
	virtual void Free();
};
END
