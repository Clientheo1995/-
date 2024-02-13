#include "..\Headers\Light.h"
#include "ViewportRect.h"
#include "Shader.h"

Light::Light(LPDIRECT3DDEVICE9 graphicDev):graphicDev(graphicDev)
{
	AddRefCount(graphicDev);
}

HRESULT Light::Ready(const _LIGHT * light)
{
	this->light = *light;
	D3DVIEWPORT9 view;
	graphicDev->GetViewport(&view);
	buffer = ViewportRect::Create(graphicDev, 0.f - 0.5f, 0.f - 0.5f, view.Width, view.Height);
	if (buffer == nullptr)
		return E_FAIL;
	return NOERROR;
}

_int Light::Update(_double timeDelta)
{
	return _int();
}

HRESULT Light::Render(Shader * shader)
{
	if (buffer == nullptr)
		return E_FAIL;

	_uint passIndex = 0;
	if (light.Type == D3DLIGHT_DIRECTIONAL)
	{
		passIndex = 0;
		shader->SetValue("lightDir", &_vec4(light.Direction, 0.f), sizeof(_vec4));
	}
	if (light.Type == D3DLIGHT_POINT)
	{
		passIndex = 1;
		shader->SetValue("lightPos", &_vec4(light.Position, 1.f), sizeof(_vec4));
		shader->SetValue("range", &light.Range, sizeof(_float));
	}
	if (light.Type == D3DLIGHT_SPOT)
	{
		passIndex=2;
		shader->SetValue("lightPos", &_vec4(light.Position, 1.f), sizeof(_vec4));
		shader->SetValue("range", &light.Range, sizeof(_float));
		shader->SetValue("height", &light.height, sizeof(_float));
		shader->SetValue("attenuation0", &light.Attenuation0, sizeof(_float));
		shader->SetValue("attenuation1", &light.Attenuation1, sizeof(_float));
		shader->SetValue("attenuation2", &light.Attenuation2, sizeof(_float));
	}
	shader->SetValue("lightDiffuse", &light.Diffuse, sizeof(_vec4));
	shader->SetValue("lightAmbient", &light.Ambient, sizeof(_vec4));
	shader->SetValue("lightSpecular", &light.Specular, sizeof(_vec4));
	shader->CommitChange();

	shader->PassBegin(passIndex);
	buffer->Render();
	shader->PassEnd();
	return NOERROR;
}

Light * Light::Create(LPDIRECT3DDEVICE9 graphicDevice, const _LIGHT * light)
{
	Light* instance = new Light(graphicDevice);
	if (FAILED(instance->Ready(light)))
	{
		MSG("Failed Create Light");
		SafeRelease(instance);
	}
	return instance;
}

void Light::Free()
{
	SafeRelease(buffer);
	SafeRelease(graphicDev);
}
