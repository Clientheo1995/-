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
	if (reference_Count == 0)//���۷���ī��Ʈ�� 0�̸� ����� - ����̳� �� ������ �����
	{
		Free();
		delete this;
		return 0;
	}
	else//0�� �ƴϸ� ���ҽ�Ų��
		return reference_Count--;
}