#include "Item.h"
#include "Manager.h"

Item::Item(LPDIRECT3DDEVICE9 graphicDevice):GameObject(graphicDevice)
{
}

Item::Item(const Item & back):GameObject(back),
transform(back.transform),
shaderComponent(back.shaderComponent),
textureComponent(back.textureComponent),
renderComponent(back.renderComponent),
bufferComponent(back.bufferComponent)
{
}

HRESULT Item::AddComponent()
{
	textureNum = 0;
	maxUse = 0;
	setUp.x = 1.f;
	setUp.y = 1.f;
	setUp.sizeX = 1.f;
	setUp.sizeY = 1.f;
	setUp.wincx = 1.f;
	setUp.wincy = 1.f;

	state = _vec3(1.f, 1.f, 1.f);
	scale= _vec3(1.f, 1.f, 1.f);
	return NOERROR;
}
HRESULT Item::SetupConstantTable()
{
	if (textureComponent == nullptr)
		return E_FAIL;
	_matrix transformMat;
	D3DXMatrixIdentity(&transformMat);

	_matrix projMat;
	D3DXMatrixOrthoLH(&projMat, setUp.wincx, setUp.wincy, 0.f, 1.f);

	shaderComponent->SetValue("world",transform->GetWorldMatPointer(), sizeof(_matrix));
	shaderComponent->SetValue("view", &transformMat, sizeof(_matrix));
	shaderComponent->SetValue("proj", &projMat, sizeof(_matrix));

	shaderComponent->SetTexture("DiffuseTexture", textureComponent->GetTexture(setUp.textureNum));
	return NOERROR;
}
HRESULT Item::LateInit()
{
	return NOERROR;
}

void Item::SetUp(UISETUP Itemset)
{
	setUp = Itemset;
	int winx = setUp.wincx;
	int winy = setUp.wincy;
	state = _vec3(setUp.x - (winx >> 1), (winy >> 1) - setUp.y, 0.f);
	scale = _vec3(setUp.sizeX, setUp.sizeY, 1.f);
}

HRESULT Item::ReadyProto()
{
	return NOERROR;
}

HRESULT Item::ReadyGameObject()
{
	if (FAILED(AddComponent()))
		return E_FAIL;

	return NOERROR;
}

_int Item::Update(_double timeDelta)
{
	if (maxUse != 0)
	{
	}
	return _int();
}
_int Item::LateUpdate(_double timeDelta)
{
	if (renderComponent == nullptr)
		return -1;

	transform->SetState(Transform::POSITION, state);
	transform->SetScale(scale);
	return _int();
}

HRESULT Item::Render(_int shaderNum)
{
	if (bufferComponent == nullptr || shaderComponent == nullptr)
		return E_FAIL;

	if (FAILED(SetupConstantTable()))
		return E_FAIL;

	shaderComponent->ShaderBegin();
	shaderComponent->PassBegin(shaderNum);

	bufferComponent->Render();

	shaderComponent->PassEnd();
	shaderComponent->ShaderEnd();
	return NOERROR;
}

Item * Item::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	Item* instance = new Item(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create Item");
		SafeRelease(instance);
	}
	return instance;
}

GameObject * Item::Clone(void * argument)
{
	Item* instance = new Item(*this);
	if (FAILED(instance->ReadyGameObject()))
	{
		MSG("Failed Clone Item");
		SafeRelease(instance);
	}
	return instance;
}

void Item::Free()
{
	SafeRelease(shaderComponent);
	SafeRelease(bufferComponent);
	SafeRelease(textureComponent);
	SafeRelease(renderComponent);
	SafeRelease(transform);
	GameObject::Free();
}
