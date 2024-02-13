#include "..\Headers\Navigation.h"
#include "Cell.h"
#include <cmath>
Navigation::Navigation(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice)
{
}

Navigation::Navigation(const Navigation & gameObject) : Component(gameObject),
celCount(gameObject.celCount), cells(gameObject.cells)
{
	for (auto& cell:cells)
		AddRefCount(cell);
}

HRESULT Navigation::SetNeighbor()
{
	for (size_t i = 0; i < celCount; i++)
	{
		for (size_t j = 0; j < celCount; j++)
		{
			if (cells[i] == cells[j])
				continue;

			if (cells[j]->ComparePoint(cells[i]->GetPoint(Cell::A), cells[i]->GetPoint(Cell::B))==true)
			{
				cells[i]->SetNeighbor(Cell::AB, cells[j]);
			}
			if (cells[j]->ComparePoint(cells[i]->GetPoint(Cell::B), cells[i]->GetPoint(Cell::C)) == true)
			{
				cells[i]->SetNeighbor(Cell::BC, cells[j]);
			}
			if (cells[j]->ComparePoint(cells[i]->GetPoint(Cell::C), cells[i]->GetPoint(Cell::A))==true)
			{
				cells[i]->SetNeighbor(Cell::CA, cells[j]);
			}
		}
	}
	return NOERROR;
}

HRESULT Navigation::SetFirstPos(_vec3 pos)
{
	for (size_t i = 0; i < celCount; i++)
	{
		if (cells[i]->Inside(pos))
			currentIndex = i;
	}
	return NOERROR;
}

HRESULT Navigation::CalculateY(_vec3* position) const
{
	//position 현재 내 포지션
	//인덱스
	_vec3 floor[3] = { cells[currentIndex]->GetPoint(Cell::A), cells[currentIndex]->GetPoint(Cell::B), cells[currentIndex]->GetPoint(Cell::C) };

	//원래 터레인이었다면 간격이 1이라 따로 구할 필요없이 인터벌을 쓰면 됐지만
	//여기는 각각 길이가 다 달라서 따로 구해줘야함
	_float ratioX, ratioZ;
	if (floor[1].x != floor[0].x)
	{
		ratioX = abs((position->x - floor[1].x) / (floor[1].x - floor[0].x));//여기가 nan이 나오고있다
	}
	else
	{
		ratioZ = abs((position->z - floor[1].z) / (floor[1].z - floor[0].z));
	}

	if (floor[1].z != floor[2].z)
	{
	 ratioZ = abs((floor[1].z - position->z) / (floor[1].z-floor[2].z));
	}
	else
	{
		ratioX = abs((floor[1].x - position->x) / (floor[1].x - floor[2].x));
	}

	//if (ratioX > ratioZ)
	//{
	(*position).y=floor[0].y + (floor[1].y - floor[0].y)*ratioX + (floor[2].y - floor[1].y)*ratioZ;
	//}
	//else if(ratioX< ratioZ)	(*position).y = floor[0].y + (floor[1].y - floor[0].y)*ratioZ + (floor[2].y - floor[1].y)*ratioX;


	return NOERROR;
}

_bool Navigation::CheckMove(const _vec3 * result)
{
	Cell* neigh = nullptr;

	//현재 이 내비메쉬를 이용하고잇는 객체가 어떤 셀에 있는지
	//움직인 후 결과가 여전히 같은 셀 안에 있을 때
	if (cells[currentIndex]->CheckNext(result, &neigh)==true)
	{
		return true;
	}
	else//움직인 후 결과가 밖에 있을 때
	{
		if (neigh != nullptr)//나간쪽에 이웃 셀이 있을때
		{
			currentIndex = neigh->GetIndex();
			return true;
		}
		else
			return false;

	}
}

HRESULT Navigation::ReadyProto(const _tchar * filePath)
{
	HANDLE file;
	_ulong byte_ = 0;
	file = CreateFile(filePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file == 0)
		return E_FAIL;

	ReadFile(file, &celCount, sizeof(_uint), &byte_, nullptr);
	cells.reserve(celCount);

	for (size_t i = 0; i < celCount; i++)
	{
		_vec3 point[3];
		ReadFile(file, point, sizeof(_vec3) * 3, &byte_, nullptr);

		if (file == 0)
			break;
		Cell* cell = Cell::Create(GetGraphic(), point[0], point[1], point[2], i);
		if (cell == nullptr)
			break;
		cells.push_back(cell);
	}
	if (FAILED(SetNeighbor()))
		return E_FAIL;
	return NOERROR;
}
HRESULT Navigation::Ready(void * argument)
{
	if (argument != nullptr)
		memcpy(&currentIndex, argument, sizeof(_uint));
	return NOERROR;
}

Navigation * Navigation::Create(LPDIRECT3DDEVICE9 graphicDevice, const _tchar * filePath)
{
	Navigation* instance = new Navigation(graphicDevice);
	if (FAILED(instance->ReadyProto(filePath)))
	{
		MSG("Failed load Navigation");
		SafeRelease(instance);
	}
	return instance;
}

Component * Navigation::Clone(void * argument)
{
	Navigation* instance = new Navigation(*this);
	if (FAILED(instance->Ready(argument)))
	{
		MSG("Failed Clonning Navigation");
		SafeRelease(instance);
	}
	return instance;
}

void Navigation::Free()
{
	for (auto& cell : cells)
		SafeRelease(cell);

	cells.clear();
	Component::Free();
}
//#ifdef _DEBUG
HRESULT Navigation::Render()
{
	for (auto& cell : cells)
	{
		if (cell != nullptr)
			cell->Render();
	}
	return NOERROR;
}
//#endif
