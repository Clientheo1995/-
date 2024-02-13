#include "..\Headers\VRect.h"


VRect::VRect(LPDIRECT3DDEVICE9 graphicDevice) :VertexBuffer(graphicDevice)
{
	
}

VRect::VRect(const VRect & vB) : VertexBuffer(vB)
{
}

HRESULT VRect::ReadyProto()
{
	//VRect 만의 정점 정보와 갯수, FVF 를 셋팅한다
	stride = sizeof(VERTEXTEXTURE);
	vertices = 4;
	fvf = D3DFVF_XYZ | D3DFVF_TEX1;


	polygonIndicesSize = sizeof(INDEX16);
	format = D3DFMT_INDEX16;
	numPrimitive = 2;

	anchor = CENTER;
	//셋팅해둔 정보를 바탕으로 정점배열(버퍼) 를 선언했다.
	if (FAILED(VertexBuffer::ReadyProto()))//정적버퍼
		return E_FAIL;

	//정점 배열공간의 앞 주소를 얻어오기위한 포인터
	VERTEXTEXTURE* ver = nullptr;

	//공간을 걸어잠근다 - 내 메인 쓰레드가 이 공간에 접근할거다(멀티쓰레드 상황에서 하나의 쓰레드만 사용할 수 있도록)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//정점배열중에 어디서부터, 어디까지(0이면 전체영역), 내가 선언한 형식의 주소

	//해당 공간에 값을 채운다
	ver[0].position = _vec3(-1.f, 1.f, 0.0f);
	ver[0].textureUV = _vec2(0.f, 0.f);

	ver[1].position = _vec3(1.f, 1.f, 0.0f);
	ver[1].textureUV = _vec2(1.f, 0.f);

	ver[2].position = _vec3(1.f, -1.f, 0.0f);
	ver[2].textureUV = _vec2(1.f, 1.f);

	ver[3].position = _vec3(-1.f, -1.f, 0.0f);
	ver[3].textureUV = _vec2(0.f, 1.f);

	for (size_t i = 0; i < vertices; i++)
		position[i] = ver[i].position;
	//공간을 해제하여 정점 배열의 값을 사용할 수 있도록 한다
	vBuffer->Unlock();

	INDEX16* indices = nullptr;

	iBuffer->Lock(0, 0, (void**)&indices, 0);

	indices[0]._1 = 0;
	indices[0]._2 = 1;
	indices[0]._3 = 2;

	indices[1]._1 = 0;
	indices[1]._2 = 2;
	indices[1]._3 = 3;

	iBuffer->Unlock();
	return NOERROR;
}

HRESULT VRect::Ready(void * argument)
{
	return NOERROR;
}

HRESULT VRect::Render()
{
	LPDIRECT3DDEVICE9 graphicDevice = GetGraphic();

	if (graphicDevice == nullptr || vBuffer == nullptr)
		return E_FAIL;

	AddRefCount(graphicDevice);
	graphicDevice->SetStreamSource(0, vBuffer, 0, stride);
	graphicDevice->SetFVF(fvf);
	graphicDevice->SetIndices(iBuffer);
	graphicDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertices, 0, numPrimitive);//D3DPT_TRIANGLELIST 점 세개를 꺼내서 그린다 0 부터 num까지

	SafeRelease(graphicDevice);
	return NOERROR;
}

HRESULT VRect::SetAnchor(DRAWANCHOR anc)
{
	anchor = anc;
	VERTEXTEXTURE* ver = nullptr;
	switch (anchor)
	{
	case LEFT_TOP:
		vBuffer->Lock(0, 0, (void**)&ver, 0);

		ver[0].position = _vec3(0.f, 0.f, 0.0f);
		ver[0].textureUV = _vec2(0.f, 0.f);

		ver[1].position = _vec3(2.f, 0.f, 0.0f);
		ver[1].textureUV = _vec2(1.f, 0.f);

		ver[2].position = _vec3(2.f, -2.f, 0.0f);
		ver[2].textureUV = _vec2(1.f, 1.f);

		ver[3].position = _vec3(0.f, -2.f, 0.0f);
		ver[3].textureUV = _vec2(0.f, 1.f);

		for (size_t i = 0; i < vertices; i++)
			position[i] = ver[i].position;
		vBuffer->Unlock();
		break;
	case CENTER:
		vBuffer->Lock(0, 0, (void**)&ver, 0);

		ver[0].position = _vec3(-1.f, 1.f, 0.0f);
		ver[0].textureUV = _vec2(0.f, 0.f);

		ver[1].position = _vec3(1.f, 1.f, 0.0f);
		ver[1].textureUV = _vec2(1.f, 0.f);

		ver[2].position = _vec3(1.f, -1.f, 0.0f);
		ver[2].textureUV = _vec2(1.f, 1.f);

		ver[3].position = _vec3(-1.f, -1.f, 0.0f);
		ver[3].textureUV = _vec2(0.f, 1.f);

		for (size_t i = 0; i < vertices; i++)
			position[i] = ver[i].position;
		vBuffer->Unlock();
		break;
	}
	return NOERROR;
}

_bool VRect::Picking(Transform * transform, _vec3 * point)
{
	if (picking == nullptr)
		return false;
	_vec3 vRay, vPivot;
	if (FAILED(picking->LocalSpaceMouse(transform, &vRay, &vPivot)))
		return false;

	_float u, v, dist;

	//와이어프레임으로 본 사각형 내부의 위 삼각형
	if (D3DXIntersectTri(&position[0], &position[1], &position[2], &vPivot, &vRay, &u, &v, &dist))
	{
		int a = 10;
		//pickingPos.x = ;
	}

	//와이어프레임으로 본 사각형 내부의 아래쪽 삼각형
	if (D3DXIntersectTri(&position[0], &position[2], &position[3], &vPivot, &vRay, &u, &v, &dist))
	{
		int a = 10;
	}
	//좀 확대해서 가까워져야지 정확하게 된다 이유는 나도모름 아마도 레이의 두께때문이 아닐까 싶다
	return _bool();
}

VRect * VRect::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	VRect* instance = new VRect(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create VRect");
		SafeRelease(instance);
	}
	return instance;
}

Component * VRect::Clone(void * argument)
{
	VRect* instance = new VRect(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clone VRect");
		SafeRelease(instance);
	}
	return instance;
}

void VRect::Free()
{
	VertexBuffer::Free();
}

void VRect::SetPos(_vec3 pos)
{
	VERTEXTEXTURE* ver = nullptr;

	//공간을 걸어잠근다 - 내 메인 쓰레드가 이 공간에 접근할거다(멀티쓰레드 상황에서 하나의 쓰레드만 사용할 수 있도록)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//정점배열중에 어디서부터, 어디까지(0이면 전체영역), 내가 선언한 형식의 주소

	ver[0].position = _vec3(pos.x - 0.5f, pos.y - 0.5f, 0.f);
	ver[1].position = _vec3(pos.x + 0.5f, pos.y - 0.5f, 0.f);
	ver[2].position = _vec3(pos.x + 0.5f, pos.y + 0.5f, 0.f);
	ver[3].position = _vec3(pos.x - 0.5f, pos.y + 0.5f, 0.f);

	ver[0].textureUV = _vec2(0.f, 0.f);
	ver[1].textureUV = _vec2(1.f, 0.f);
	ver[2].textureUV = _vec2(1.f, 1.f);
	ver[3].textureUV = _vec2(0.f, 1.f);

	for (size_t i = 0; i < vertices; i++)
		position[i] = ver[i].position;
	//공간을 해제하여 정점 배열의 값을 사용할 수 있도록 한다
	vBuffer->Unlock();
}
