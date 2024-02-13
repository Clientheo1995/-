#pragma once
class TagFinder
{
private:
	const wchar_t* string = nullptr;
public:
	TagFinder(const wchar_t* string) :string(string)
	{
	}
	template<typename T>
	bool operator() (T& pair)
	{
		return !lstrcmp(string, pair.first);
	}
};
