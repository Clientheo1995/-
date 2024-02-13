#include "..\Headers\TargetMNG.h"
#include "Shader.h"
IMPLEMENT_SINGLETON(TargetMNG)
TargetMNG::TargetMNG()
{
}

Target * TargetMNG::Find(const _tchar * targetTag)
{
	auto iter = find_if(targets.begin(), targets.end(), TagFinder(targetTag));
	if (iter == targets.end())
		return nullptr;

	return iter->second;
}

list<Target*>* TargetMNG::FindMRT(const _tchar * MRTTag)
{
	auto iter = find_if(mrt.begin(), mrt.end(), TagFinder(MRTTag));
	if (iter == mrt.end())
		return nullptr;

	return &iter->second;
}


HRESULT TargetMNG::SetShader(Shader * shader, D3DXHANDLE constantName, const _tchar * targetTag)
{
	Target* target = Find(targetTag);
	if (nullptr == target)
		return E_FAIL;

	return target->SetShader(shader, constantName);
}

HRESULT TargetMNG::Add(LPDIRECT3DDEVICE9 graphic, const _tchar * targetTag, _uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor)
{
	if (Find(targetTag) != nullptr)
		return E_FAIL;
	Target* target = Target::Create(graphic, width, height, format, clearColor);
	if (nullptr == target)
		return E_FAIL;

	targets.insert(TARGETS::value_type(targetTag, target));
	return NOERROR;
}

HRESULT TargetMNG::AddMRT(const _tchar * MRTTag, const _tchar * targetTag)
{
	Target* target = Find(targetTag);
	if (nullptr == target)
		return E_FAIL;
	list<Target*>* mrtList = FindMRT(MRTTag);

	if (mrtList == nullptr)
	{
		list<Target*> mrtlist;
		mrtlist.push_back(target);

		mrt.insert(MRT::value_type(MRTTag, mrtlist));
	}
	else
		mrtList->push_back(target);

	AddRefCount(target);
	return NOERROR;
}

HRESULT TargetMNG::BeginMRT(const _tchar * MRTTag)
{
	list<Target*>* mrtList = FindMRT(MRTTag);

	if (mrtList == nullptr)
		return E_FAIL;

	//초기화 전에 백버퍼를 지정한 색으로 싹 채운다
	for (auto& target : *mrtList)
	{
		if (FAILED(target->ClearTarget()))
			return E_FAIL;
	}
	_uint index = 0;

	for (auto& target : *mrtList)
	{
		if(FAILED(target->SetupGraphicDev(index++)))
			return E_FAIL;
	}
	return NOERROR;
}

HRESULT TargetMNG::EndMRT(const _tchar * MRTTag)
{
	list<Target*>* mrtList = FindMRT(MRTTag);

	if (mrtList == nullptr)
		return E_FAIL;

	_uint index = 0;

	for (auto& target : *mrtList)
	{
		target->ReleaseGraphicDev(index++);
	}
	return NOERROR;
}

HRESULT TargetMNG::ReadyDebugBuffer(const _tchar * targetTag, _float startX, _float startY, _float sizeX, _float sizeY)
{
	Target* target = Find(targetTag);
	if (target == nullptr)
		return E_FAIL;
	return target->ReadyDebugBuffer(startX, startY, sizeX, sizeY);
}

HRESULT TargetMNG::RenderDebugBuffer(const _tchar * MRTTag)
{
	list<Target*>* mrtList = FindMRT(MRTTag);

	if (mrtList == nullptr)
		return E_FAIL;

	for (auto& target : *mrtList)
	{
		target->RenderDebugBuffer();
	}
	return NOERROR;
}
void TargetMNG::Free()
{
	for (auto& pair : mrt)
	{
		for (auto& target : pair.second)
			SafeRelease(target);
		pair.second.clear();
	}

	mrt.clear();
	for (auto& pair : targets)
	{
		SafeRelease(pair.second);
	}

	targets.clear();
}
