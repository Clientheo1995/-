#pragma once
//그려야 할 객체들을 그리는 순서에 따라 분류하여 모아놓는다
//모아놓은 객체들을 순서에 따라 그린다
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
	//클라이언트로 보여준다 정의한(라이브러리로 등록한) 클래스에서 멤버변수로 템플릿을 사용하게 되면 워닝이 발생함
	//클라개발자가 어떻게 사용하느냐에 따라 코드가 늘어날수도 있다는것을 알림
	//직접적으로 선언만 안해주면 워닝이 뜨지 않음
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