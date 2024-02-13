#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Item.h"
BEGIN(Engine)
class Item;
class ItemManager :
	public Base
{
	DECLARE_SINGLETON(ItemManager)
private:
	map<const _tchar*, GameObject*> items;
	typedef map<const _tchar*, GameObject*> ITEM;

	ItemManager();
	virtual ~ItemManager() =default;

	GameObject* FindItem(const _tchar* itemTag);
public:
	_uint GetInvenSize() { return items.size(); }
	vector<GameObject*> GetItems();
	HRESULT AddItem(const _tchar* itemTag, GameObject* item);
	HRESULT DeleteItem(const _tchar* itemTag);
	HRESULT SortItem();
	virtual void Free();
};

END