#pragma once
#include "Base.h"

//���� �� ��ü���� �׸���(������ ������������ ��ĥ��) �ݵ�� �ʿ��� ��ĺ�ȯ ������ ������ �ִ�.
BEGIN(Engine)

class PipeLine final:
	public Base
{
	DECLARE_SINGLETON(PipeLine)

private:
	_matrix viewMat, projMat;
	explicit PipeLine();
	virtual ~PipeLine() = default;

public:
	_matrix GetTransform(D3DTRANSFORMSTATETYPE transformType);
	HRESULT SetTransform(D3DTRANSFORMSTATETYPE transformType, _matrix transformMat);

	virtual void Free();
};
END