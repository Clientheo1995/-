#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL Shader final:
public Component
{
private:
	LPD3DXEFFECT effect = nullptr;//���̴� ������ ������, ���̴��� �̿��ϱ����� �������̽��� ������ �ִ� ��ü(���̴��� ��ǥ�ϴ� ��ü)
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