#pragma once
#include "Base.h"

//게임 내 객체들을 그릴떄(렌더링 파이프라인을 거칠때) 반드시 필요한 행렬변환 정보를 가지고 있다.
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