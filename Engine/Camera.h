#pragma once
#include "GameObject.h"
#include "PipeLine.h"
#include "Transform.h"
BEGIN(Engine)
class ENGINE_DLL Camera abstract:
	public GameObject
{
public:
	typedef struct tagCameraOption
	{
		_vec3 eye, at, axisY;//���彺���̽� ���� ī�޶� ��ġ / ���彺���̽� ���� ī�޶� �ٶ󺸴� �� / �� ���� ��ǥ�迡���� y�� ����
		_float fovY, aspect, _near, _far;//�þ߰�, ���μ��� ����, ���� ����� ��, ���� �� ��
		_float speed, rotation;//per second
	}CAMERAOPTION;

protected:
	Transform* transformComponent = nullptr;
	PipeLine* pipeLine = nullptr;

	_vec3 right, up, look, position;

	_matrix viewMat;
	_matrix projMat;

	_vec3 originAt, parsingAt;//�÷��̾ ���� �����̰� ī�޶��� ���󰡴�ȿ���� ����
	_bool playerMoving;

	CAMERAOPTION option;

	explicit Camera(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Camera(const Camera& gameObject);
	virtual ~Camera() = default;

public:
	const Transform* GetTransform() { return transformComponent; };
	HRESULT SetVector();
	HRESULT SetView();
	HRESULT RotatePlayerY(const _vec3 * axis, _double timeDelta);
	HRESULT RotatePlayerX(const _vec3 * axis, _double timeDelta);

	HRESULT Forward(_double timeDelta, _float speed);
	HRESULT Back(_double timeDelta, _float speed);
	HRESULT Left(_double timeDelta, _float speed);
	HRESULT Right(_double timeDelta, _float speed);

	virtual HRESULT LateInit();
	virtual HRESULT ReadyProto();
	virtual HRESULT ReadyGameObject(void* argument=nullptr);
	virtual _int Update(_double timeDelta);
	virtual _int LateUpdate(_double timeDelta);
	virtual HRESULT Render();

	virtual GameObject* Clone(void* argument) = 0;
	virtual void Free();
};
END