#include "..\Headers\Transform.h"

Transform::Transform(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
}

Transform::Transform(const Transform & component) : Component(component), worldMat(component.worldMat)
{
}

_matrix Transform::GetWorldMatInverse()const
{
	_matrix worldInv;

	D3DXMatrixInverse(&worldInv, nullptr, &worldMat);
	return _matrix(worldInv);
}

void Transform::SetStateOption(STATEOPTION stateop)
{
	stateOP = stateop;
}

HRESULT Transform::ReadyProto()
{
	D3DXMatrixIdentity(&worldMat);
	return NOERROR;
}

HRESULT Transform::Ready(void * argument)
{
	if (argument != nullptr)
		memcpy(&stateOP, argument, sizeof(STATEOPTION));
	return NOERROR;
}

void Transform::SetRotate(const _vec3 * axis, _float radian)
{
	//원형에서 회전
	_matrix rotate;
	D3DXMatrixRotationAxis(&rotate, axis, radian);

	_vec3 right, up, look;
	right = _vec3(1.f, 0.f, 0.f)*GetScale(Transform::RIGHT);
	up = _vec3(0.f, 1.f, 0.f)*GetScale(Transform::UP);
	look = _vec3(0.f, 0.f, 1.f)*GetScale(Transform::LOOK);

	D3DXVec3TransformNormal((_vec3*)&worldMat.m[RIGHT][0], &right, &rotate);
	D3DXVec3TransformNormal((_vec3*)&worldMat.m[UP][0], &up, &rotate);
	D3DXVec3TransformNormal((_vec3*)&worldMat.m[LOOK][0], &look, &rotate);
}

void Transform::SetScale(_vec3 scale)
{
	_vec3 right, up, look;

	right = GetState(Transform::RIGHT);
	up = GetState(Transform::UP);
	look = GetState(Transform::LOOK);

	right = *D3DXVec3Normalize(&right, &right)*scale.x;
	up = *D3DXVec3Normalize(&up, &up)*scale.y;
	look = *D3DXVec3Normalize(&look, &look)*scale.z;

	SetState(Transform::RIGHT, right);
	SetState(Transform::UP, up);
	SetState(Transform::LOOK, look);
}
void Transform::SetY(const _float y)
{
	worldMat.m[3][1] = y;
}

HRESULT Transform::SetLook(const _vec3 * look)
{
	_vec3 right, up, look_;
	look_ = *look;

	D3DXVec3Normalize(&look_, &look_);

	D3DXVec3Cross(&right, &_vec3(0.f, 1.f, 0.f), &look_);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&right, &right);

	D3DXVec3Cross(&up, &look_, &right);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&up, &up);

	_matrix mat;
	memcpy(&worldMat.m[0], &right, sizeof(_vec3));
	memcpy(&worldMat.m[1], &up, sizeof(_vec3));
	memcpy(&worldMat.m[2], &look_, sizeof(_vec3));

	return NOERROR;
}

HRESULT Transform::Forward(_double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 look = GetState(Transform::LOOK);

	position += *D3DXVec3Normalize(&look, &look)* stateOP.speed*_float(timeDelta);
	SetState(Transform::POSITION, position);
	return NOERROR;
}
HRESULT Transform::Forward(Navigation * navi, _double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 look = GetState(Transform::LOOK);

	position += *D3DXVec3Normalize(&look, &look)* stateOP.speed*_float(timeDelta);
	SetState(Transform::POSITION, position);

	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);

	}
	return NOERROR;
}
HRESULT Transform::Forward(Navigation* navi, _double timeDelta, _matrix camMat)
{
	_vec3 camLook = _vec3(camMat.m[2][0], 0.f, camMat.m[2][2]);
	_vec3 camRight = _vec3(camMat.m[0]);
	D3DXVec3Normalize(&camLook, &camLook);
	D3DXVec3Normalize(&camRight, &camRight);

	SetState(Transform::LOOK, camLook);
	SetState(Transform::RIGHT, camRight);

	_vec3 position = GetState(Transform::POSITION);
	position += camLook* stateOP.speed*_float(timeDelta);

	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);

	}
	return NOERROR;
}

HRESULT Transform::Back(_double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 look = GetState(Transform::LOOK);

	position -= *D3DXVec3Normalize(&look, &look)* stateOP.speed*_float(timeDelta);
	SetState(Transform::POSITION, position);
	return NOERROR;
}

HRESULT Transform::Back(Navigation * navi, _double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 look = GetState(Transform::LOOK);

	position -= *D3DXVec3Normalize(&look, &look)* stateOP.speed*_float(timeDelta);
	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);

	}
	return NOERROR;
}

HRESULT Transform::Back(Navigation* navi, _double timeDelta, _matrix camMat)
{
	_vec3 camLook = _vec3(camMat.m[2][0], 0.f, camMat.m[2][2]);
	_vec3 camRight = _vec3(camMat.m[0]);
	D3DXVec3Normalize(&camLook, &camLook);
	D3DXVec3Normalize(&camRight, &camRight);

	SetState(Transform::LOOK, camLook);
	SetState(Transform::RIGHT, camRight);

	_vec3 position = GetState(Transform::POSITION);
	position -= camLook* stateOP.speed*_float(timeDelta);

	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);
	}
	return NOERROR;
}

HRESULT Transform::Left(_double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 right = GetState(Transform::RIGHT);

	position -= *D3DXVec3Normalize(&right, &right)* stateOP.speed*_float(timeDelta);
	SetState(Transform::POSITION, position);
	return NOERROR;
}

HRESULT Transform::Left(Navigation * navi, _double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 right = GetState(Transform::RIGHT);

	position -= *D3DXVec3Normalize(&right, &right)* stateOP.speed*_float(timeDelta);
	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);
	}
	return NOERROR;
}

HRESULT Transform::Left(Navigation* navi, _double timeDelta, _matrix camMat)
{
	_vec3 camLook = _vec3(camMat.m[2][0], 0.f, camMat.m[2][2]);
	_vec3 camRight = _vec3(camMat.m[0]);
	D3DXVec3Normalize(&camLook, &camLook);
	D3DXVec3Normalize(&camRight, &camRight);

	SetState(Transform::LOOK, camLook);
	SetState(Transform::RIGHT, camRight);

	_vec3 position = GetState(Transform::POSITION);
	position -= camRight* stateOP.speed*_float(timeDelta);

	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);
	}
	return NOERROR;
}

HRESULT Transform::Right(_double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 right = GetState(Transform::RIGHT);

	position += *D3DXVec3Normalize(&right, &right)* stateOP.speed*_float(timeDelta);
	SetState(Transform::POSITION, position);
	return NOERROR;
}

HRESULT Transform::Right(Navigation * navi, _double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 right = GetState(Transform::RIGHT);

	position += *D3DXVec3Normalize(&right, &right)* stateOP.speed*_float(timeDelta);
	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);
	}
	return NOERROR;
}

HRESULT Transform::Right(Navigation* navi, _double timeDelta, _matrix camMat)
{
	_vec3 camLook = _vec3(camMat.m[2][0], 0.f, camMat.m[2][2]);
	_vec3 camRight = _vec3(camMat.m[0]);
	D3DXVec3Normalize(&camLook, &camLook);
	D3DXVec3Normalize(&camRight, &camRight);

	SetState(Transform::LOOK, camLook);
	SetState(Transform::RIGHT, camRight);

	_vec3 position = GetState(Transform::POSITION);
	position += camRight* stateOP.speed*_float(timeDelta);

	if (navi->CheckMove(&position))
	{
		navi->CalculateY(&position);
		SetState(Transform::POSITION, position);
	}
	return NOERROR;
}

HRESULT Transform::RotateAxis(const _vec3 * axis, _double timeDelta)
{
	_matrix rotate;
	D3DXMatrixRotationAxis(&rotate, axis, stateOP.rotation*timeDelta);

	_vec3 right, up, look;

	right = GetState(Transform::RIGHT);
	up = GetState(Transform::UP);
	look = GetState(Transform::LOOK);

	D3DXVec3TransformNormal((_vec3*)&worldMat.m[RIGHT][0], &right, &rotate);
	D3DXVec3TransformNormal((_vec3*)&worldMat.m[UP][0], &up, &rotate);
	D3DXVec3TransformNormal((_vec3*)&worldMat.m[LOOK][0], &look, &rotate);
	return NOERROR;
}


HRESULT Transform::RotateY(const _vec3 * axis, _double timeDelta)
{
	_matrix rotate;

	D3DXMatrixRotationAxis(&rotate, axis, stateOP.rotation*timeDelta);

	_vec3 right, up, look, position;

	right = GetState(Transform::RIGHT);
	up = GetState(Transform::UP);
	look = GetState(Transform::LOOK);
	position = GetState(Transform::POSITION);

	D3DXVec3TransformCoord((_vec3*)&worldMat.m[RIGHT][0], &right, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[UP][0], &up, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[LOOK][0], &look, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[POSITION][0], &position, &rotate);
	return NOERROR;
}

HRESULT Transform::RotateX(const _vec3 * axis, _double timeDelta)
{
	//아직 수정 안함
	_matrix rotate;

	D3DXMatrixRotationAxis(&rotate, axis, stateOP.rotation*timeDelta);

	_vec3 right, up, look, position;

	right = GetState(Transform::RIGHT);
	up = GetState(Transform::UP);
	look = GetState(Transform::LOOK);
	position = GetState(Transform::POSITION);

	D3DXVec3TransformCoord((_vec3*)&worldMat.m[RIGHT][0], &right, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[UP][0], &up, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[LOOK][0], &look, &rotate);
	D3DXVec3TransformCoord((_vec3*)&worldMat.m[POSITION][0], &position, &rotate);
	return NOERROR;
}

_float Transform::Parsing(_vec3 targetPos, _double timeDelta)
{
	_vec3 position = GetState(Transform::POSITION);
	_vec3 dir = targetPos - position;
	_float distance = D3DXVec3Length(&dir);

	_vec3 right, up, look;
	up = GetState(Transform::UP);

	D3DXVec3Cross(&right, &up, &dir);
	D3DXVec3Cross(&look, &right, &up);

	SetState(Transform::RIGHT, *D3DXVec3Normalize(&right, &right)*GetScale(Transform::RIGHT));
	SetState(Transform::RIGHT, *D3DXVec3Normalize(&look, &look)*GetScale(Transform::LOOK));
	Forward(timeDelta);
	return distance;
}

Transform * Transform::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	Transform* instance = new Transform(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create Transform");
		SafeRelease(instance);
	}
	return instance;
}

Component * Transform::Clone(void * argument)
{
	Transform* instance = new Transform(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning Transform");
		SafeRelease(instance);
	}
	return instance;
}

void Transform::Free()
{
	Component::Free();
}
