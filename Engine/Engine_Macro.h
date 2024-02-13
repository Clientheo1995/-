#pragma once

#define NO_COPY(ClassName)\
private:\
	ClassName(const ClassName&);\
	ClassName& operator=(const ClassName&);

#define DECLARE_SINGLETON(ClassName)\
		NO_COPY(ClassName)\
public:\
	static ClassName* GetInstance();\
	unsigned long DestroyInstance();\
private:\
	static ClassName* instance;
#define IMPLEMENT_SINGLETON(ClassName)\
ClassName* ClassName::instance = nullptr;\
ClassName* ClassName::GetInstance()\
{\
	if (nullptr == instance)\
		instance = new ClassName;\
	return instance;\
}\
unsigned long ClassName::DestroyInstance()\
{\
	unsigned long referenceCount = 0;\
	if (nullptr != instance)\
	{\
		referenceCount = instance->Release();\
		if (referenceCount == 0)\
			instance = nullptr;\
	}\
	return referenceCount;\
}

#define MSG(MESSAGE) MessageBox(0, TEXT(MESSAGE), L"System Error", MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE{
#define END }
#define USING(NAMESPACE) using namespace NAMESPACE;

#ifdef ENGINE_EXPORTS

#define ENGINE_DLL _declspec(dllexport)//dll로 만들기위함

#else
#define ENGINE_DLL _declspec(dllimport)//dll을 불러오기 위함

#endif