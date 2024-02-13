#include "..\Headers\ViewportRect.h"


ViewportRect::ViewportRect(LPDIRECT3DDEVICE9 graphicDevice) :VertexBuffer(graphicDevice)
{
}

ViewportRect::ViewportRect(const ViewportRect & vB) : VertexBuffer(vB), declare(vB.declare)
{
	AddRefCount(declare);
}

HRESULT ViewportRect::ReadyProto(_float startX, _float startY, _float sizeX, _float sizeY)
{
	//ViewportRect 만의 정점 정보와 갯수, FVF 를 셋팅한다
	stride = sizeof(VIEWPORT_VERTEX_TEXTURE);
	vertices = 4;
	fvf = D3DFVF_XYZRHW | D3DFVF_TEX1;

	polygonIndicesSize = sizeof(INDEX16);
	format = D3DFMT_INDEX16;
	numPrimitive = 2;

	//셋팅해둔 정보를 바탕으로 정점배열(버퍼) 를 선언했다.
	if (FAILED(VertexBuffer::ReadyProto()))//정적버퍼
		return E_FAIL;

	//정점 배열공간의 앞 주소를 얻어오기위한 포인터
	VIEWPORT_VERTEX_TEXTURE* ver = nullptr;

	//공간을 걸어잠근다 - 내 메인 쓰레드가 이 공간에 접근할거다(멀티쓰레드 상황에서 하나의 쓰레드만 사용할 수 있도록)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//정점배열중에 어디서부터, 어디까지(0이면 전체영역), 내가 선언한 형식의 주소

	//해당 공간에 값을 채운다
	ver[0].position = _vec4(startX, startY, 0.0f, 1.f);
	ver[0].textureUV = _vec2(0.f, 0.f);

	ver[1].position = _vec4(startX+sizeX, startY, 0.0f, 1.f);
	ver[1].textureUV = _vec2(1.f, 0.f);

	ver[2].position = _vec4(startX + sizeX, startY + sizeY, 0.0f, 1.f);
	ver[2].textureUV = _vec2(1.f, 1.f);

	ver[3].position = _vec4(startX, startY + sizeY, 0.0f, 1.f);
	ver[3].textureUV = _vec2(0.f, 1.f);

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

HRESULT ViewportRect::Ready(void * argument)
{
	return NOERROR;
}

HRESULT ViewportRect::Render()
{
	LPDIRECT3DDEVICE9 graphicDevice = GetGraphic();

	if (graphicDevice == nullptr || vBuffer == nullptr)
		return E_FAIL;

	AddRefCount(graphicDevice);
	graphicDevice->SetStreamSource(0, vBuffer, 0, stride);
	graphicDevice->SetFVF(fvf);
	graphicDevice->SetIndices(iBuffer);
	graphicDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertices, 0, numPrimitive);

	SafeRelease(graphicDevice);
	return NOERROR;
}

ViewportRect * ViewportRect::Create(LPDIRECT3DDEVICE9 graphicDevice, _float startX, _float startY, _float sizeX, _float sizeY)
{
	ViewportRect* instance = new ViewportRect(graphicDevice);
	if (FAILED(instance->ReadyProto(startX, startY, sizeX, sizeY)))
	{
		MSG("Failed Create ViewportRect");
		SafeRelease(instance);
	}
	return instance;
}

Component * ViewportRect::Clone(void * argument)
{
	ViewportRect* instance = new ViewportRect(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clone ViewportRect");
		SafeRelease(instance);
	}
	return instance;
}

void ViewportRect::Free()
{
	SafeRelease(declare);
	VertexBuffer::Free();
}
