#include "..\Headers\Frustum.h"
Frustum::Frustum(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice), pipeLine(PipeLine::GetInstance())
{
	AddRefCount(pipeLine);
}

_bool Frustum::FrustumCulling(_matrix* mat, _float radius)
{
	if (pipeLine == nullptr)
		return false;

	_vec3 point[8];//지역변수로 만들어서 멤버변수의 값이 중복으로 변경되지 않도록

	_matrix proj = pipeLine->GetTransform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&proj, nullptr, &proj);

	_matrix view = pipeLine->GetTransform(D3DTS_VIEW);
	D3DXMatrixInverse(&view, nullptr, &view);

	_matrix worldMat = *mat;
	D3DXMatrixInverse(&worldMat, nullptr, &worldMat);


	for (size_t i = 0; i<8; i++)
	{
		//뷰스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &projPoint[i], &proj);

		//월드스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &point[i], &view);

		//로컬 스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &point[i], &worldMat);
	}


	//+x
	D3DXPlaneFromPoints(&plane[0], &point[1], &point[5], &point[6]);
	//-x
	D3DXPlaneFromPoints(&plane[1], &point[4], &point[0], &point[3]);
	//+y
	D3DXPlaneFromPoints(&plane[2], &point[4], &point[5], &point[1]);
	//-y
	D3DXPlaneFromPoints(&plane[3], &point[3], &point[2], &point[6]);
	//+z
	D3DXPlaneFromPoints(&plane[4], &point[5], &point[4], &point[7]);
	//-z
	D3DXPlaneFromPoints(&plane[5], &point[0], &point[1], &point[2]);

	_vec3 localPos;
	_vec3 matPos;
	memcpy(&matPos, &mat->m[3], sizeof(_vec3));

	D3DXVec3TransformCoord(&localPos, &matPos, &worldMat);

	return !InFrustum(localPos, radius);
}

_bool Frustum::FrustumCulling(Transform * transform, _float radius)
{
	if (pipeLine == nullptr)
		return false;

	_vec3 point[8];//지역변수로 만들어서 멤버변수의 값이 중복으로 변경되지 않도록

	_matrix proj = pipeLine->GetTransform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&proj, nullptr, &proj);
	
	_matrix view= pipeLine->GetTransform(D3DTS_VIEW);
	D3DXMatrixInverse(&view, nullptr, &view);

	_matrix worldMat = transform->GetWorldMatInverse();


	for (size_t i = 0; i<8; i++)
	{
		//뷰스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &projPoint[i], &proj);

		//월드스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &point[i], &view);

		//로컬 스페이스 상의 절두체 여덟개 점을 구한다
		D3DXVec3TransformCoord(&point[i], &point[i], &worldMat);
	}


	//+x
	D3DXPlaneFromPoints(&plane[0], &point[1], &point[5], &point[6]);
	//-x
	D3DXPlaneFromPoints(&plane[1], &point[4], &point[0], &point[3]);
	//+y
	D3DXPlaneFromPoints(&plane[2], &point[4], &point[5], &point[1]);
	//-y
	D3DXPlaneFromPoints(&plane[3], &point[3], &point[2], &point[6]);
	//+z
	D3DXPlaneFromPoints(&plane[4], &point[5], &point[4], &point[7]);
	//-z
	D3DXPlaneFromPoints(&plane[5], &point[0], &point[1], &point[2]);

	_vec3 localPos;

	D3DXVec3TransformCoord(&localPos, &transform->GetState(Transform::POSITION), &worldMat);

	return !InFrustum(localPos, radius);
}

_bool Frustum::InFrustum(_vec3 pointInLocal, _float radius)
{
	//ax + by + cz + d>0

	//a b c d
	//x y z 1

	for (size_t i = 0; i < 6; i++)
	{
		//z뒤의 1을 살리기 위해 coord를 쓴다
		if (D3DXPlaneDotCoord(&plane[i], &pointInLocal) > radius)
			return false;
	}
	return _bool(true);
}

HRESULT Frustum::ReadyProto()
{
	//원형객체 생성 시 투영스페이스 상의 절두체를 구성
	projPoint[0] = _vec3(-1.f, 1.f, 0.f);
	projPoint[1] = _vec3(1.f, 1.f, 0.f);
	projPoint[2] = _vec3(1.f, -1.f, 0.f);
	projPoint[3] = _vec3(-1.f, -1.f, 0.f);

	projPoint[4] = _vec3(-1.f, 1.f, 1.f);
	projPoint[5] = _vec3(1.f, 1.f, 1.f);
	projPoint[6] = _vec3(1.f, -1.f, 1.f);
	projPoint[7] = _vec3(-1.f, -1.f, 1.f);
	return NOERROR;
}

HRESULT Frustum::Ready(void * argument)
{
	return NOERROR;
}

Frustum * Frustum::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	Frustum* instance = new Frustum(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create Frustum");
		SafeRelease(instance);
	}
	return instance;
}

Component * Frustum::Clone(void * argument)
{
	AddRef();

	return this;
}

void Frustum::Free()
{
	SafeRelease(pipeLine);
	Component::Free();
}
