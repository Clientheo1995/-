#include "..\Headers\LightMNG.h"
#include "Light.h"
#include "Shader.h"
#include "Manager.h"
IMPLEMENT_SINGLETON(LightMNG)
LightMNG::LightMNG()
{
	manager = Manager::GetInstance();
}

_LIGHT LightMNG::GetLight(_uint index) const
{
	_LIGHT light;
	ZeroMemory(&light, sizeof(_LIGHT));

	if (lights.size() <= index)
		return light;
	return lights[index]->GetLight();
}

HRESULT LightMNG::LightCulling()
{
	return NOERROR;
}

HRESULT LightMNG::Reserve(_uint lightNum)
{
	lights.reserve(lightNum);

	return NOERROR;
}

HRESULT LightMNG::AddLight(LPDIRECT3DDEVICE9 graphicDev, const _LIGHT * light)
{
	Light* _light = Light::Create(graphicDev, light);
	if (_light == nullptr)
		return E_FAIL;
	_matrix worldMat;
	D3DXMatrixIdentity(&worldMat);
	//D3DXMatrixScaling(&worldMat, 0.015f, 0.015f, 0.015f);
	memcpy(&worldMat.m[3], &light->Position, sizeof(_vec3));

	lightPos.push_back(&worldMat);
	lightRange.push_back(light->Range);
	lights.push_back(_light);
	return NOERROR;
}

HRESULT LightMNG::SetLightPos(_vec3 * pos)
{
	return NOERROR;
}

HRESULT LightMNG::Render(Shader * shader)
{
	if (lights.size()==0)
		return NOERROR;
	lights[0]->Render(shader);
	for (int i = 1; i < lights.size(); i++)
	{
			if (manager == nullptr)
				return NOERROR;
			if (manager->FrustumCulling(lightPos[i], lightRange[i])==false)
				lights[i]->Render(shader);
			else
			{
				int a = 0;
			}
	}
	return NOERROR;
}

void LightMNG::Free()
{
	for (auto& _light : lights)
	{
		SafeRelease(_light);
	}

	lights.clear();
}

