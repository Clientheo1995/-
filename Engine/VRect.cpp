#include "..\Headers\VRect.h"


VRect::VRect(LPDIRECT3DDEVICE9 graphicDevice) :VertexBuffer(graphicDevice)
{
	
}

VRect::VRect(const VRect & vB) : VertexBuffer(vB)
{
}

HRESULT VRect::ReadyProto()
{
	//VRect ���� ���� ������ ����, FVF �� �����Ѵ�
	stride = sizeof(VERTEXTEXTURE);
	vertices = 4;
	fvf = D3DFVF_XYZ | D3DFVF_TEX1;


	polygonIndicesSize = sizeof(INDEX16);
	format = D3DFMT_INDEX16;
	numPrimitive = 2;

	anchor = CENTER;
	//�����ص� ������ �������� �����迭(����) �� �����ߴ�.
	if (FAILED(VertexBuffer::ReadyProto()))//��������
		return E_FAIL;

	//���� �迭������ �� �ּҸ� ���������� ������
	VERTEXTEXTURE* ver = nullptr;

	//������ �ɾ���ٴ� - �� ���� �����尡 �� ������ �����ҰŴ�(��Ƽ������ ��Ȳ���� �ϳ��� �����常 ����� �� �ֵ���)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//�����迭�߿� ��𼭺���, ������(0�̸� ��ü����), ���� ������ ������ �ּ�

	//�ش� ������ ���� ä���
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
	graphicDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertices, 0, numPrimitive);//D3DPT_TRIANGLELIST �� ������ ������ �׸��� 0 ���� num����

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

	//���̾����������� �� �簢�� ������ �� �ﰢ��
	if (D3DXIntersectTri(&position[0], &position[1], &position[2], &vPivot, &vRay, &u, &v, &dist))
	{
		int a = 10;
		//pickingPos.x = ;
	}

	//���̾����������� �� �簢�� ������ �Ʒ��� �ﰢ��
	if (D3DXIntersectTri(&position[0], &position[2], &position[3], &vPivot, &vRay, &u, &v, &dist))
	{
		int a = 10;
	}
	//�� Ȯ���ؼ� ����������� ��Ȯ�ϰ� �ȴ� ������ ������ �Ƹ��� ������ �β������� �ƴұ� �ʹ�
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

	//������ �ɾ���ٴ� - �� ���� �����尡 �� ������ �����ҰŴ�(��Ƽ������ ��Ȳ���� �ϳ��� �����常 ����� �� �ֵ���)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//�����迭�߿� ��𼭺���, ������(0�̸� ��ü����), ���� ������ ������ �ּ�

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
	//������ �����Ͽ� ���� �迭�� ���� ����� �� �ֵ��� �Ѵ�
	vBuffer->Unlock();
}
