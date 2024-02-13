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
	HRESULT Reserve(_uint lightNum);//const ĳ��Ʈ�� ���� ������ �����긦 �صд�
	HRESULT AddLight(LPDIRECT3DDEVICE9 graphicDev, const _LIGHT* light);
	//�ͷ��ο��� const �������������� �Ѱ��ټ��� ��� �ٶ���Ʈ�� �ּ� ��ü�� �ƴ϶� �ε����� �޾ƿ��Բ� �ٲٰ� ������ �Լ��� ����

	HRESULT SetLightPos(_vec3* pos);
	HRESULT Render(Shader* shader);
	virtual void Free();
};

END