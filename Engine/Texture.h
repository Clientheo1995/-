#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL Texture final:public Component
{
	//텍스처를 호출할때마다 원본주소를 반환한다 - 미리 만들어두었기 때문에 이후 호출할때마다 검색하지 않아도 된다
public:
	enum TEXTURETYPE {GENERAL, CUBE, _END};
private:
	vector<IDirect3DBaseTexture9*> textures;
	typedef vector<IDirect3DBaseTexture9*> TEXTURES;

	explicit Texture(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Texture(const Texture& gameObject);
	virtual ~Texture() = default;

public:
	virtual HRESULT ReadyProtoComponent(TEXTURETYPE type, const _tchar* filePath, _uint numTexture);
	virtual HRESULT Ready(void* argument);

	static Texture* Create(LPDIRECT3DDEVICE9 graphicDevice, TEXTURETYPE type, const _tchar* filePath, _uint numTexture=1);
	virtual Component* Clone(void* argument);
	virtual void Free();
	IDirect3DBaseTexture9* GetTexture(_uint index);
};

END