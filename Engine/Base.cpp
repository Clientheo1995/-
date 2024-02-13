#include "../Headers/Base.h"

Base::Base():reference_Count(0)
{
}

_ulong Base::AddRef()
{
	return ++reference_Count;
}

_ulong Base::Release()
{
	if (reference_Count == 0)//레퍼런스카운트가 0이면 지운다 - 몇명이나 날 가져다 썼는지
	{
		Free();
		delete this;
		return 0;
	}
	else//0이 아니면 감소시킨다
		return reference_Count--;
}