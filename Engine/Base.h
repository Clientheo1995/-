#pragma once
#include "Engine_Headers.h"

//모든클래스의 부모가 된다
//레퍼런스 카운트를 관리한다(레퍼런스 카운트를 증가시킨다 or 감소시킨다 혹은 삭제한다)
//독립적으로 만들어질 수 없게 추상클래스로 만든다

BEGIN(Engine)
class ENGINE_DLL Base abstract
{
private:
	_ulong reference_Count = 0;
protected:
	explicit Base();
	virtual ~Base()=default;

public:
	_ulong AddRef();//레퍼런스 증가
	_ulong Release();//감소하거나 삭제

	virtual void Free() = 0;//부모의 릴리즈를 먼저 호출하고 싶을때
};
END