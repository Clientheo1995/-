#include "..\Headers\Collider.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"
Collider::Collider(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice), pipeLine(PipeLine::GetInstance()), collision(false)
{
	AddRefCount(pipeLine);
}

Collider::Collider(const Collider & col) : Component(col), type(col.type), shader(col.shader), pipeLine(PipeLine::GetInstance()), collision(false)
{
	if (FAILED(col.mesh->CloneMeshFVF(col.mesh->GetOptions(), col.mesh->GetFVF(), GetGraphic(), &mesh)))
		MSG("Failed cloning collider mesh");
	AddRefCount(pipeLine);
	AddRefCount(shader);
}

HRESULT Collider::ReadyBox()
{
	if (FAILED(D3DXCreateBox(GetGraphic(), 1.f, 1.f, 1.f, &mesh, nullptr)))
		return E_FAIL;
	return NOERROR;
}

HRESULT Collider::ReadySphere()
{
	if (FAILED(D3DXCreateSphere(GetGraphic(), 0.5f, 10, 10, &mesh, nullptr)))
		return E_FAIL;
	return NOERROR;
}

HRESULT Collider::Render()
{
	if (shader == nullptr)
		return E_FAIL;

	shader->SetValue("world", &transform->GetWorld(), sizeof(_matrix));
	shader->SetValue("view", &pipeLine->GetTransform(D3DTS_VIEW), sizeof(_matrix));
	shader->SetValue("proj", &pipeLine->GetTransform(D3DTS_PROJECTION), sizeof(_matrix));

	shader->SetBool("IsCollision", collision);

	shader->ShaderBegin();
	shader->PassBegin(0);

	mesh->DrawSubset(0);

	shader->PassEnd();
	shader->ShaderEnd();


	return NOERROR;
}

HRESULT Collider::GetSuperWorld(_matrix * mat)
{
	return NOERROR;
}

_bool Collider::AABBCollision(Collider * target, _bool checkSource, _bool checkDest)
{
	if (target == nullptr)
		return false;

	if (checkSource)//check my attack
	{
		if (!this->attack)
		{
			collision = false;
			target->collision = false;
			return false;
		}
	}

	//for parry
	if (checkDest)//check their attack
	{
		if (!target->attack)
		{
			collision = false;
			target->collision = false;
			return false;
		}
	}

	_vec3 smin, smax;//내꺼
	_vec3 dmin, dmax;

	D3DXVec3TransformCoord(&smin, &min, transform->GetWorldMatPointer());
	D3DXVec3TransformCoord(&smax, &max, transform->GetWorldMatPointer());

	D3DXVec3TransformCoord(&dmin, &target->min, target->transform->GetWorldMatPointer());
	D3DXVec3TransformCoord(&dmax, &target->max, target->transform->GetWorldMatPointer());

	collision = false;
	target->collision = false;

	//너비
	if (max(smin.x, dmin.x) > min(smax.x, dmax.x))
		return false;

	//높이
	if (max(smin.y, dmin.y) > min(smax.y, dmax.y))
		return false;

	//깊이
	if (max(smin.z, dmin.z) > min(smax.z, dmax.z))
		return false;

	if (checkDest)
	{
		collision = true;
	}
	//else if (!checkDest)
	//{
	//	collision = false;
	//}
	if (checkSource)
	{
		target->SetCollision(true);
	}
	//else if (!checkSource)
	//{
	//	target->SetCollision(false);
	//}

	return _bool(true);
}

_bool Collider::OBBCollision(Collider * target, _bool checkSource, _bool checkDest)
{
	if (checkSource)//check my attack
	{
		if (!this->attack)
		{
			collision = false;
			target->collision = false;
			return false;
		}
	}

	//for parry
	if (checkDest)//check their attack
	{
		if (!target->attack)
		{
			collision = false;
			target->collision = false;
			return false;
		}
	}

	OBBOPTION obb[2];//충돌체 두개
	_matrix worldMat[2] = { { transform->GetWorld() },{ target->transform->GetWorld() } };

	memcpy(&obb[0], obbOP, sizeof(OBBOPTION));
	memcpy(&obb[1], target->obbOP, sizeof(OBBOPTION));

	for (size_t i = 0; i < 2; i++)//두 충돌체의
	{
		D3DXVec3TransformCoord(&obb[i].centerPos, &obb[i].centerPos, &worldMat[i]);
		for (size_t j = 0; j < 3; j++)//xyz투영 비교
		{
			D3DXVec3TransformNormal(&obb[i].projAxis[j], &obb[i].projAxis[j], &worldMat[i]);
			D3DXVec3TransformNormal(&obb[i].alignAxis[j], &obb[i].alignAxis[j], &worldMat[i]);
			//내적하고 투영한값을 노멀라이징 한다
			D3DXVec3Normalize(&obb[i].alignAxis[j], &obb[i].alignAxis[j]);
		}
	}

	_float distance[3];
	collision = false;
	target->collision = false;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			_vec3 dir = obb[1].centerPos - obb[0].centerPos;//각 충돌체의 중점사이의 길이
			distance[0] = fabs(D3DXVec3Dot(&dir, &obb[i].alignAxis[j]));

			distance[1] = fabs(D3DXVec3Dot(&obb[0].projAxis[0], &obb[i].alignAxis[j])) +
				fabs(D3DXVec3Dot(&obb[0].projAxis[1], &obb[i].alignAxis[j])) +
				fabs(D3DXVec3Dot(&obb[0].projAxis[2], &obb[i].alignAxis[j]));//한 충돌체에서 각 축 x y z 에 내적한 길이의 합

			distance[2] = fabs(D3DXVec3Dot(&obb[1].projAxis[0], &obb[i].alignAxis[j])) +
				fabs(D3DXVec3Dot(&obb[1].projAxis[1], &obb[i].alignAxis[j])) +
				fabs(D3DXVec3Dot(&obb[1].projAxis[2], &obb[i].alignAxis[j]));//다른 충돌체에서 각 축 x y z 에 내적한 길이의 합

			if (distance[1] + distance[2] < distance[0])
				return false;

		}
	}
	if (checkDest)
	{
		collision = true;
	}
	//else if (!checkDest)
	//{
	//	collision = false;
	//}
	if (checkSource)
	{
		target->SetCollision(true);
	}
	//else if (!checkSource)
	//{
	//	target->SetCollision(false);
	//}

	return _bool(true);
}

_bool Collider::SphereCollision(Collider * target)
{
	if (target->type == OBB)
		return false;
	_vec3 A = transform->GetState(Transform::POSITION);//본인
	_vec3 B = target->transform->GetState(Transform::POSITION);//대상
	_float length = D3DXVec3Length(&(A - B));

	if ((target->colliderOP.radius + colliderOP.radius) >= length)
	{
		collision = true;
		return true;
	}
	collision = false;
	return false;
}
_bool Collider::GetItem(Collider * target)
{
	if (!target->attack)
		return false;

	if (target->type == OBB)
		return false;
	_vec3 A = transform->GetState(Transform::POSITION);//본인
	_vec3 B = target->transform->GetState(Transform::POSITION);//대상
	_float length = D3DXVec3Length(&(A - B));

	if ((target->colliderOP.radius + colliderOP.radius) >= length)
	{
		collision = true;
		return true;
	}
	collision = false;
	return false;
}
_bool Collider::SphereCollision_Invince(Collider * target)
{
	if (!attack)
		return false;

	if (target->type == OBB)
	{
		target->collision = false;
		return false;
	}
	_vec3 A = transform->GetState(Transform::POSITION);//본인
	_vec3 B = target->transform->GetState(Transform::POSITION);//대상
	_float length = D3DXVec3Length(&(A - B));

	if ((target->colliderOP.radius + colliderOP.radius) >= length)
	{
		collision = true;
		target->collision = true;
		return true;
	}
	collision = false;
	target->collision = false;
	return false;
}
_int Collider::Update(_double timeDelta)
{
	switch (type)
	{
	case Collider::AABB:
		transform->SetState(Transform::POSITION, *(_vec3*)&colliderOP.targetMat->m[3][0]);
		break;
	case Collider::OBB:
		transform->SetState(Transform::RIGHT, *(_vec3*)&colliderOP.targetMat->m[0][0]);
		transform->SetState(Transform::UP, *(_vec3*)&colliderOP.targetMat->m[1][0]);
		transform->SetState(Transform::LOOK, *(_vec3*)&colliderOP.targetMat->m[2][0]);
		transform->SetState(Transform::POSITION, *(_vec3*)&colliderOP.targetMat->m[3][0]);
		break;
	case Collider::SPHERE:
		transform->SetState(Transform::POSITION, *(_vec3*)&colliderOP.targetMat->m[3][0]);
		break;
	}
	return _int();
}

HRESULT Collider::ReadyProto(COLLIDERTYPE type)
{
	this->type = type;
	HRESULT hr = 0;
	switch (type)
	{
	case Collider::AABB:
	case Collider::OBB:
		hr = ReadyBox();
		break;
	case Collider::SPHERE:
		hr = ReadySphere();
		break;
	}
	if (FAILED(hr))
		return E_FAIL;

	shader = Shader::Create(GetGraphic(), L"../Bin/Shader/Collider_Shader.fx");
	if (shader == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT Collider::Ready(void * argument)
{
	transform = Transform::Create(GetGraphic());
	if (transform == nullptr)
		return E_FAIL;

	if (argument != nullptr)
		memcpy(&colliderOP, argument, sizeof(COLLIDEROPTION));

	_matrix transformMat;
	_matrix scaleMat;

	switch (type)
	{
	case Collider::AABB:
	case Collider::OBB:
		D3DXMatrixScaling(&scaleMat, colliderOP.localScale.x, colliderOP.localScale.y, colliderOP.localScale.z);
		break;
	case Collider::SPHERE:
		colliderOP.diameter = colliderOP.radius * 2;
		D3DXMatrixScaling(&scaleMat, colliderOP.diameter, colliderOP.diameter, colliderOP.diameter);
		break;
	}

	//위치를 받아서 맥스상 원점을 만들수있음
	_matrix positionMat;
	D3DXMatrixTranslation(&positionMat, colliderOP.localPos.x, colliderOP.localPos.y, colliderOP.localPos.z);

	transformMat = scaleMat * positionMat;

	_ulong numVer = mesh->GetNumVertices();
	_uint stride = D3DXGetFVFVertexSize(mesh->GetFVF());

	void* vertices = nullptr;

	mesh->LockVertexBuffer(0, &vertices);

	for (size_t i = 0; i < numVer; i++)
		D3DXVec3TransformCoord((_vec3*)((_byte*)vertices + i * stride), (_vec3*)((_byte*)vertices + i * stride), &transformMat);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)vertices, numVer, stride, &min, &max)))
		return E_FAIL;
	mesh->UnlockVertexBuffer();


	if (SPHERE != type)
	{
		obbOP = new OBBOPTION;
		ZeroMemory(obbOP, sizeof(OBBOPTION));

		_vec3 point[8];

		point[0] = _vec3(min.x, max.y, min.z);
		point[1] = _vec3(max.x, max.y, min.z);
		point[2] = _vec3(max.x, min.y, min.z);
		point[3] = min;

		point[4] = _vec3(min.x, max.y, max.z);
		point[5] = max;
		point[6] = _vec3(max.x, min.y, max.z);
		point[7] = _vec3(min.x, min.y, max.z);

		obbOP->centerPos = (point[3] + point[5])*0.5f;

		//투영공간상의 두 면의 충돌 앞 점 0123 뒤 점 4567 왼쪽 위부터 시작하는 폴리곤
		obbOP->alignAxis[0] = point[2] - point[3];
		obbOP->alignAxis[1] = point[0] - point[3];
		obbOP->alignAxis[2] = point[7] - point[3];

		obbOP->projAxis[0] = (point[5] + point[2])*0.5f - obbOP->centerPos;
		obbOP->projAxis[1] = (point[5] + point[0])*0.5f - obbOP->centerPos;
		obbOP->projAxis[2] = (point[5] + point[7])*0.5f - obbOP->centerPos;
	}
	return NOERROR;
}

Collider * Collider::Create(LPDIRECT3DDEVICE9 graphicDevice, COLLIDERTYPE type)
{
	Collider* instance = new Collider(graphicDevice);
	if (FAILED(instance->ReadyProto(type)))
	{
		MSG("Failed Create Collider");
		SafeRelease(instance);
	}
	return instance;
}

Component * Collider::Clone(void * argument)
{
	Collider* instance = new Collider(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning Collider");
		SafeRelease(instance);
	}
	return instance;
}

void Collider::Free()
{
	SafeRelease(transform);
	SafeRelease(pipeLine);
	SafeRelease(shader);
	SafeRelease(mesh);
	Component::Free();
}
