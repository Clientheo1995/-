#pragma once
//엔진을 사용하기 위한 초기화작업을 수행한다
//엔진의 사용이 끝났을 때 정리하기 위한 기능을 수행한다
//클라이언트에서 엔진의 특정 기능들을 이용하고자 할때마다 소통의 창구가 된다
//현재 게임에 보여지는 씬의 주소를 보관한다
#include "GraphicDevice.h"
#include "InputDevice.h"
#include "SoundMNG.h"
#include "LightMNG.h"
#include "TimerMNG.h"
#include "ObjectManager.h"
#include "ComponentMNG.h"
#include "TargetMNG.h"
#include "ItemManager.h"
#include "PipeLine.h"
BEGIN(Engine)
class Live;
class Scene;
class Frustum;
class ENGINE_DLL Manager final:public Base
{
	DECLARE_SINGLETON(Manager)
private:
	GraphicDevice* graphicDev = nullptr;
	InputDevice* inputDev = nullptr;
	Scene* currentScene = nullptr;
	ObjectManager* objectMNG = nullptr;
	ComponentMNG* componentMNG = nullptr;
	PipeLine* pipeLine = nullptr;
	TimerMNG* timerMNG = nullptr;
	LightMNG* lightMNG = nullptr;
	SoundMNG* soundMNG = nullptr;
	TargetMNG* targetMNG = nullptr;
	ItemManager* itemMNG = nullptr;
	Frustum* frustum = nullptr;

	vector<_vec3> teleportList;
	explicit Manager();
	virtual ~Manager() = default;

public:
	HRESULT EngineReady(_uint numScene);

	//그래픽 디바이스용
	HRESULT MakeGraphicReady(HWND hWnd, GraphicDevice::MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9* graphicDevice);
	//인풋디바이스
	HRESULT MakeInputReady(HINSTANCE hInst, HWND hWnd);
	HRESULT SetInputState();

	_byte GetDIKState(_ubyte DIKey);
	_byte GetDIMState(InputDevice::MOUSEBUTTON DIKey);
	_byte GetDIMMove(InputDevice::MOUSEMOVE DIMove);
	//씬 용 
	HRESULT ReadyCurrentScene(Scene* currentScene);
	_int UpdateCurrentScene(_double timeDelta);
	HRESULT RenderCurrentScene();

	//오브젝트매니저
	HRESULT AddPrototype(const _tchar* prototypeTag, GameObject* gameObject);
	HRESULT AddGameObject(const _tchar* prototypeTag, GameObject** gameObject, void* arg = nullptr);
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, void* argument = nullptr, _bool isTeleport=false);
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, GameObject** gameObject, void* argument = nullptr, _bool isTeleport=false);
	//const GameObject* GetGameObject(const _tchar * componentTag, const _tchar * layerTag, _uint sceneIndex);
	const Layer* GetLayer(_uint sceneIndex, const _tchar* layerTag);
	const GameObject* GetObjectFromLayer(_uint sceneIndex, const _tchar* layerTag, _uint objectIndex);
	Live* GetLive(_uint sceneIndex, const _tchar* layerTag, _uint objectIndex);
	vector<_vec3> GetTeleportList() { return teleportList; }

	//컴포넌트매니저
	HRESULT AddPrototype(_uint scene, const _tchar* prototypeTag, Component* component);
	Component* Clone(_uint scene, const _tchar* prototypeTag, void* arg=nullptr);
	const Component* GetComponent(_uint scene, const _tchar * layerTag, const _tchar* componentTag, _uint index=0);

	//타겟
	HRESULT SetRenderTarget(Shader* shader, D3DXHANDLE constantName, const _tchar* targetTag);

	//파이프라인
	_matrix GetTransform(D3DTRANSFORMSTATETYPE transformType);
	_matrix GetInverseTransform(D3DTRANSFORMSTATETYPE transformType);

	//타이머
	HRESULT AddTimer(const _tchar* timerTag);
	_double CalculateTimeDelta(const _tchar* timerTag);

	//절두체
	_bool FrustumCulling(Transform* transform, _float radius = 0.f);
	_bool FrustumCulling(_matrix* mat, _float radius = 0.f);

	//라이트
	HRESULT ReserveLight(_uint lightNum);
	HRESULT AddLight(LPDIRECT3DDEVICE9 graphicDevice, const _LIGHT* light);
	D3DLIGHT9 GetLight(_uint index);

	//사운드
	HRESULT PlaySFX(const wstring& wstrSoundKey, SoundMNG::CHANNEL_ID ID, _bool loop=false);
	HRESULT PlayBGM(const wstring& wstrSoundKey);
	HRESULT StopSound(SoundMNG::CHANNEL_ID ID);
	HRESULT StopAll();
	HRESULT SetVolume(SoundMNG::CHANNEL_ID ID, float Vol);
	HRESULT SetMusicSpeed(const wstring& wstrSoundKey, float speed);


	//아이템
	_uint GetInvenSize();
	vector<GameObject*> GetItems();
	HRESULT AddItem(const _tchar* itemTag, GameObject* item);
	HRESULT DeleteItem(const _tchar* itemTag);
	HRESULT SortItem();

	HRESULT LateInit();
	HRESULT ClearScene(_uint scene);
	static HRESULT EngineRelease();

	virtual void Free();
};

END