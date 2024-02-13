#include "..\Headers\PipeLine.h"

IMPLEMENT_SINGLETON(PipeLine)

PipeLine::PipeLine()
{
}

_matrix PipeLine::GetTransform(D3DTRANSFORMSTATETYPE transformType)
{
	_matrix temp;
	D3DXMatrixIdentity(&temp);

	switch (transformType)
	{
	case D3DTS_VIEW:
		return viewMat;
		break;
	case D3DTS_PROJECTION:
		return projMat;
		break;
	}
	return temp;
}

HRESULT PipeLine::SetTransform(D3DTRANSFORMSTATETYPE transformType, _matrix transformMat)
{
	switch (transformType)
	{
	case D3DTS_VIEW:
		viewMat = transformMat;
		break;
	case D3DTS_PROJECTION:
		projMat = transformMat;
		break;
	}
	return NOERROR;
}

void PipeLine::Free()
{
}
