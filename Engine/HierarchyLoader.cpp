#include "..\Headers\HierarchyLoader.h"

HierarchyLoader::HierarchyLoader(LPDIRECT3DDEVICE9 graphicDevice) :graphicDev(graphicDevice)
{
	AddRefCount(graphicDev);
}

HRESULT HierarchyLoader::DynamicName(char ** sourName, const char * destName)
{
	size_t length = strlen(destName);

	*sourName = new char[length + 1];
	ZeroMemory(*sourName, sizeof(char)*length + 1);

	strcpy(*sourName, destName);

	return NOERROR;
}

HRESULT HierarchyLoader::SetTextureFile(_tchar * out, _tchar * in, const _tchar * sourText, const _tchar * destText)
{
	_uint length = lstrlen(in);
	lstrcpy(out, in);

	//뒤에서부터 .을 찾고 해당 글자를 찾는다
	for (size_t i = length - 4; i >= 0; i--)//엥간하면 확장자는 3글자니까
	{
		if (in[i] == *sourText)
		{
			out[i] = *destText;
			break;
		}
	}
	return NOERROR;
}

HRESULT HierarchyLoader::Ready(const _tchar* filePath)
{
	this->filePath = filePath;
	return NOERROR;
}

STDMETHODIMP HierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED* derivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(derivedFrame, sizeof(D3DXFRAME_DERIVED));

	if (Name != nullptr)
		DynamicName(&derivedFrame->Name, Name);

	derivedFrame->combinedTransform = *D3DXMatrixIdentity(&derivedFrame->TransformationMatrix);

	*ppNewFrame = derivedFrame;
	return NOERROR;
}

STDMETHODIMP HierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED* meshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(meshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));


	if (Name != nullptr)
		DynamicName(&meshContainer->Name, Name);

	meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH mesh = pMeshData->pMesh;
	AddRefCount(mesh);//주소를 공유해서 함부로 지우지 못하도록(나중에 또 써야한다)

	if (FAILED(mesh->CloneMeshFVF(mesh->GetOptions(), mesh->GetFVF()/*| D3DFVF_NORMAL*/, graphicDev, &meshContainer->MeshData.pMesh)))
	return E_FAIL;



	//이렇게 직접 할당을 해줘도 된다
	/*D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE] = {
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
	};

	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), Decl, m_pGraphic_Device, &pMeshContainer->MeshData.pMesh)))
	return E_FAIL;*/

	SafeRelease(mesh);

	meshContainer->NumMaterials = NumMaterials;


	//이건 깊은복사로 아예 복사를 해두는것
	meshContainer->pMaterials = new D3DXMATERIAL[meshContainer->NumMaterials];
	memcpy(meshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)*meshContainer->NumMaterials);

	meshContainer->meshTexture = new MESHTEXTURE[meshContainer->NumMaterials];
	ZeroMemory(meshContainer->meshTexture, sizeof(MESHTEXTURE)* meshContainer->NumMaterials);


	_tchar fullPath[MAX_PATH] = L"";
	for (size_t i = 0; i < meshContainer->NumMaterials; i++)
	{
		lstrcpy(fullPath, filePath);

		_tchar fileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, meshContainer->pMaterials[i].pTextureFilename, strlen(meshContainer->pMaterials[i].pTextureFilename), fileName, MAX_PATH);

		lstrcat(fullPath, fileName);

		_tchar _out[MAX_PATH] = L"";


		//diffuse map
		SetTextureFile(_out, fullPath, L"D", L"D");
		D3DXCreateTextureFromFile(graphicDev, _out, &meshContainer->meshTexture[i].texture[MESHTEXTURE::DIFFUSE]);

		//normal map
		SetTextureFile(_out, fullPath, L"D", L"N");
		D3DXCreateTextureFromFile(graphicDev, _out, &meshContainer->meshTexture[i].texture[MESHTEXTURE::NORMAL]);

		//specular map
		SetTextureFile(_out, fullPath, L"D", L"S");
		D3DXCreateTextureFromFile(graphicDev, _out, &meshContainer->meshTexture[i].texture[MESHTEXTURE::SPECULAR]);

		//emissive map
		SetTextureFile(_out, fullPath, L"D", L"E");
		D3DXCreateTextureFromFile(graphicDev, _out, &meshContainer->meshTexture[i].texture[MESHTEXTURE::EMISSIVE]);
	}

	meshContainer->pAdjacency = new _ulong[meshContainer->MeshData.pMesh->GetNumFaces() * 3];
	memcpy(meshContainer->pAdjacency, pAdjacency, sizeof(_ulong)*meshContainer->MeshData.pMesh->GetNumFaces() * 3);

	meshContainer->pSkinInfo = pSkinInfo;
	AddRefCount(meshContainer->pSkinInfo);

	meshContainer->numFrames = meshContainer->pSkinInfo->GetNumBones();

	meshContainer->offsetMatrices = new _matrix[meshContainer->numFrames];

	for (_ulong i = 0; i < meshContainer->numFrames; i++)
	{
		meshContainer->offsetMatrices[i] = *meshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	meshContainer->combinedTransformMatrices = new _matrix*[meshContainer->numFrames];
	meshContainer->renderingMatrices = new _matrix[meshContainer->numFrames];
	if (FAILED(meshContainer->MeshData.pMesh->CloneMeshFVF(meshContainer->MeshData.pMesh->GetOptions(), meshContainer->MeshData.pMesh->GetFVF(), graphicDev, &meshContainer->originalMesh)))
		return E_FAIL;
	*ppNewMeshContainer = meshContainer;
	return NOERROR;
}

STDMETHODIMP HierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SafeDelete_Arr(pFrameToFree->Name);
	if (pFrameToFree->pMeshContainer != nullptr)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	if (pFrameToFree->pFrameFirstChild != nullptr)
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	if (pFrameToFree->pFrameSibling != nullptr)
		DestroyFrame(pFrameToFree->pFrameSibling);

	SafeDelete(pFrameToFree);
	return NOERROR;
}

STDMETHODIMP HierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED* meshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	SafeDelete_Arr(meshContainer->Name);
	SafeDelete_Arr(meshContainer->pMaterials);
	SafeDelete_Arr(meshContainer->pAdjacency);
	SafeDelete_Arr(meshContainer->offsetMatrices);
	SafeDelete_Arr(meshContainer->combinedTransformMatrices);
	SafeDelete_Arr(meshContainer->renderingMatrices);
	SafeRelease(meshContainer->MeshData.pMesh);
	SafeRelease(meshContainer->pSkinInfo);
	SafeRelease(meshContainer->originalMesh);

	for (size_t i = 0; i < meshContainer->NumMaterials; i++)
	{
		for (size_t j = 0; j < MESHTEXTURE::LAST; j++)
		{
			SafeRelease(meshContainer->meshTexture[i].texture[j]);
		}
	}

	SafeDelete_Arr(meshContainer->meshTexture);
	SafeDelete(meshContainer);
	return NOERROR;
}

HierarchyLoader * HierarchyLoader::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * filePath)
{
	HierarchyLoader* instance = new HierarchyLoader(graphicDevice);
	if (FAILED(instance->Ready(filePath)))
	{
		MSG("Failed Create HierarchyLoader");
		SafeRelease(instance);
	}
	return instance;
}

void HierarchyLoader::Free()
{
	SafeRelease(graphicDev);
}
