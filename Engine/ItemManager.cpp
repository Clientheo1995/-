#include "..\Headers\ItemManager.h"
#include "Item.h"
IMPLEMENT_SINGLETON(ItemManager)

ItemManager::ItemManager()
{
}

GameObject * ItemManager::FindItem(const _tchar * itemTag)
{
	auto iter = find_if(items.begin(), items.end(), TagFinder(itemTag));//임시객체의 오버라이딩한 함수객체가 들어가서 생성자를 호출. 임시객체의 생성자라서 새로운 객체가 생성된다
	if (iter == items.end())
		return nullptr;

	return iter->second;
}

vector<GameObject*> ItemManager::GetItems()
{
	vector<GameObject*> item_vector;
	for (auto& pair : items)
	{
		item_vector.push_back(pair.second);
	}
	return item_vector;
}

HRESULT ItemManager::AddItem(const _tchar * itemTag, GameObject * item)
{
	if (item == nullptr)
		return E_FAIL;
	items.insert(ITEM::value_type(itemTag, item));
	return NOERROR;
}

HRESULT ItemManager::DeleteItem(const _tchar * itemTag)
{
	if (FindItem(itemTag) != nullptr)
		return E_FAIL;
	items.erase(itemTag);
	return NOERROR;
}

HRESULT ItemManager::SortItem()
{
	map<const _tchar*, GameObject*>::iterator i = items.begin();
	map<const _tchar*, GameObject*>::iterator j = items.begin();
	for (; i == items.end(); i++)
		for (; j == items.end(); j++)
		{
			//if (_tcscmp(i->first, j->first))
				//i->swap(j);
		//https://skmagic.tistory.com/226
		}
	return NOERROR;
}

void ItemManager::Free()
{
	for (auto& pair : items)
	{
		SafeRelease(pair.second);
	}
	items.clear();
}
