#include "../Headers/Camera.h"

Camera::Camera(LPDIRECT3DDEVICE9 graphicDevice) :GameObject(graphicDevice), pipeLine(PipeLine::GetInstance())
{
	AddRefCount(pipeLine);
}

Camera::Camera(const Camera & gameObject) :GameObject(gameObject), pipeLine(gameObject.pipeLine),transformComponent(gameObject.transformComponent)
{
	AddRefCount(pipeLine);
}

HRESULT Camera::LateInit()
{
	return NOERROR;
}

HRESULT Camera::ReadyProto()
{
	transformComponent = Transform::Create(GetGraphic());
	if (transformComponent == nullptr)
		return E_FAIL;
	return NOERROR;
}

HRESULT Camera::ReadyGameObject(void * argument)
{
	option = *(CAMERAOPTION*)argument;

	Transform::STATEOPTION  stateOP;

	stateOP.speed = option.speed;
	stateOP.rotation = option.rotation;

	transformComponent->SetStateOption(stateOP);

		//카메라가 바라보는 벡터
	look = option.at - option.eye;

	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&right, &option.axisY, &look);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&right, &right);

	D3DXVec3Cross(&up, &look, &right);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&up, &up);

	transformComponent->SetState(Transform::RIGHT, right);
	transformComponent->SetState(Transform::UP, up);
	transformComponent->SetState(Transform::LOOK, look);
	//transformComponent->SetState(Transform::POSITION, position);

	viewMat = transformComponent->GetWorldMatInverse();

	D3DXMatrixPerspectiveFovLH(&projMat, option.fovY, option.aspect, option._near, option._far);

	return NOERROR;
}

HRESULT Camera::SetVector()
{
	//카메라가 바라보는 벡터
	look = option.at - option.eye;
	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&right, &option.axisY, &look);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&right, &right);

	D3DXVec3Cross(&up, &look, &right);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&up, &up);
	return NOERROR;
}

HRESULT Camera::SetView()
{
	look = option.at - option.eye;
	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&right, &option.axisY, &look);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&right, &right);

	D3DXVec3Cross(&up, &look, &right);//외적해서 나머지하나를 구하기
	D3DXVec3Normalize(&up, &up);

	transformComponent->SetState(Transform::RIGHT, right);
	transformComponent->SetState(Transform::UP, up);
	transformComponent->SetState(Transform::LOOK, look);
	//transformComponent->SetState(Transform::POSITION, option.eye);

	viewMat = transformComponent->GetWorldMatInverse();

	return NOERROR;
}
HRESULT Camera::RotatePlayerY(const _vec3 * axis, _double timeDelta)
{
	_matrix rotate;
	_vec3 dist;

	D3DXMatrixRotationY(&rotate, timeDelta);//timedelta 에 값을 곱해줘도 된다
	memcpy(&rotate._41, &option.at,sizeof(D3DXVECTOR3));

	dist = option.eye - option.at;

	D3DXVec3TransformNormal(&right, &right, &rotate);
	D3DXVec3Normalize(&right, &right);

	D3DXVec3TransformNormal(&up, &up, &rotate);
	D3DXVec3Normalize(&up, &up);

	D3DXVec3TransformNormal(&look, &look, &rotate);
	D3DXVec3Normalize(&look, &look);

	D3DXVec3TransformCoord(&option.eye, &dist, &rotate);

	//position = option.eye;
	return NOERROR;
}
HRESULT Camera::RotatePlayerX(const _vec3 * axis, _double timeDelta)
{
	_matrix rotate;
	_vec3 dist;

	D3DXMatrixRotationX(&rotate, timeDelta);
	memcpy(&rotate._41, &option.at, sizeof(D3DXVECTOR3));

	dist = option.eye - option.at;

	D3DXVec3TransformNormal(&right, &right, &rotate);
	D3DXVec3Normalize(&right, &right);

	D3DXVec3TransformNormal(&up, &up, &rotate);
	D3DXVec3Normalize(&up, &up);

	D3DXVec3TransformNormal(&look, &look, &rotate);
	D3DXVec3Normalize(&look, &look);

	D3DXVec3TransformCoord(&option.eye, &dist, &rotate);

	//position = option.eye;
	return NOERROR;
}

HRESULT Camera::Forward(_double timeDelta, _float speed)
{
	option.eye += *D3DXVec3Normalize(&look, &look)* speed*_float(timeDelta);
	position = option.eye;
	return NOERROR;
}
HRESULT Camera::Back(_double timeDelta, _float speed)
{
	option.eye -= *D3DXVec3Normalize(&look, &look)* speed*_float(timeDelta);
	position = option.eye;
	return NOERROR;
}
HRESULT Camera::Left(_double timeDelta, _float speed)
{
	option.eye -= *D3DXVec3Normalize(&right, &right)* speed*_float(timeDelta);
	position = option.eye;
	return NOERROR;
}
HRESULT Camera::Right(_double timeDelta, _float speed)
{
	option.eye += *D3DXVec3Normalize(&right, &right)* speed*_float(timeDelta);
	position = option.eye;
	return NOERROR;
}
_int Camera::Update(_double timeDelta)
{
	pipeLine->SetTransform(D3DTS_VIEW, viewMat = transformComponent->GetWorldMatInverse());
	pipeLine->SetTransform(D3DTS_PROJECTION, projMat);
	return _int();
}

_int Camera::LateUpdate(_double timeDelta)
{
	return _int();
}

HRESULT Camera::Render()
{
	return NOERROR;
}

void Camera::Free()
{
	SafeRelease(pipeLine);
	SafeRelease(transformComponent);

	GameObject::Free();
}
