#pragma once
#include "GameObject.h"

BEGIN(Engine)
class Shader;
class Texture;
class Renderer;
class Transform;
class VRect;

class ENGINE_DLL Item:
	public GameObject
{
private:
	_int textureNum;
	_int maxUse;
	UISETUP setUp;
	_vec3 state, scale;

	_tchar name[MAXCHAR];
	_tchar textureTag[MAXCHAR];
	_tchar meshTag[MAXCHAR];

	Transform* transform = nullptr;
	Shader* shaderComponent = nullptr;
	Texture* textureComponent = nullptr;
	Renderer* renderComponent = nullptr;
	VRect* bufferComponent = nullptr;

	explicit Item(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Item(const Item& back);
	virtual ~Item() = default;
public:
	virtual HRESULT ReadyProto();
	virtual HRESULT ReadyGameObject();
	virtual HRESULT LateInit();

	void SetUp(UISETUP Itemset);
	void SetState(_vec3 state) { this->state = state; }
	void SetScale(_vec3 scale) { this->scale = scale; }

	void SetNameTag(const _tchar* itemName, const _tchar* textureTag=nullptr, const _tchar* meshTag=nullptr) { 
		memcpy(&name, itemName, sizeof(_tchar)*MAXCHAR); 

		if (textureTag == nullptr)
			return;
		memcpy(&this->textureTag, textureTag, sizeof(_tchar)*MAXCHAR);

		if (meshTag == nullptr)
			return;
		memcpy(&this->meshTag, meshTag, sizeof(_tchar)*MAXCHAR);
	};

	void SetTransform(Transform* comp) {transform=comp; }
	void SetShader(Shader* comp) { shaderComponent=comp; }
	void SetTexture(Texture* comp) { textureComponent = comp; }
	void SetRenderer(Renderer* comp) { renderComponent = comp; }
	void SetVRect(VRect* comp) { bufferComponent = comp; }

	const _tchar* GetName() { return name; }
	const _tchar* GetTextureTag() { return textureTag; }

	HRESULT AddComponent();//GameObject AddComponent 로 각각 추가해주면 된다
	HRESULT SetupConstantTable();
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render(_int shaderNum);

	static Item* Create(LPDIRECT3DDEVICE9 graphicDevice);//원본생성
	virtual GameObject* Clone(void* argument);
	virtual void Free();
};

END