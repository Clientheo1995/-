#include "../Headers/Manager.h"
#include "Scene.h"
#include "Layer.h"
#include "Live.h"
IMPLEMENT_SINGLETON(Manager)

Manager::Manager()
{
}

HRESULT Manager::MakeGraphicReady(HWND hWnd, GraphicDevice::MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9 * graphicDevice)
{
	if (graphicDev == nullptr)
		return E_FAIL;
	if (FAILED(graphicDev->MakeGraphicReady(hWnd, mode, wincx, wincy, graphicDevice)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Manager::MakeInputReady(HINSTANCE hInst, HWND hWnd)
{
	if (inputDev == nullptr)
		return E_FAIL;

	return inputDev->ReadyDevice(hInst, hWnd);
}

HRESULT Manager::SetInputState()
{
	if (inputDev == nullptr)
		return E_FAIL;

	return inputDev->SetState();
}

_byte Manager::GetDIKState(_ubyte DIKey)
{
	if (inputDev == nullptr)
		return 0;
	return _byte(inputDev->GetDIKState(DIKey));
}

_byte Manager::GetDIMState(InputDevice::MOUSEBUTTON DIKey)
{
	if (inputDev == nullptr)
		return 0;
	return _byte(inputDev->GetDIMState(DIKey));
}

_byte Manager::GetDIMMove(InputDevice::MOUSEMOVE DIMove)
{
	if (inputDev == nullptr)
		return 0;
	return _byte(inputDev->GetDIMMove(DIMove));
}

HRESULT Manager::ReadyCurrentScene(Scene * currentScene)
{
	if (nullptr == currentScene)
		return E_FAIL;

	if (SafeRelease(this->currentScene) != 0)
	{
		MSG("Failed release currentScene");
		return E_FAIL;
	}
	this->currentScene = currentScene;
	AddRefCount(currentScene);
	return NOERROR;
}

_int Manager::UpdateCurrentScene(_double timeDelta)
{
	if (currentScene == nullptr || objectMNG == nullptr)
		return -1;
	_int updateEvent = 0;

	objectMNG->Update(timeDelta);
	objectMNG->LateUpdate(timeDelta);

	updateEvent = currentScene->Update(timeDelta);
	if (updateEvent & 0x80000000)
		return updateEvent;
	updateEvent = currentScene->LateUpdate(timeDelta);
	if (updateEvent & 0x80000000)
		return updateEvent;

	return _int();
}

HRESULT Manager::RenderCurrentScene()
{
	if (currentScene == nullptr)
		return E_FAIL;

	return currentScene->Render();
}

HRESULT Manager::AddPrototype(const _tchar * prototypeTag, GameObject * gameObject)
{
	if (objectMNG == nullptr)
		return E_FAIL;
	return objectMNG->AddPrototype(prototypeTag, gameObject);
}

HRESULT Manager::AddGameObject(const _tchar * prototypeTag, GameObject ** gameObject, void* argument)
{
	if (objectMNG == nullptr)
		return E_FAIL;
	return objectMNG->AddGameObject(prototypeTag, gameObject, argument);
}

HRESULT Manager::GameObjectToLayer(const _tchar * prototypeTag, _uint sceneIndex, const _tchar * layerTag, void * argument, _bool isTeleport)
{
	if (objectMNG == nullptr)
		return E_FAIL;

	//화톳불 텔레포트용 추가한 갯수만큼 추가
	if (isTeleport)
	{
		_vec3 position[2];
		memcpy(&position, argument, sizeof(_vec3[2]));
		//teleportList.push_back(position[0]);
	}
	return objectMNG->GameObjectToLayer(prototypeTag, sceneIndex, layerTag, argument);
}

HRESULT Manager::GameObjectToLayer(const _tchar * prototypeTag, _uint sceneIndex, const _tchar * layerTag, GameObject ** gameObject, void * argument, _bool isTeleport)
{
	if (objectMNG == nullptr)
		return E_FAIL;

	//화톳불 텔레포트용 추가한 갯수만큼 추가
	if (isTeleport)
	{
		_vec3 position[2];
		memcpy(&position, argument, sizeof(_vec3[2]));
		//teleportList.push_back(position[0]);
	}
	return objectMNG->GameObjectToLayer(prototypeTag, sceneIndex, layerTag, gameObject, argument);
}

//const GameObject * Manager::GetGameObject(const _tchar * componentTag, const _tchar * layerTag, _uint sceneIndex)
//{
//	if (objectMNG == nullptr)
//	return nullptr;
//	return objectMNG->GetGameObject(componentTag, layerTag, sceneIndex);
//}

const Layer * Manager::GetLayer(_uint sceneIndex, const _tchar * layerTag)
{
	if (objectMNG == nullptr)
		return nullptr;
	return objectMNG->GetLayer(sceneIndex, layerTag);
}

const GameObject * Manager::GetObjectFromLayer(_uint sceneIndex, const _tchar * layerTag, _uint objectIndex)
{
	if (objectMNG == nullptr)
		return nullptr;
	Layer* layer = objectMNG->GetLayer(sceneIndex, layerTag);
	if (layer == nullptr)
		return nullptr;
	return layer->GetGameObject(objectIndex);
}
Live * Manager::GetLive(_uint sceneIndex, const _tchar * layerTag, _uint objectIndex)
{
	if (objectMNG == nullptr)
		return nullptr;
	Layer* layer = objectMNG->GetLayer(sceneIndex, layerTag);
	if (layer == nullptr)
		return nullptr;
	return (Live*)layer->GetGameObject(objectIndex);
}

HRESULT Manager::AddPrototype(_uint scene, const _tchar * prototypeTag, Component * component)
{
	if (componentMNG == nullptr)
		return E_FAIL;
	return componentMNG->AddPrototype(scene, prototypeTag, component);
}

Component* Manager::Clone(_uint scene, const _tchar * prototypeTag, void * arg)
{
	if (componentMNG == nullptr)
		return nullptr;
	return componentMNG->Clone(scene, prototypeTag, arg);
}

const Component * Manager::GetComponent(_uint scene, const _tchar * layerTag, const _tchar* componentTag, _uint index)
{
	if (objectMNG == nullptr)
		return nullptr;
	return objectMNG->GetComponent(scene, layerTag, componentTag, index);
}

HRESULT Manager::SetRenderTarget(Shader * shader, D3DXHANDLE constantName, const _tchar * targetTag)
{
	if (targetMNG == nullptr)
		return E_FAIL;
	return targetMNG->SetShader(shader, constantName, targetTag);
}

_matrix Manager::GetTransform(D3DTRANSFORMSTATETYPE transformType)
{
	_matrix temp;
	D3DXMatrixIdentity(&temp);

	if (pipeLine == nullptr)
		return temp;
	return pipeLine->GetTransform(transformType);
}

_matrix Manager::GetInverseTransform(D3DTRANSFORMSTATETYPE transformType)
{
	_matrix temp;
	D3DXMatrixIdentity(&temp);

	if (pipeLine == nullptr)
		return temp;
	temp = pipeLine->GetTransform(transformType);
	D3DXMatrixInverse(&temp, NULL, &temp);
	return temp;
}

HRESULT Manager::AddTimer(const _tchar * timerTag)
{
	if (timerMNG == nullptr)
		return E_FAIL;
	return timerMNG->AddTimer(timerTag);
}

_double Manager::CalculateTimeDelta(const _tchar * timerTag)
{
	if (timerMNG == nullptr)
		return E_FAIL;
	return timerMNG->CalculateTimeDelta(timerTag);
}

_bool Manager::FrustumCulling(Transform * transform, _float radius)
{
	if (lightMNG == nullptr)
		return false;
	return frustum->FrustumCulling(transform, radius);
}

_bool Manager::FrustumCulling(_matrix * mat, _float radius)
{
	if (lightMNG == nullptr)
		return false;
	return frustum->FrustumCulling(mat, radius);
}

HRESULT Manager::ReserveLight(_uint lightNum)
{
	if (lightMNG == nullptr)
		return E_FAIL;
	return lightMNG->Reserve(lightNum);
}

HRESULT Manager::AddLight(LPDIRECT3DDEVICE9 graphicDevice, const _LIGHT * light)
{
	if (lightMNG == nullptr)
		return E_FAIL;
	return lightMNG->AddLight(graphicDevice, light);
}

D3DLIGHT9 Manager::GetLight(_uint index)
{
	D3DLIGHT9 light_;
	ZeroMemory(&light_, sizeof(D3DLIGHT9));
	if (lightMNG == nullptr)
		return light_;
	return lightMNG->GetLight(index);
}

HRESULT Manager::PlaySFX(const wstring & wstrSoundKey, SoundMNG::CHANNEL_ID ID, _bool loop)
{
	if (soundMNG == nullptr)
		return E_FAIL;
	return soundMNG->PlaySFX(wstrSoundKey, ID, loop);
}

HRESULT Manager::PlayBGM(const wstring & wstrSoundKey)
{
	if (soundMNG == nullptr)
		return E_FAIL;
	return soundMNG->PlayBGM(wstrSoundKey);
}

HRESULT Manager::StopSound(SoundMNG::CHANNEL_ID ID)
{
	if (soundMNG == nullptr)
		return E_FAIL;
	return soundMNG->StopSound(ID);
}

HRESULT Manager::StopAll()
{
	if (soundMNG == nullptr)
		return E_FAIL;
	return soundMNG->StopAll();
}

HRESULT Manager::SetVolume(SoundMNG::CHANNEL_ID ID, float Vol)
{
	if (soundMNG == nullptr)
		return E_FAIL;
	return soundMNG->SetVolume(ID, Vol);
}

HRESULT Manager::SetMusicSpeed(const wstring & wstrSoundKey, float speed)
{
	return soundMNG->SetMusicSpeed(wstrSoundKey, speed);
}

_uint Manager::GetInvenSize()
{
	return itemMNG->GetInvenSize();
}

vector<GameObject*> Manager::GetItems()
{
	return itemMNG->GetItems();
}

HRESULT Manager::AddItem(const _tchar * itemTag, GameObject * item)
{
	return itemMNG->AddItem(itemTag, item);
}

HRESULT Manager::DeleteItem(const _tchar * itemTag)
{
	return itemMNG->DeleteItem(itemTag);
}

HRESULT Manager::SortItem()
{
	return itemMNG->SortItem();
}

HRESULT Manager::LateInit()
{
	if (objectMNG == nullptr)
		return E_FAIL;
	objectMNG->LateInit();
	return NOERROR;
}

HRESULT Manager::ClearScene(_uint scene)
{
	if (componentMNG == nullptr || objectMNG == nullptr)
		return E_FAIL;
	if (FAILED(objectMNG->ClearScene(scene)))
		return E_FAIL;
	if (FAILED(componentMNG->ClearScene(scene)))
		return E_FAIL;
	return NOERROR;
}

HRESULT Manager::EngineReady(_uint numScene)
{
	//그래픽 디바이스
	graphicDev = GraphicDevice::GetInstance();
	if (graphicDev == nullptr)
		return E_FAIL;
	AddRefCount(graphicDev);

	//인풋 디바이스
	inputDev = InputDevice::GetInstance();
	if (inputDev == nullptr)
		return E_FAIL;
	AddRefCount(inputDev);

	//오브젝트 매니저
	objectMNG = ObjectManager::GetInstance();
	if (objectMNG == nullptr)
		return E_FAIL;
	AddRefCount(objectMNG);

	if (FAILED(objectMNG->Reserve(numScene)))
		return E_FAIL;

	//컴포넌트 매니저
	componentMNG = ComponentMNG::GetInstance();
	if (componentMNG == nullptr)
		return E_FAIL;
	AddRefCount(componentMNG);

	if (FAILED(componentMNG->Reserve(numScene)))
		return E_FAIL;

	//파이프라인
	pipeLine = PipeLine::GetInstance();
	if (pipeLine == nullptr)
		return E_FAIL;
	AddRefCount(pipeLine);

	//타이머 매니저
	timerMNG = TimerMNG::GetInstance();
	if (timerMNG == nullptr)
		return E_FAIL;
	AddRefCount(timerMNG);

	frustum = Frustum::Create(graphicDev->GetGraphic());
	if (frustum == nullptr)
		return E_FAIL;

	//조명매니저
	lightMNG = LightMNG::GetInstance();
	if (lightMNG == nullptr)
		return E_FAIL;
	AddRefCount(lightMNG);


	//타겟매니저
	targetMNG = TargetMNG::GetInstance();
	if (targetMNG == nullptr)
		return E_FAIL;
	AddRefCount(targetMNG);


	//사운드매니저
	soundMNG = SoundMNG::GetInstance();
	if(soundMNG==nullptr)
		return E_FAIL;
	AddRefCount(soundMNG);
	if (FAILED(soundMNG->Initialize()))
		return E_FAIL;


	itemMNG = ItemManager::GetInstance();
	if (itemMNG == nullptr)
		return E_FAIL;




	AddRefCount(itemMNG);
	return NOERROR;
}

HRESULT Manager::EngineRelease()
{
	_ulong refCount = 0;

	if (refCount = Manager::GetInstance()->DestroyInstance())
		MSG("Failed Release Manager");
	if (refCount = ObjectManager::GetInstance()->DestroyInstance())
		MSG("Failed Release ObjectManager");
	if (refCount = ComponentMNG::GetInstance()->DestroyInstance())
		MSG("Failed Release ComponentMNG");
	if (refCount = PipeLine::GetInstance()->DestroyInstance())
		MSG("Failed Release PipeLine");
	if (refCount = LightMNG::GetInstance()->DestroyInstance())
		MSG("Failed Release LightMNG");
	if (refCount = TargetMNG::GetInstance()->DestroyInstance())
		MSG("Failed Release TargetMNG");
	if (refCount = GraphicDevice::GetInstance()->DestroyInstance())
		MSG("Failed Release GraphicDevice");
	if (refCount = TimerMNG::GetInstance()->DestroyInstance())
		MSG("Failed Release TimerMNG");
	if (refCount = InputDevice::GetInstance()->DestroyInstance())
		MSG("Failed Release InputDevice");
	if (refCount = SoundMNG::GetInstance()->DestroyInstance())
		MSG("Failed Release SoundMNG");
	if (refCount = ItemManager::GetInstance()->DestroyInstance())
		MSG("Failed Release ItemManager");
	return NOERROR;
}

void Manager::Free()
{
	SafeRelease(itemMNG);
	SafeRelease(soundMNG);
	SafeRelease(lightMNG);
	SafeRelease(timerMNG);
	SafeRelease(pipeLine);
	SafeRelease(currentScene);

	SafeRelease(frustum);
	SafeRelease(targetMNG);
	SafeRelease(componentMNG);
	SafeRelease(objectMNG);
	SafeRelease(inputDev);
	SafeRelease(graphicDev);
}