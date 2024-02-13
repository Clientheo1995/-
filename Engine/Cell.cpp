#include "..\Headers\Cell.h"
#include "PipeLine.h"

Cell::Cell(LPDIRECT3DDEVICE9 graphicDevice):graphicDev(graphicDevice)
{
	AddRefCount(graphicDev);
}
#ifdef _DEBUG
HRESULT Cell::Render()
{
	if (line == nullptr)
		return E_FAIL;
	PipeLine* pl = PipeLine::GetInstance();

	if(pl==nullptr)
		return E_FAIL;

	AddRefCount(pl);


	_vec3 points[4] = {
		{ point[A]},
		{ point[B] },
		{ point[C] },
		{ point[A] } };

	for (size_t i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&points[i], &points[i], &pl->GetTransform(D3DTS_VIEW));

		//카메라가 0점을 넘어가면 모양이 뒤바뀐다 그것을 잡아줌
		if (points[i].z < 0)
			points[i].z = 0.f;

		D3DXVec3TransformCoord(&points[i], &points[i], &pl->GetTransform(D3DTS_PROJECTION));
	}
	_matrix identity;
	line->SetWidth(2.0f);
	line->Begin();

	line->DrawTransform(points, 4, D3DXMatrixIdentity(&identity), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));//입체공간상에 선을 그리는 함수

	line->End();

	SafeRelease(pl);

	return NOERROR;
}
#endif

HRESULT Cell::Ready(_vec3 pointa, _vec3 pointb, _vec3 pointc, _uint index)
{
	point[A] = pointa;
	point[B] = pointb;
	point[C] = pointc;

	//선의 어느쪽에 있는지 구분하기위해
	direction[LINE_AB] = point[B] - point[A];
	direction[LINE_BC] = point[C] - point[B];
	direction[LINE_CA] = point[A] - point[C];

	//각 벡터의 법선벡터를 만든다
	normal[LINE_AB] = _vec3(direction[LINE_AB].z*-1.f, 0.0f, direction[LINE_AB].x);
	normal[LINE_BC] = _vec3(direction[LINE_BC].z*-1.f, 0.0f, direction[LINE_BC].x);
	normal[LINE_CA] = _vec3(direction[LINE_CA].z*-1.f, 0.0f, direction[LINE_CA].x);


	for (size_t i = 0; i < LINELAST; i++)
	{
		D3DXVec3Normalize(&normal[i], &normal[i]);
	}

	this->index = index;


#ifdef _DEBUG
	if (FAILED(D3DXCreateLine(graphicDev, &line)))
		return E_FAIL;
#endif


	ZeroMemory(neigh, sizeof(Cell*)*NEIGHBORLAST);
	return NOERROR;
}

_bool Cell::ComparePoint(_vec3 sour, _vec3 dest)
{
	if (point[A]==sour)
	{
		if (point[B] == dest)
			return true;
		if (point[C] == dest)
			return true;
	}

	if (point[B] == sour)
	{
		if (point[C] == dest)
			return true;
		if (point[A] == dest)
			return true;
	}

	if (point[C] == sour)
	{
		if (point[A] == dest)
			return true;
		if (point[B] == dest)
			return true;
	}

	return false;
}

_bool Cell::CheckNext(const _vec3 * result, Cell** neigh)
{
	LINE lineID;
	if (Inside(result, &lineID) == false)
	{
		if (this->neigh[lineID] == nullptr)
		{
			return false;
		}
		else
		{
			if (this->neigh[lineID]->CheckNext(result, neigh)==true)
			{
				//왜 이웃체크가 안될까
				*neigh = this->neigh[lineID];
				return false;
			}
			else
				return false;
		}
	}
	else
		return true;
}

_bool Cell::Inside(const _vec3 * result, LINE * lineID)
{
	for (size_t i = 0; i < LINELAST; i++)
	{
		//enum LINE { LINE_AB, LINE_BC, LINE_CA, LINELAST };
		_vec3 temp = *result - point[i];
		if (D3DXVec3Dot(&normal[i], D3DXVec3Normalize(&temp, &temp)) > 0)
		{
			*lineID = LINE(i);
			return false;
		}
	}
	return true;
}
_bool Cell::Inside(_vec3 result)
{
	for (size_t i = 0; i < LINELAST; i++)
	{
		//enum LINE { LINE_AB, LINE_BC, LINE_CA, LINELAST };
		_vec3 temp = result - point[i];
		if (D3DXVec3Dot(&normal[i], D3DXVec3Normalize(&temp, &temp)) > 0)
		{
			return false;
		}
	}
	return true;
}

Cell * Cell::Create(LPDIRECT3DDEVICE9 graphicDevice, _vec3 pointa, _vec3 pointb, _vec3 pointc, _uint index)
{
	Cell* instance = new Cell(graphicDevice);
	if (FAILED(instance->Ready(pointa, pointb, pointc, index)))
	{
		MSG("Failed Create Cell");
		SafeRelease(instance);
	}
	return instance;
}

void Cell::Free()
{
#ifdef _DEBUG
	SafeRelease(line);
#endif
	SafeRelease(graphicDev);
}
