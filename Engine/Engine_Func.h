#pragma once
namespace Engine
{
	template <typename T>
	void SafeDelete(T& pointer)//그냥 포인터를 삭제하고싶을때
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeDelete_Arr(T& pointer)//그냥 포인터를 삭제하고싶을때
	{
		if (nullptr != pointer)
		{
			delete [] pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	unsigned long SafeRelease(T& instance)//레퍼런스 카운트로 관리되고있는 객체를 삭제하고자 할때
	{
		unsigned long referenceCount = 0;

		if (nullptr != instance)
		{
			referenceCount=instance->Release();
			if(referenceCount==0)
				instance = nullptr;
		}
		return referenceCount;
	}

	template <typename T>
	unsigned long AddRefCount(T& instance)
	{
		unsigned long referenceCount = 0;

		if (nullptr != instance)
		{
			referenceCount = instance->AddRef();
		}
		return referenceCount;
	}
}