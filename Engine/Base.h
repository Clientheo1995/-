#pragma once
#include "Engine_Headers.h"

//���Ŭ������ �θ� �ȴ�
//���۷��� ī��Ʈ�� �����Ѵ�(���۷��� ī��Ʈ�� ������Ų�� or ���ҽ�Ų�� Ȥ�� �����Ѵ�)
//���������� ������� �� ���� �߻�Ŭ������ �����

BEGIN(Engine)
class ENGINE_DLL Base abstract
{
private:
	_ulong reference_Count = 0;
protected:
	explicit Base();
	virtual ~Base()=default;

public:
	_ulong AddRef();//���۷��� ����
	_ulong Release();//�����ϰų� ����

	virtual void Free() = 0;//�θ��� ����� ���� ȣ���ϰ� ������
};
END