#include "..\Headers\VTerrain.h"
#include "Manager.h"
VTerrain::VTerrain(LPDIRECT3DDEVICE9 graphicDevice) :VertexBuffer(graphicDevice)
{
}

VTerrain::VTerrain(const VTerrain & vB) : VertexBuffer(vB),terrainX(vB.terrainX), terrainZ(vB.terrainZ),interval(vB.interval)
{
}

HRESULT VTerrain::ReadyProto(_uint verX, _uint verZ, _float interval)
{
	//VRect ���� ���� ������ ����, FVF �� �����Ѵ�
	stride = sizeof(VERTEXTEXTURE);
	vertices = verX * verZ;
	fvf = D3DFVF_XYZ | D3DFVF_TEX1;

	polygonIndicesSize = sizeof(INDEX32);
	format = D3DFMT_INDEX32;
	numPrimitive = (verX - 1)*(verZ - 1) * 2;

	//�����ص� ������ �������� �����迭(����) �� �����ߴ�.
	if (FAILED(VertexBuffer::ReadyProto()))//��������
		return E_FAIL;

	//���� �迭������ �� �ּҸ� ���������� ������
	VERTEXTEXTURE* ver = nullptr;

	//������ �ɾ���ٴ� - �� ���� �����尡 �� ������ �����ҰŴ�(��Ƽ������ ��Ȳ���� �ϳ��� �����常 ����� �� �ֵ���)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//�����迭�߿� ��𼭺���, ������(0�̸� ��ü����), ���� ������ ������ �ּ�

	for (size_t i = 0; i < verZ; i++)
	{
		for (size_t j = 0; j < verX; j++)
		{
			_uint index = i * verX + j;
			ver[index].position = _vec3(j*interval, 0.f, i*interval);
			ver[index].textureUV = _vec2(j / float(verX - 1), i / float(verZ - 1));
			//UV��ǥ�� 0���� 1 ���̿� �ֱ� ������ ��ǥ���� �׸��� ����ũ���� 1���� �۾ƾ��Ѵ�(������ ����� �׸��� ������ �� ������ ����)
		}
	}
	//������ �����Ͽ� ���� �迭�� ���� ����� �� �ֵ��� �Ѵ�
	vBuffer->Unlock();



	//�ε��� ����
	INDEX32* indices = nullptr;
	iBuffer->Lock(0, 0, (void**)&indices, 0);
	_uint polygon = 0;

	//�� �� �� �������� �׷����� �ʱ� ������ ������ ������ -1
	for (size_t i = 0; i < verZ - 1; i++)
	{
		for (size_t j = 0; j < verX - 1; j++)
		{
			_uint index = i * verX + j;
			indices[polygon]._1 = index + verX;
			indices[polygon]._2 = index + verX + 1;
			indices[polygon]._3 = index + 1;
			polygon++;

			indices[polygon]._1 = index + verX;
			indices[polygon]._2 = index + 1;
			indices[polygon]._3 = index;
			polygon++;
		}
	}
	iBuffer->Unlock();
	return NOERROR;
}

HRESULT VTerrain::ReadyProto(const _tchar * heightMapPath, _float interval)
{
	_ulong byte = 0;
	HANDLE file = CreateFile(heightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


	//��� ���� �ȼ����� �￬������ �о�鿩�� �ϳ��� ��Ʈ���� �����ִ�
	BITMAPFILEHEADER bitheader;
	ReadFile(file, &bitheader, sizeof(BITMAPFILEHEADER), &byte, nullptr);

	BITMAPINFOHEADER bitinfo;
	ReadFile(file, &bitinfo, sizeof(BITMAPINFOHEADER), &byte, nullptr);

	_ulong* pixel = new _ulong[bitinfo.biWidth*bitinfo.biHeight];

	ReadFile(file, pixel, sizeof(_ulong)*bitinfo.biWidth*bitinfo.biHeight, &byte, 0);

	CloseHandle(file);

	//VRect ���� ���� ������ ����, FVF �� �����Ѵ�
	stride = sizeof(NORMALTEXTURE);
	vertices = bitinfo.biWidth*bitinfo.biHeight;
	terrainX = bitinfo.biWidth;
	terrainZ = bitinfo.biHeight;
	fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	this->interval = interval;
	polygonIndicesSize = sizeof(INDEX32);
	format = D3DFMT_INDEX32;
	numPrimitive = (bitinfo.biWidth - 1)*(bitinfo.biHeight - 1) * 2;

	//�����ص� ������ �������� �����迭(����) �� �����ߴ�.
	if (FAILED(VertexBuffer::ReadyProto()))//��������
		return E_FAIL;

	//���� �迭������ �� �ּҸ� ���������� ������
	NORMALTEXTURE* ver = nullptr;
	_uint versize = 0;
	//������ �ɾ���ٴ� - �� ���� �����尡 �� ������ �����ҰŴ�(��Ƽ������ ��Ȳ���� �ϳ��� �����常 ����� �� �ֵ���)
	vBuffer->Lock(0, 0, (void**)&ver, 0);//�����迭�߿� ��𼭺���, ������(0�̸� ��ü����), ���� ������ ������ �ּ�

	for (size_t i = 0; i < bitinfo.biHeight; i++)
	{
		for (size_t j = 0; j < bitinfo.biWidth; j++)
		{
			_uint index = i * bitinfo.biWidth + j;

			ver[index].position = _vec3(j*interval,0.f/*(pixel[index] & 0x000000ff)*/ / 10.0f, i*interval);//���� ���� ��縸 ���Ͽ� �����̸� ���
			ver[index].normal = _vec3(0.f, 0.f, 0.f);
			ver[index].textureUV = _vec2(j / float(bitinfo.biWidth - 1), i / float(bitinfo.biHeight - 1));
			//UV��ǥ�� 0���� 1 ���̿� �ֱ� ������ ��ǥ���� �׸��� ����ũ���� 1���� �۾ƾ��Ѵ�(������ ����� �׸��� ������ �� ������ ����)
		}
	}

	
	//�ε��� ����
	INDEX32* indices = nullptr;
	iBuffer->Lock(0, 0, (void**)&indices, 0);
	_uint polygon = 0;

	//�� �� �� �������� �׷����� �ʱ� ������ ������ ������ -1
	for (size_t i = 0; i < bitinfo.biHeight - 1; i++)
	{
		for (size_t j = 0; j < bitinfo.biWidth - 1; j++)
		{
			_uint index = i * bitinfo.biWidth + j;

			_vec3 source, destination, normal;//��ָ�(����)

			indices[polygon]._1 = index + bitinfo.biWidth;
			indices[polygon]._2 = index + bitinfo.biWidth + 1;
			indices[polygon]._3 = index + 1;

			//���⺤��
			source = ver[indices[polygon]._2].position - ver[indices[polygon]._1].position;
			destination = ver[indices[polygon]._3].position - ver[indices[polygon]._2].position;
			D3DXVec3Cross(&normal, &source, &destination);

			ver[indices[polygon]._1].normal += *D3DXVec3Normalize(&normal, &normal);
			ver[indices[polygon]._2].normal += *D3DXVec3Normalize(&normal, &normal);
			ver[indices[polygon]._3].normal += *D3DXVec3Normalize(&normal, &normal);
			polygon++;

			indices[polygon]._1 = index + bitinfo.biWidth;
			indices[polygon]._2 = index + 1;
			indices[polygon]._3 = index;

			source = ver[indices[polygon]._2].position - ver[indices[polygon]._1].position;
			destination = ver[indices[polygon]._3].position - ver[indices[polygon]._2].position;
			D3DXVec3Cross(&normal, &source, &destination);

			ver[indices[polygon]._1].normal += *D3DXVec3Normalize(&normal, &normal);
			ver[indices[polygon]._2].normal += *D3DXVec3Normalize(&normal, &normal);
			ver[indices[polygon]._3].normal += *D3DXVec3Normalize(&normal, &normal);
			polygon++;

		}
	}

	iBuffer->Unlock();
	for (size_t i = 0; i < vertices - 1; i++)
		D3DXVec3Normalize(&ver[i].normal, &ver[i].normal);

	for (size_t i = 0; i < vertices; i++)
		position[i] = ver[i].position;

	vBuffer->Unlock();
	SafeDelete_Arr(pixel);
	return NOERROR;
}

HRESULT VTerrain::Ready(void * argument)
{
	return NOERROR;
}

HRESULT VTerrain::Render()
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

VTerrain * VTerrain::Create(LPDIRECT3DDEVICE9 graphicDevice, _uint verX, _uint verZ, _float interval)
{
	VTerrain* instance = new VTerrain(graphicDevice);
	if (FAILED(instance->ReadyProto(verX, verZ, interval)))
	{
		MSG("Failed Create VertexTerrain");
		SafeRelease(instance);
	}
	return instance;
}

VTerrain * VTerrain::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* heightMapPath, _float interval)
{
	VTerrain* instance = new VTerrain(graphicDevice);
	if (FAILED(instance->ReadyProto(heightMapPath, interval)))
	{
		MSG("Failed Create VertexTerrain");
		SafeRelease(instance);
	}
	return instance;
}

Component * VTerrain::Clone(void * argument)
{
	VTerrain* instance = new VTerrain(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning VertexTerrain");
		SafeRelease(instance);
	}
	return instance;
}

_bool VTerrain::Picking(const Transform * transform, _vec3 * point)const
{
	if (picking == nullptr)
		return false;
	_vec3 vRay, vPivot;
	if (FAILED(picking->LocalSpaceMouse(transform, &vRay, &vPivot)))
		return false;

	_float u, v, dist;

	_bool collision = false;

	for (size_t i = 0; i < terrainZ - 1; i++)
	{
		for (size_t j = 0; j < terrainX - 1; j++)
		{
			_uint index = i * terrainX + j;

			_uint order[4] = { 
				index + terrainX ,
				index + terrainX + 1,
				index + 1,
				index };


			//�� �ﰢ��
			if (collision=D3DXIntersectTri(&position[order[1]], &position[order[0]], &position[order[2]], &vPivot, &vRay, &u, &v, &dist))
			{
				//������ �������� ���
				_vec3 width = (position[order[0]] - position[order[1]]);
				_float length = D3DXVec3Length(&width);

				D3DXVec3Normalize(&width, &width);
				width *= (length*u);

				_vec3 height =( position[order[2]] - position[order[1]]);
				length = D3DXVec3Length(&height);
				D3DXVec3Normalize(&height, &height);
				height *= (length*v);

				*point = position[order[1]] + width + height;

				return collision;
			}

			//�Ʒ��ﰢ��
			if (collision = D3DXIntersectTri(&position[order[3]], &position[0], &position[order[2]], &vPivot, &vRay, &u, &v, &dist))
			{
				//���α���
				_vec3 width = position[order[2]]- position[order[3]];
				_float length = D3DXVec3Length(&width);

				D3DXVec3Normalize(&width, &width);
				width *= (length*u);

				_vec3 height = position[order[0]] - position[order[3]];
				D3DXVec3Normalize(&height, &height);
				length = D3DXVec3Length(&height);
				height *= (length*v);


				*point = position[order[3]] + width + height;
				return collision;
			}
		}
	}
	return _bool(collision);
}

_float VTerrain::CalculateY(_vec3 position) const
{
	_uint index = _uint(position.z / interval)*terrainX + _uint(position.x/ interval);
	_float ratioX = (position.x - this->position[index + terrainX].x) / interval;
	_float ratioZ = (this->position[index + terrainX].z-position.z) / interval;

	_float orderY[4] = {
		this->position[index+ terrainX].y,
		this->position[index + terrainX+1].y,
		this->position[index+1].y,
		this->position[index].y
	};

	if (ratioX > ratioZ)//������ ��
	{
		return orderY[0] + (orderY[1]- orderY[0])*ratioX + (orderY[2]- orderY[1])*ratioZ;
	}
	else //���� �Ʒ�
	{
		return orderY[0] + (orderY[3] - orderY[0])*ratioZ + (orderY[2] - orderY[3])*ratioX;
	}
}

void VTerrain::Free()
{
	VertexBuffer::Free();
}
