#include "..\Headers\DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationControl.h"

DynamicMesh::DynamicMesh(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
	D3DXMatrixIdentity(&localMat);
}

DynamicMesh::DynamicMesh(const DynamicMesh & meshs) : Component(meshs),
rootFrame(meshs.rootFrame),
meshContainers(meshs.meshContainers),
loader(meshs.loader),
localMat(meshs.localMat),
originMat(meshs.originMat),
aniControl(meshs.aniControl->Clone())
{
	//AddRefCount(aniControl);
	AddRefCount(loader);
}

HRESULT DynamicMesh::UpdateTransform(D3DXFRAME_DERIVED * frame, _matrix parent)
{
	frame->combinedTransform = frame->TransformationMatrix*parent;

	if (frame->pFrameFirstChild != nullptr)
		UpdateTransform((D3DXFRAME_DERIVED*)frame->pFrameFirstChild,frame->combinedTransform);

	if (frame->pFrameSibling != nullptr)
		UpdateTransform((D3DXFRAME_DERIVED*)frame->pFrameSibling,parent);

	return NOERROR;
}


//메시컨테이너 안의 combinedTransformMatrices 배열에 뼈의 컴바인드 행렬의 주소를 담는다
HRESULT DynamicMesh::SetTransformPointer(D3DXFRAME_DERIVED * frame)
{
	if (frame->pMeshContainer != nullptr)
	{
		D3DXMESHCONTAINER_DERIVED* meshCon = (D3DXMESHCONTAINER_DERIVED*)frame->pMeshContainer;
		for (_ulong i = 0; i < meshCon->numFrames; i++)
		{
			//우리는 모르는 뼈의이름을 가져온다
			D3DXFRAME_DERIVED* findframe = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame, meshCon->pSkinInfo->GetBoneName(i));

			meshCon->combinedTransformMatrices[i] = &findframe->combinedTransform;
		}

		//원본을 미리 등록해두자
		meshContainers.push_back(meshCon);
	}
	if (frame->pFrameFirstChild != nullptr)
		SetTransformPointer((D3DXFRAME_DERIVED*)frame->pFrameFirstChild);
	if (frame->pFrameSibling != nullptr)
		SetTransformPointer((D3DXFRAME_DERIVED*)frame->pFrameSibling);

	return NOERROR;
}

D3DXFRAME_DERIVED * DynamicMesh::CopyFrame(D3DXFRAME_DERIVED * frame)
{
	D3DXFRAME_DERIVED* newFrame = new D3DXFRAME_DERIVED;

	ZeroMemory(newFrame, sizeof(D3DXFRAME_DERIVED));

	if (frame->Name != nullptr)
	{
		size_t length = strlen(frame->Name);
		newFrame->Name = new char[length + 1];
		ZeroMemory(newFrame->Name, sizeof(char)*length + 1);
		strcpy(newFrame->Name, frame->Name);
	}

	if (frame->pFrameFirstChild != nullptr)
		newFrame->pFrameFirstChild = CopyFrame((D3DXFRAME_DERIVED*)frame->pFrameFirstChild);

	if (frame->pFrameSibling != nullptr)
		newFrame->pFrameSibling = CopyFrame((D3DXFRAME_DERIVED*)frame->pFrameSibling);


	return newFrame;
}

HRESULT DynamicMesh::ReadyProto(const _tchar * filePath, const _tchar * fileName, const _matrix * localMatrix)
{
	_tchar fullPath[MAX_PATH] = L"";
	lstrcpy(fullPath, filePath);
	lstrcat(fullPath, fileName);

	loader = HierarchyLoader::Create(GetGraphic(), filePath);
	if (loader == nullptr)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER control = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, GetGraphic(), loader, nullptr, &rootFrame, &control)))
		return E_FAIL;
	if (localMatrix != nullptr)
	{
		localMat = *localMatrix;
		originMat = localMat;
	}
	aniControl = AnimationControl::Create(control);

	if (aniControl == nullptr)
		return E_FAIL;

	if (FAILED(SetTransformPointer((D3DXFRAME_DERIVED*)rootFrame)))
		return E_FAIL;
	return NOERROR;
}

HRESULT DynamicMesh::Ready(void * argument)
{
	//copyFrame = CopyFrame((D3DXFRAME_DERIVED*)rootFrame);

	//LPD3DXFRAME fr = D3DXFrameFind(rootFrame, "R_Hand");

	//fr = D3DXFrameFind(copyFrame, "R_Hand");
	//if (FAILED(UpdateTransform((D3DXFRAME_DERIVED*)rootFrame, (D3DXFRAME_DERIVED*)copyFrame, localMat)))
		//return E_FAIL;
	//if (FAILED(SetTransformPointer((D3DXFRAME_DERIVED*)rootFrame)))
		//return E_FAIL;
	return NOERROR;
}

void DynamicMesh::SetTimeSpeed(_double timeSpeed)
{
	if (aniControl == nullptr)
		return;
	return aniControl->SetTimeSpeed(timeSpeed);
}

HRESULT DynamicMesh::UpdateSkinned(_uint meshContainerIndex)
{
	if (meshContainers.size() <= meshContainerIndex)
		return E_FAIL;
	for (_ulong i = 0; i < meshContainers[meshContainerIndex]->numFrames; i++)
		meshContainers[meshContainerIndex]->renderingMatrices[i] = meshContainers[meshContainerIndex]->offsetMatrices[i] * (*meshContainers[meshContainerIndex]->combinedTransformMatrices[i]);//마지막에 안곱해주면 메쉬가 난리가난다

	void* sourceVer, *destVer;

	//오리진과 출력용 둘 다 락
	meshContainers[meshContainerIndex]->originalMesh->LockVertexBuffer(0, &sourceVer);
	meshContainers[meshContainerIndex]->MeshData.pMesh->LockVertexBuffer(0, &destVer);

	//오리진의 메쉬에 값을 곱하여 출력용에 저장
	meshContainers[meshContainerIndex]->pSkinInfo->UpdateSkinnedMesh(meshContainers[meshContainerIndex]->renderingMatrices, nullptr, sourceVer, destVer);

	meshContainers[meshContainerIndex]->originalMesh->UnlockVertexBuffer();
	meshContainers[meshContainerIndex]->MeshData.pMesh->UnlockVertexBuffer();
	return NOERROR;
}

HRESULT DynamicMesh::Render(_uint meshContainerIndex, _ulong subsetIndex)
{
	if (meshContainers.size() <= meshContainerIndex ||
		meshContainers[meshContainerIndex]->NumMaterials <= subsetIndex)
		return E_FAIL;
	meshContainers[meshContainerIndex]->MeshData.pMesh->DrawSubset(subsetIndex);
	return NOERROR;
}

HRESULT DynamicMesh::AnimationSet(_uint index, _double period)
{
	if (aniControl == nullptr)
		return E_FAIL;
	return aniControl->AnimationSet(index, period);
}

HRESULT DynamicMesh::Play(_double timeDelta)
{
	if (aniControl == nullptr)
		return E_FAIL;
	aniControl->Play(timeDelta);
	if (FAILED(UpdateTransform((D3DXFRAME_DERIVED*)rootFrame,localMat)))
		return E_FAIL;

	return NOERROR;
}

HRESULT DynamicMesh::Stop(_double timeDelta)
{
	if (aniControl == nullptr)
		return E_FAIL;
	aniControl->Stop(timeDelta);
	return NOERROR;
}

_bool DynamicMesh::Finished() const
{
	if (aniControl == nullptr)
		return false;
	return aniControl->Finished();
}

HRESULT DynamicMesh::Rotate(DIRECTION dir, _matrix cameraMat)
{
	_matrix rotate, scale;

	if (direction == dir)
		return NOERROR;
	switch (dir)
	{
	case FRONT:
		//원형에서 회전한 각도인가
		D3DXMatrixRotationY(&rotate, D3DXToRadian(0));//이걸 하고나면 항등행렬이 되니까 다시 곱해준다
		break;
	case BACK:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(180));
		break;
	case LEFT:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(270));
		break;
	case RIGHT:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(90));
		break;
	case LF:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(315));
		break;
	case RF:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(45));
		break;
	case LB:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(225));
		break;
	case RB:
		D3DXMatrixRotationY(&rotate, D3DXToRadian(135));
		break;
	}

	D3DXMatrixScaling(&scale, 0.015f, 0.015f, 0.015f);
	localMat =scale * rotate;
	direction = dir;
	return NOERROR;
}

LPDIRECT3DTEXTURE9 DynamicMesh::GetMaterialTexture(_uint meshConIndex, _ulong subsetIndex, MESHTEXTURE::TYPE type) const
{
	if (meshContainers.size() <= meshConIndex ||
		meshContainers[meshConIndex]->NumMaterials <= subsetIndex ||
		meshContainers[meshConIndex]->meshTexture == nullptr)
		return nullptr;

	return meshContainers[meshConIndex]->meshTexture[subsetIndex].texture[type];
}

DynamicMesh * DynamicMesh::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * filePath, const _tchar * fileName, const _matrix * localMatrix)
{
	DynamicMesh* instance = new DynamicMesh(graphicDevice);
	if (FAILED(instance->ReadyProto(filePath, fileName, localMatrix)))
	{
		MSG("Failed Create DynamicMesh");
		SafeRelease(instance);
	}
	return instance;
}

Component * DynamicMesh::Clone(void * argument)
{
	DynamicMesh* instance = new DynamicMesh(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning DynamicMesh");
		SafeRelease(instance);
	}
	return instance;
}

void DynamicMesh::Free()
{
	if (IsClone() == false)
	{
		loader->DestroyFrame(rootFrame);
	}
	SafeRelease(aniControl);
	SafeRelease(loader);
	Component::Free();
}
