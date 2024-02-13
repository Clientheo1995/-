#include "..\Headers\Component.h"

Component::Component(LPDIRECT3DDEVICE9 graphicDevice) :graphicDevice(graphicDevice), isClone(false)
{
	AddRefCount(this->graphicDevice);
}

Component::Component(const Component & gameObject) : graphicDevice(gameObject.graphicDevice), isClone(true)
{
	AddRefCount(this->graphicDevice);
}

HRESULT Component::ReadyProto()
{
	return NOERROR;
}

HRESULT Component::Ready(void * argument)
{
	return NOERROR;
}

void Component::Free()
{
	SafeRelease(graphicDevice);
}
