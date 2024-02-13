#include "..\Headers\Text.h"
#include "Manager.h"
Text::Text(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
}

Text::Text(const Text & gameObject) : Component(gameObject), font(gameObject.font)
{
	AddRefCount(font);
}

HRESULT Text::SetupConstantTable()
{
	return NOERROR;
}

HRESULT Text::ReadyProtoComponent(LPDIRECT3DDEVICE9 graphicDevice, D3DXFONT_DESC fontType)
{
	HRESULT hr = 0;

	hr = D3DXCreateFontIndirect(graphicDevice, &fontType, &font);

	if (FAILED(hr))
		return E_FAIL;

	//ZeroMemory(&sprite, sizeof(LPD3DXSPRITE));
	//if (sprite == nullptr)
	//return E_FAIL;

	return NOERROR;
}

HRESULT Text::Ready(void * argument)
{
	memcpy(text, (wchar_t*)argument, sizeof(wchar_t)*MAXCHAR);
	return NOERROR;
}

HRESULT Text::Render()
{
	//스프라이트 사이즈를 정해놔야한다
	sprite->SetTransform(worldMat);
	sprite->Begin(D3DXSPRITE_BILLBOARD);
	font->DrawText(NULL, text, -1, &textBox, DT_CENTER, 0xffffff00);
	sprite->End();
	return NOERROR;
}

Text * Text::Create(LPDIRECT3DDEVICE9 graphicDevice, D3DXFONT_DESC fontType)
{
	Text* instance = new Text(graphicDevice);
	if (FAILED(instance->ReadyProtoComponent(graphicDevice, fontType)))
	{
		MSG("Failed Create Text");
		SafeRelease(instance);
	}
	return instance;
}

Component * Text::Clone(void * argument)
{
	Text* instance = new Text(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed clone Text");
		SafeRelease(instance);
	}
	return instance;
}

void Text::Free()
{
	SafeRelease(font);
	Component::Free();
}

LPD3DXFONT Text::GetText(_uint index)
{
	return font;
}
