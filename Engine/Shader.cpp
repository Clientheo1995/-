#include "..\Headers\Shader.h"

Shader::Shader(LPDIRECT3DDEVICE9 graphicDevice):Component(graphicDevice)
{
}

Shader::Shader(const Shader & object) : Component(object), effect(object.effect), errorMessage(object.errorMessage)
{
	AddRefCount(effect);
	AddRefCount(errorMessage);
}

HRESULT Shader::ReadyProto(const _tchar * shaderFilePath)
{
	//이펙트 객체를 생성한다
	if (FAILED(D3DXCreateEffectFromFile(GetGraphic(), shaderFilePath, nullptr, nullptr, 0, nullptr, &effect, &errorMessage)))
	{
		MessageBoxA(0, (char*)errorMessage->GetBufferPointer(), nullptr, MB_OK);
		return E_FAIL;
	}
	return NOERROR;
}

HRESULT Shader::Ready(void * argument)
{
	return NOERROR;
}

HRESULT Shader::ShaderBegin()
{
	if (effect == nullptr)
		return E_FAIL;
	effect->Begin(&passNum, 0);
	return NOERROR;
}


HRESULT Shader::PassBegin(_uint passIndex)
{
	if (effect == nullptr || passNum <= passIndex)
		return E_FAIL;

	effect->BeginPass(passIndex);
	return NOERROR;
}

HRESULT Shader::PassEnd()
{
	if (effect == nullptr)
		return E_FAIL;
	effect->EndPass();
	return NOERROR;
}

HRESULT Shader::ShaderEnd()
{
	if (effect == nullptr)
		return E_FAIL;

	effect->End();//주소 엑세스 오류
	//http://dolphin.ivyro.net/file/hlsl/tutorial05.html
	return NOERROR;
}

HRESULT Shader::CommitChange()
{
	if (effect == nullptr)
		return E_FAIL;
	
	return effect->CommitChanges();
}

HRESULT Shader::SetBool(D3DXHANDLE handle, _bool data)
{
	if (effect == nullptr)
		return E_FAIL;
	return effect->SetBool(handle, data);
}

HRESULT Shader::SetValue(D3DXHANDLE handle, void * data, _uint size)
{
	if (effect == nullptr)
		return E_FAIL;
	return effect->SetValue(handle, data, size);
}

HRESULT Shader::SetTexture(D3DXHANDLE handle, LPDIRECT3DBASETEXTURE9 texture)
{
	if (effect == nullptr)
		return E_FAIL;
	return effect->SetTexture(handle,texture);
}

Shader * Shader::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * shaderFilePath)
{
	Shader* instance = new Shader(graphicDevice);
	if (FAILED(instance->ReadyProto(shaderFilePath)))
	{
		MSG("Failed Create Shader");
		SafeRelease(instance);
	}
	return instance;
}

Component * Shader::Clone(void * argument)
{
	Shader* instance = new Shader(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clone Shader");
		SafeRelease(instance);
	}
	return instance;
}

void Shader::Free()
{
	SafeRelease(errorMessage);
	SafeRelease(effect);

	Component::Free();
}
