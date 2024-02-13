#pragma once
#include "Base.h"
BEGIN(Engine)
class Light;
class Shader;
class Manager;
class LightMNG final :
	public Base
{
	DECLARE_SINGLETON(LightMNG)
private:
	Manager* manager = nullptr;
	vector<Light*> lights;
	typedef vector<Light*> LIGHTS;

	vector<_matrix*> lightPos;
	vector<_float> lightRange;

	explicit LightMNG();
	virtual ~LightMNG() = default;

public:
	_LIGHT GetLight(_uint index) const;
	HRESULT LightCulling();
	HRESULT Reserve(_uint lightNum);//const 캐스트를 쓰기 싫으니 리저브를 해둔다
	HRESULT AddLight(LPDIRECT3DDEVICE9 graphicDev, const _LIGHT* light);
	//터레인에서 const 포인터형식으로 넘겨줄수가 없어서 겟라이트를 주소 자체가 아니라 인덱스만 받아오게끔 바꾸고 리저브 함수를 만듬

	HRESULT SetLightPos(_vec3* pos);
	HRESULT Render(Shader* shader);
	virtual void Free();
};

END