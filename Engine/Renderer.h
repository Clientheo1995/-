#pragma once
//�׷��� �� ��ü���� �׸��� ������ ���� �з��Ͽ� ��Ƴ��´�
//��Ƴ��� ��ü���� ������ ���� �׸���
#include "Component.h"
#include "TargetMNG.h"
BEGIN(Engine)
class GameObject;
class Shader;
class LightMNG;
class PipeLine;
class ENGINE_DLL Renderer final:
public Component
{
	public:
	enum RENDERGROUP { PRERENDER, NONALPHA, ALPHA, UI, LAST };
private:
	//Ŭ���̾�Ʈ�� �����ش� ������(���̺귯���� �����) Ŭ�������� ��������� ���ø��� ����ϰ� �Ǹ� ������ �߻���
	//Ŭ�󰳹��ڰ� ��� ����ϴ��Ŀ� ���� �ڵ尡 �þ���� �ִٴ°��� �˸�
	//���������� ���� �����ָ� ������ ���� ����
	list<GameObject*> renderList[LAST];
	typedef list<GameObject*> RENDERLIST;

	//list<GameObject*> lateRenderList;

	_bool renderDebug = false;
	TargetMNG* targetMNG = nullptr;
	LightMNG* lightMNG = nullptr;
	Shader* lightAcc = nullptr;
	Shader* blend = nullptr;
	ViewportRect* buffer = nullptr;
	PipeLine* pipeLine = nullptr;


	explicit Renderer(LPDIRECT3DDEVICE9 graphicDevice);
	virtual ~Renderer() = default;

public:
	virtual HRESULT ReadyProto();
	virtual HRESULT Ready(void* argument);

	void SetRenderDebug(_bool render) { renderDebug = render; }

	HRESULT AddRenderList(RENDERGROUP group, GameObject* gameObject);
	HRESULT AddLateRenderList(GameObject* gameObject);
	HRESULT LateRender();
	HRESULT Render();

	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();

	HRESULT Render_Light();
	HRESULT Render_Blend();

	HRESULT Render_Alpha();
	HRESULT Render_UI();

	static Renderer* Create(LPDIRECT3DDEVICE9 graphicDevice);
	virtual Component* Clone(void* argument);
	virtual void Free();
};
END