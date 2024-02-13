#pragma once
//������ ����ϱ� ���� �ʱ�ȭ�۾��� �����Ѵ�
//������ ����� ������ �� �����ϱ� ���� ����� �����Ѵ�
//Ŭ���̾�Ʈ���� ������ Ư�� ��ɵ��� �̿��ϰ��� �Ҷ����� ������ â���� �ȴ�
//���� ���ӿ� �������� ���� �ּҸ� �����Ѵ�
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

	//�׷��� ����̽���
	HRESULT MakeGraphicReady(HWND hWnd, GraphicDevice::MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9* graphicDevice);
	//��ǲ����̽�
	HRESULT MakeInputReady(HINSTANCE hInst, HWND hWnd);
	HRESULT SetInputState();

	_byte GetDIKState(_ubyte DIKey);
	_byte GetDIMState(InputDevice::MOUSEBUTTON DIKey);
	_byte GetDIMMove(InputDevice::MOUSEMOVE DIMove);
	//�� �� 
	HRESULT ReadyCurrentScene(Scene* currentScene);
	_int UpdateCurrentScene(_double timeDelta);
	HRESULT RenderCurrentScene();

	//������Ʈ�Ŵ���
	HRESULT AddPrototype(const _tchar* prototypeTag, GameObject* gameObject);
	HRESULT AddGameObject(const _tchar* prototypeTag, GameObject** gameObject, void* arg = nullptr);
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, void* argument = nullptr, _bool isTeleport=false);
	HRESULT GameObjectToLayer(const _tchar* prototypeTag, _uint sceneIndex, const _tchar* layerTag, GameObject** gameObject, void* argument = nullptr, _bool isTeleport=false);
	//const GameObject* GetGameObject(const _tchar * componentTag, const _tchar * layerTag, _uint sceneIndex);
	const Layer* GetLayer(_uint sceneIndex, const _tchar* layerTag);
	const GameObject* GetObjectFromLayer(_uint sceneIndex, const _tchar* layerTag, _uint objectIndex);
	Live* GetLive(_uint sceneIndex, const _tchar* layerTag, _uint objectIndex);
	vector<_vec3> GetTeleportList() { return teleportList; }

	//������Ʈ�Ŵ���
	HRESULT AddPrototype(_uint scene, const _tchar* prototypeTag, Component* component);
	Component* Clone(_uint scene, const _tchar* prototypeTag, void* arg=nullptr);
	const Component* GetComponent(_uint scene, const _tchar * layerTag, const _tchar* componentTag, _uint index=0);

	//Ÿ��
	HRESULT SetRenderTarget(Shader* shader, D3DXHANDLE constantName, const _tchar* targetTag);

	//����������
	_matrix GetTransform(D3DTRANSFORMSTATETYPE transformType);
	_matrix GetInverseTransform(D3DTRANSFORMSTATETYPE transformType);

	//Ÿ�̸�
	HRESULT AddTimer(const _tchar* timerTag);
	_double CalculateTimeDelta(const _tchar* timerTag);

	//����ü
	_bool FrustumCulling(Transform* transform, _float radius = 0.f);
	_bool FrustumCulling(_matrix* mat, _float radius = 0.f);

	//����Ʈ
	HRESULT ReserveLight(_uint lightNum);
	HRESULT AddLight(LPDIRECT3DDEVICE9 graphicDevice, const _LIGHT* light);
	D3DLIGHT9 GetLight(_uint index);

	//����
	HRESULT PlaySFX(const wstring& wstrSoundKey, SoundMNG::CHANNEL_ID ID, _bool loop=false);
	HRESULT PlayBGM(const wstring& wstrSoundKey);
	HRESULT StopSound(SoundMNG::CHANNEL_ID ID);
	HRESULT StopAll();
	HRESULT SetVolume(SoundMNG::CHANNEL_ID ID, float Vol);
	HRESULT SetMusicSpeed(const wstring& wstrSoundKey, float speed);


	//������
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