#include "..\Headers\StaticMesh.h"

StaticMesh::StaticMesh(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
}

StaticMesh::StaticMesh(const StaticMesh & meshs) : Component(meshs),
mesh(meshs.mesh),
adjacencyBuffer(meshs.adjacencyBuffer),
materialBuffer(meshs.materialBuffer),
materialCount(meshs.materialCount),
materials(meshs.materials),
meshTexture(meshs.meshTexture)
{
	AddRefCount(mesh);
	AddRefCount(adjacencyBuffer);
	AddRefCount(materialBuffer);

	for (size_t i = 0; i < materialCount; i++)
	{
		for (size_t j = 0; j < MESHTEXTURE::LAST; j++)
		{
			AddRefCount(meshTexture[i].texture[j]);
		}
	}
}

HRESULT StaticMesh::SetTextureFile(_tchar * out, _tchar * in, const _tchar * sourText, const _tchar * destText)
{
	_uint length = lstrlen(in);
	lstrcpy(out, in);

	//뒤에서부터 .을 찾고 해당 글자를 찾는다
	for (size_t i = length - 4; i >= 0; i--)
	{
		if (in[i] == *sourText)
		{
			out[i] = *destText;
		break;
		}
	}
	return NOERROR;
}

HRESULT StaticMesh::ReadyProto(const _tchar * filePath, const _tchar * fileName, const _matrix* localMatrix,
	_bool diffuse, _bool normal, _bool specular, _bool emissive)
{
	_tchar fullPath[MAX_PATH] = L"";
	lstrcpy(fullPath, filePath);
	lstrcat(fullPath, fileName);

	if (FAILED(D3DXLoadMeshFromX(fullPath, D3DXMESH_MANAGED, GetGraphic(),
		&adjacencyBuffer, &materialBuffer, nullptr, &materialCount, &mesh)))
		return E_FAIL;

	//사용할 수 있는 재질정보를 세팅한다
	materials = new D3DXMATERIAL[materialCount];
	memcpy(materials, materialBuffer->GetBufferPointer(), sizeof(D3DXMATERIAL)*materialCount);

	meshTexture = new MESHTEXTURE[materialCount];
	ZeroMemory(meshTexture, sizeof(MESHTEXTURE)*materialCount);

	for (size_t i = 0; i < materialCount; i++)
	{
		lstrcpy(fullPath, filePath);

		_tchar fileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, materials[i].pTextureFilename, strlen(materials[i].pTextureFilename), fileName, MAX_PATH);

		lstrcat(fullPath, fileName);

		_tchar _out[MAX_PATH] = L"";

		if (diffuse)
		{
			//diffuse map
			SetTextureFile(_out, fullPath, L"D", L"D");
			D3DXCreateTextureFromFile(GetGraphic(), _out, &meshTexture[i].texture[MESHTEXTURE::DIFFUSE]);
		}
		else
			D3DXCreateTextureFromFile(GetGraphic(), fullPath, &meshTexture[i].texture[MESHTEXTURE::DIFFUSE]);

		if (normal)
		{
		//normal map
		SetTextureFile(_out, fullPath, L"D", L"N");
		D3DXCreateTextureFromFile(GetGraphic(), _out, &meshTexture[i].texture[MESHTEXTURE::NORMAL]);
		}
		else
			D3DXCreateTextureFromFile(GetGraphic(), fullPath, &meshTexture[i].texture[MESHTEXTURE::NORMAL]);

		if (specular)
		{
		//specular map
		SetTextureFile(_out, fullPath, L"D", L"S");
		D3DXCreateTextureFromFile(GetGraphic(), _out, &meshTexture[i].texture[MESHTEXTURE::SPECULAR]);
		}
		else
			D3DXCreateTextureFromFile(GetGraphic(), fullPath, &meshTexture[i].texture[MESHTEXTURE::SPECULAR]);

		if (emissive)
		{
		//emissive map
		SetTextureFile(_out, fullPath, L"D", L"E");
		D3DXCreateTextureFromFile(GetGraphic(), _out, &meshTexture[i].texture[MESHTEXTURE::EMISSIVE]);
		}
		else
			D3DXCreateTextureFromFile(GetGraphic(), fullPath, &meshTexture[i].texture[MESHTEXTURE::EMISSIVE]);
	}

	void* vertices = nullptr;

	mesh->LockVertexBuffer(0, &vertices);

	_ulong verticeCount = mesh->GetNumVertices();
	_uint stride = D3DXGetFVFVertexSize(mesh->GetFVF());


	//메시를 구성하는 정점의 정의를 가져온다
	D3DVERTEXELEMENT9  decl[MAX_FVF_DECL_SIZE];

	mesh->GetDeclaration(decl);

	_ushort normalOffset = 0;
	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (decl[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffset = decl[i].Offset;
			break;
		}
	}

	for (_ulong i = 0; i < verticeCount; i++)
	{
		//정점의 위치벡터를 localmatrix 만큼 변환시킨다
		D3DXVec3TransformCoord((_vec3*)((_byte*)vertices + (i*stride)), (_vec3*)((_byte*)vertices + (i*stride)), localMatrix);
		D3DXVec3TransformNormal((_vec3*)((_byte*)vertices + normalOffset+(i*stride)), (_vec3*)((_byte*)vertices + normalOffset + (i*stride)), localMatrix);
	}
	mesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT StaticMesh::Ready(void * argument)
{
	return NOERROR;
}

HRESULT StaticMesh::Render(_ulong subsetIndex)
{
	if (mesh == nullptr)
		return E_FAIL;

		mesh->DrawSubset(subsetIndex);
	return NOERROR;
}

LPDIRECT3DTEXTURE9 StaticMesh::GetMaterialTexture(_ulong subsetIndex, MESHTEXTURE::TYPE type) const
{
	if(materialCount<=subsetIndex||meshTexture==nullptr)
		return nullptr;
	return meshTexture[subsetIndex].texture[type];
}

StaticMesh * StaticMesh::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * filePath, const _tchar * fileName, const _matrix* localMatrix,
	_bool diffuse, _bool normal, _bool specular, _bool emissive)
{
	StaticMesh* instance = new StaticMesh(graphicDevice);
	if (FAILED(instance->ReadyProto(filePath, fileName,localMatrix,diffuse, normal, specular, emissive)))
	{
		MSG("Failed Create StaticMesh");
		SafeRelease(instance);
	}
	return instance;
}

Component * StaticMesh::Clone(void * argument)
{
	StaticMesh* instance = new StaticMesh(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning StaticMesh");
		SafeRelease(instance);
	}
	return instance;
}

void StaticMesh::Free()
{
	for (size_t i = 0; i < materialCount; i++)
		for (size_t j = 0; j < MESHTEXTURE::LAST; j++)
			SafeRelease(meshTexture[i].texture[j]);

	if (IsClone() == false)
	{
		SafeDelete_Arr(materials);
		SafeDelete_Arr(meshTexture);
	}
	SafeRelease(adjacencyBuffer);
	SafeRelease(materialBuffer);
	SafeRelease(mesh);

	Component::Free();
}
