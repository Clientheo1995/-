#include "UI.h"
#include "Manager.h"

UI::UI(LPDIRECT3DDEVICE9 graphicDevice) :GameObject(graphicDevice)
{
	ZeroMemory(&setUp, sizeof(UISETUP));
}

UI::UI(const UI & back) : GameObject(back),
transform(back.transform),
shaderComponent(back.shaderComponent),
textureComponent(back.textureComponent),
renderComponent(back.renderComponent),
bufferComponent(back.bufferComponent)
{
}

HRESULT UI::AddComponent()
{
	setUp.is2D = true;
	setUp.x = 1.f;
	setUp.y = 1.f;
	setUp.sizeX = 1.f;
	setUp.sizeY = 1.f;
	setUp.wincx = 1.f;
	setUp.wincy = 1.f;

	state = _vec3(1.f, 1.f, 1.f);
	scale = _vec3(1.f, 1.f, 1.f);
	return NOERROR;
}
HRESULT UI::SetupConstantTable()
{
	if (textureComponent == nullptr)
		return E_FAIL;
	if (setUp.is2D)
	{
		_matrix transformMat;
		D3DXMatrixIdentity(&transformMat);

		_matrix projMat;
		D3DXMatrixOrthoLH(&projMat, setUp.wincx, setUp.wincy, 0.f, 1.f);

		shaderComponent->SetValue("world", transform->GetWorldMatPointer(), sizeof(_matrix));
		shaderComponent->SetValue("view", &transformMat, sizeof(_matrix));
		shaderComponent->SetValue("proj", &projMat, sizeof(_matrix));

		shaderComponent->SetTexture("DiffuseTexture", textureComponent->GetTexture(setUp.textureNum));
	}
	else
	{
		Manager* manager = Manager::GetInstance();
		if (manager == nullptr)
			return E_FAIL;

		AddRefCount(manager);

		shaderComponent->SetValue("world", transform->GetWorldMatPointer(), sizeof(_matrix));
		shaderComponent->SetValue("view", &manager->GetTransform(D3DTS_VIEW), sizeof(_matrix));
		shaderComponent->SetValue("proj", &manager->GetTransform(D3DTS_PROJECTION), sizeof(_matrix));

		shaderComponent->SetTexture("DiffuseTexture", textureComponent->GetTexture(setUp.textureNum));
		SafeRelease(manager);
	}
	return NOERROR;
}
HRESULT UI::LateInit()
{
	return NOERROR;
}

HRESULT UI::SetType(UITYPE type)
{
	this->type = type;
	return NOERROR;
}

HRESULT UI::SetBillBoard()
{
	Manager* manager = Manager::GetInstance();
	if (manager == nullptr)
		return E_FAIL;

	AddRefCount(manager);
	_matrix viewMat = manager->GetTransform(D3DTS_VIEW);
	D3DXMatrixInverse(&viewMat, nullptr, &viewMat);

	_vec3 right = *(_vec3*)&viewMat.m[0][0];
	_vec3 up = *(_vec3*)&viewMat.m[1][0];
	_vec3 look = *(_vec3*)&viewMat.m[2][0];

	transform->SetState(Transform::RIGHT, right*transform->GetScale(Transform::RIGHT));//스케일값을 덮어씌우기때문에 여기서 곱해주면 적용된다
	transform->SetState(Transform::UP, up*transform->GetScale(Transform::UP));//y축으로만 회전시키고 싶다면 주석처리 - 카메라가 위에 있을땐 위를 바라보지 않는다
	transform->SetState(Transform::LOOK, look*transform->GetScale(Transform::LOOK));
	SafeRelease(manager);
	return NOERROR;
}

UISETUP UI::GetUISet()
{
	return setUp;
}

void UI::SetUp(UISETUP uiset)
{
	setUp = uiset;
	int winx = setUp.wincx;
	int winy = setUp.wincy;
	state = _vec3(setUp.x - (winx >> 1), (winy >> 1) - setUp.y, 0.f);
	scale = _vec3(setUp.sizeX, setUp.sizeY, 1.f);
}

HRESULT UI::ReadyProto()
{
	return NOERROR;
}

HRESULT UI::ReadyGameObject(void* argument)
{
	if (FAILED(AddComponent()))
		return E_FAIL;

	return NOERROR;
}

//void UI::SetTextRect(RECT rc)
//{
//textComponent->SetRect(rc);
//}

_bool UI::IsClicked(HWND hWnd)
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		RECT rect;
		SetRect(&rect, setUp.x - setUp.sizeX * 0.5f, setUp.y - setUp.sizeY * 0.5f, setUp.x + setUp.sizeX * 0.5f, setUp.y + setUp.sizeY * 0.5f);

		if (PtInRect(&rect, mouse))
			return true;
	}
	return false;
}

_int UI::Update(_double timeDelta)
{
	return _int();
}
_int UI::LateUpdate(_double timeDelta)
{
	if (renderComponent == nullptr)
		return -1;

	transform->SetState(Transform::POSITION, state);
	transform->SetScale(scale);
	//renderComponent->AddRenderList(Renderer::UI, this);
	return _int();
}

HRESULT UI::Render(_int shaderNum)
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

UI * UI::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	UI* instance = new UI(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create UI");
		SafeRelease(instance);
	}
	return instance;
}

GameObject * UI::Clone(void * argument)
{
	UI* instance = new UI(*this);
	if (FAILED(instance->ReadyGameObject(argument)))
	{
		MSG("Failed Clone UI");
		SafeRelease(instance);
	}
	return instance;
}

void UI::Free()
{
	SafeRelease(shaderComponent);
	SafeRelease(bufferComponent);
	SafeRelease(textureComponent);
	SafeRelease(renderComponent);
	SafeRelease(transform);
	GameObject::Free();
}
