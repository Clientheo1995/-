#include "GameObject.h"
#include "Manager.h"
#include "Live.h"


Live::Live(LPDIRECT3DDEVICE9 graphicDevice):GameObject(graphicDevice), manager(Manager::GetInstance())
{
	if (manager == nullptr)
		return;
	AddRefCount(manager);
	stat.hp = 0;
	stat.stamina = 0;
}

Live::Live(const Live & gameobject) : GameObject(gameobject), manager(gameobject.manager), stat(gameobject.stat)
{
}

HRESULT Live::AddComponent()
{
	return NOERROR;
}

HRESULT Live::SetupConstantTable()
{
	if (transform == nullptr || shader == nullptr)
		return E_FAIL;

	if (manager == nullptr)
		return E_FAIL;

	if (FAILED(shader->SetValue("world", &transform->GetWorld(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(shader->SetValue("view", &manager->GetTransform(D3DTS_VIEW), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(shader->SetValue("proj", &manager->GetTransform(D3DTS_PROJECTION), sizeof(_matrix))))
		return E_FAIL;

	//D3DLIGHT9 lightOp = manager->GetLight(0);

	//shader->SetValue("lightDir", &lightOp.Direction, sizeof(_vec4));
	//shader->SetValue("lightDiffuse", &lightOp.Diffuse, sizeof(_vec4));
	//shader->SetValue("lightAmbient", &lightOp.Ambient, sizeof(_vec4));
	//shader->SetValue("lightSpecular", &lightOp.Specular, sizeof(_vec4));

	//_matrix view = manager->GetTransform(D3DTS_VIEW);
	//D3DXMatrixInverse(&view, nullptr, &view);

	//shader->SetValue("cameraPos", &view.m[3][0], sizeof(_vec4));

	return NOERROR;
}

HRESULT Live::SetupOnTerrain()
{
	if (manager == nullptr)
		return -1;

	if (terrain == nullptr)
		return -1;
	_vec3 position = transform->GetState(Transform::POSITION);
	position.y = terrain->CalculateY(position);
	transform->SetState(Transform::POSITION, position);
	return NOERROR;
}

HRESULT Live::LateInit()
{
	return NOERROR;
}

HRESULT Live::ReadyProto()
{
	return NOERROR;
}

HRESULT Live::Ready(void * argument)
{
	if (argument == nullptr)
		return NOERROR;

	_vec3 position= *(_vec3*)argument;
	transform->SetState(Transform::POSITION, position);
	return NOERROR;
}

_int Live::Update(_double timeDelta)
{
	return _int();
}

_int Live::LateUpdate(_double timeDelta)
{
	if (render == nullptr)
		return -1;
	if(isRender)
	if(manager->FrustumCulling(transform, 1.f)==false)
	render->AddRenderList(Renderer::NONALPHA, this);
	else
	{
		int a = 0;
	}
	return _int();
}

HRESULT Live::Render()
{
	return NOERROR;
}

_matrix Live::RemoveScale(_matrix * mat)
{
	_matrix matrix;
	memcpy(&matrix, mat, sizeof(_matrix));

	D3DXVec3Normalize((_vec3*)&matrix.m[0][0], (_vec3*)&matrix.m[0][0]);
	D3DXVec3Normalize((_vec3*)&matrix.m[1][0], (_vec3*)&matrix.m[1][0]);
	D3DXVec3Normalize((_vec3*)&matrix.m[2][0], (_vec3*)&matrix.m[2][0]);

	return matrix;
}

HRESULT Live::Parsing()
{
	return NOERROR;
}

Live * Live::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	return nullptr;
}

GameObject * Live::Clone(void * argument)
{
	return nullptr;
}

void Live::Free()
{
	for (auto& coll : colliders)
		SafeRelease(coll);
	colliders.clear();

	SafeRelease(weaponColl);
	SafeRelease(manager);
	SafeRelease(dMesh);
	SafeRelease(transform);
	SafeRelease(shader);
	SafeRelease(render);
	GameObject::Free();
}
