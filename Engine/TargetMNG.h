#pragma once
//1. ����Ÿ�ٵ��� �����Ͽ� �����Ѵ�
//2. ������ Ÿ�ٵ��� ��Ƽ� ������´�(��Ƽ����Ÿ��)
//3. ��Ƽ����Ÿ�ٿ� ��ϵǾ��ִ� Ÿ�ٵ��� ��ġ�� ���Ѵ�
//4. ��Ƽ����Ÿ�ٿ� ��ϵǾ��ִ� Ÿ�ٵ��� ȸ���Ѵ�
#include "Target.h"
BEGIN(Engine)

class TargetMNG final :
	public Base
{
	DECLARE_SINGLETON(TargetMNG)
private:
	map<const _tchar*, Target*> targets;
	typedef map<const _tchar*, Target*> TARGETS;

	map<const _tchar*, list<Target*>> mrt;
	typedef map<const _tchar*, list<Target*>> MRT;

	Target* Find(const _tchar* targetTag);
	list<Target*>* FindMRT(const _tchar* MRTTag);

	explicit TargetMNG();
	virtual ~TargetMNG() = default;

public:
	HRESULT SetShader(Shader* shader, D3DXHANDLE constantName, const _tchar* targetTag);
	HRESULT Add(LPDIRECT3DDEVICE9 graphic, const _tchar* targetTag,_uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor);

	HRESULT AddMRT(const _tchar* MRTTag, const _tchar* targetTag);
	HRESULT BeginMRT(const _tchar* MRTTag);
	HRESULT EndMRT(const _tchar* MRTTag);

	HRESULT ReadyDebugBuffer(const _tchar* targetTag, _float startX, _float startY, _float sizeX, _float sizeY);
	HRESULT RenderDebugBuffer(const _tchar* MRTTag);

	virtual void Free();
};
END