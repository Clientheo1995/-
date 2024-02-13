#pragma once
#include "GameObject.h"
BEGIN(Engine)
class Shader;
class Texture;
class Renderer;
class Transform;
class VRect;
//class Text;
class ENGINE_DLL UI:
	public GameObject
{
public:
	enum UITYPE {TEXTURE, TEXT, NONE};
private:
	UISETUP setUp;
	UITYPE type;
	_vec3 state, scale;
	Transform* transform = nullptr;
	Shader* shaderComponent = nullptr;
	Texture* textureComponent = nullptr;
	Renderer* renderComponent = nullptr;
	VRect* bufferComponent = nullptr;
	//Text* textComponent = nullptr;

	explicit UI(LPDIRECT3DDEVICE9 graphicDevice);
	explicit UI(const UI& back);
	virtual ~UI() = default;
public:
	virtual HRESULT ReadyProto();
	virtual HRESULT ReadyGameObject(void* argument);
	virtual HRESULT LateInit();
	virtual HRESULT SetType(UITYPE type);
	HRESULT SetBillBoard();

	UISETUP GetUISet();
	void SetUp(UISETUP uiset);
	void SetState(_vec3 state) { this->state = state; }
	void SetScale(_vec3 scale) { this->scale = scale; }
	void SetTransform(Transform* comp) {transform=comp; }
	void SetShader(Shader* comp) { shaderComponent=comp; }
	void SetTexture(Texture* comp) { textureComponent = comp; }
	void SetRenderer(Renderer* comp) { renderComponent = comp; }
	void SetVRect(VRect* comp) { bufferComponent = comp; }
	//void SetText(Text* comp) { textComponent = comp; }
	//void SetTextRect(RECT rc);//문자를 그리고 싶다면 여기서 전부 초기 설정을 해줘야 한다

	virtual _bool IsClicked(HWND hWnd);
	HRESULT AddComponent();//GameObject AddComponent 로 각각 추가해주면 된다
	HRESULT SetupConstantTable();
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render(_int shaderNum=0);

	static UI* Create(LPDIRECT3DDEVICE9 graphicDevice);//원본생성
	virtual GameObject* Clone(void* argument);
	virtual void Free();
};

END