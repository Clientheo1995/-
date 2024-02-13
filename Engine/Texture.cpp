#include "..\Headers\Texture.h"

Texture::Texture(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
}

Texture::Texture(const Texture & gameObject) : Component(gameObject), textures(gameObject.textures)
{
	for (auto& texture : textures)
		AddRefCount(texture);
}

HRESULT Texture::ReadyProtoComponent(TEXTURETYPE type, const _tchar * filePath, _uint numTexture)
{
	_tchar tfilePath[MAX_PATH] = L"";
	textures.reserve(numTexture);
	for (size_t i = 0; i < numTexture; i++)
	{
		IDirect3DBaseTexture9* texture = nullptr;
		wsprintf(tfilePath, filePath, i);
		HRESULT hr = 0;
		switch (type)
		{
		case Texture::CUBE:
			hr = D3DXCreateCubeTextureFromFile(GetGraphic(), tfilePath, (LPDIRECT3DCUBETEXTURE9*)&texture);
			break;
		case Texture::GENERAL:
			hr = D3DXCreateTextureFromFile(GetGraphic(), tfilePath, (LPDIRECT3DTEXTURE9*)&texture);//포인터의 포인터로 넣어준다 어차피 전부 4바이트 포인터니까
			break;
		}
		if (FAILED(hr))
			return E_FAIL;

		textures.push_back(texture);
	}
	return NOERROR;
}

HRESULT Texture::Ready(void * argument)
{
	return NOERROR;
}

Texture * Texture::Create(LPDIRECT3DDEVICE9 graphicDevice, TEXTURETYPE type, const _tchar * filePath, _uint numTexture)
{
	Texture* instance = new Texture(graphicDevice);
	if (FAILED(instance->ReadyProtoComponent(type, filePath, numTexture)))
	{
		MSG("Failed Create Texture");
		SafeRelease(instance);
	}
	return instance;
}

Component * Texture::Clone(void * argument)
{
	Texture* instance = new Texture(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed clone Texture");
		SafeRelease(instance);
	}
	return instance;
}

void Texture::Free()
{
	for (auto& texture : textures)
		SafeRelease(texture);
	textures.clear();
	Component::Free();
}

IDirect3DBaseTexture9 * Texture::GetTexture(_uint index)
{
	if (textures.size() <= index)
		return nullptr;
	return textures[index];
}
