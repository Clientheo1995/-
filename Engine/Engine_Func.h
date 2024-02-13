#pragma once
namespace Engine
{
	template <typename T>
	void SafeDelete(T& pointer)//�׳� �����͸� �����ϰ������
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeDelete_Arr(T& pointer)//�׳� �����͸� �����ϰ������
	{
		if (nullptr != pointer)
		{
			delete [] pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	unsigned long SafeRelease(T& instance)//���۷��� ī��Ʈ�� �����ǰ��ִ� ��ü�� �����ϰ��� �Ҷ�
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