#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL Shader final:
public Component
{
private:
	LPD3DXEFFECT effect = nullptr;//셰이더 파일을 컴파일, 셰이더를 이용하기위한 인터페이스를 가지고 있는 객체(셰이더를 대표하는 객체)
	LPD3DXBUFFER errorMessage = nullptr;
	_uint passNum = 0;

	explicit Shader(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Shader(const Shader& object);
	virtual ~Shader() = default;

public:
	virtual HRESULT ReadyProto(const _tchar* shaderFilePath);
	virtual HRESULT Ready(void* argument);

	HRESULT ShaderBegin();
	HRESULT PassBegin(_uint passIndex);
	HRESULT PassEnd();
	HRESULT ShaderEnd();
	HRESULT CommitChange();

	HRESULT SetBool(D3DXHANDLE handle, _bool data);
	HRESULT SetValue(D3DXHANDLE handle, void* data, _uint size);
	HRESULT SetTexture(D3DXHANDLE handle, LPDIRECT3DBASETEXTURE9 texture);

	static Shader* Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar* shaderFilePath);
	virtual Component* Clone(void* argument);
	virtual void Free();
};

END