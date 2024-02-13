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
	//ViewportRect ���� ���� ������ ����, FVF �� �����Ѵ�
	stride = sizeof(VIEWPORT_VERTEX_TEXTURE);
	vertices = 4;
	fvf = D3DFVF_XYZRHW | D3DFVF_TEX1;

	polygonIndicesSize = sizeof(INDEX16);
	format = D3DFMT_INDEX16;
	numPrimitive = 2;

	//�����ص� ������ �������� �����迭(����) �� �����ߴ�.
	if (FAILED(VertexBuffer::ReadyProto()))//��������
		return E_FAIL;

	//���� �迭������ �� �ּҸ� ���������� ������
	VIEWPORT_VERTEX_TEXTURE* ver = nullptr;

	//������ �ɾ���ٴ� - �� ���� �����尡 �� ������ �����ҰŴ�(��Ƽ������ ��Ȳ���� �ϳ��� �����常 ����� �� �ֵ���)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//�����迭�߿� ��𼭺���, ������(0�̸� ��ü����), ���� ������ ������ �ּ�

	//�ش� ������ ���� ä���
	ver[0].position = _vec4(startX, startY, 0.0f, 1.f);
	ver[0].textureUV = _vec2(0.f, 0.f);

	ver[1].position = _vec4(startX+sizeX, startY, 0.0f, 1.f);
	ver[1].textureUV = _vec2(1.f, 0.f);

	ver[2].position = _vec4(startX + sizeX, startY + sizeY, 0.0f, 1.f);
	ver[2].textureUV = _vec2(1.f, 1.f);

	ver[3].position = _vec4(startX, startY + sizeY, 0.0f, 1.f);
	ver[3].textureUV = _vec2(0.f, 1.f);

	//������ �����Ͽ� ���� �迭�� ���� ����� �� �ֵ��� �Ѵ�
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
