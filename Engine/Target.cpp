#include "..\Headers\Target.h"
#include "Shader.h"
Target::Target(LPDIRECT3DDEVICE9 graphic) :graphic(graphic)
{
	AddRefCount(this->graphic);
}
#ifdef _DEBUG
HRESULT Target::ReadyDebugBuffer(_float startX, _float startY, _float sizeX, _float sizeY)
{
	debugBuffer = ViewportRect::Create(graphic,startX, startY, sizeX, sizeY);
	if (debugBuffer == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT Target::RenderDebugBuffer()
{
	if (debugBuffer == nullptr)
		return E_FAIL;
	graphic->SetTexture(0, targetTexture);
	debugBuffer->Render();
	return NOERROR;
}
#endif
HRESULT Target::Ready(_uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor)
{
	//렌더타겟용 텍스쳐를 생성
	if (FAILED(D3DXCreateTexture(graphic, width, height, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &targetTexture)))
		return E_FAIL;
	//생성한 텍스쳐로부터 렌더타겟을 얻어온다(반환한다)
	if (FAILED(targetTexture->GetSurfaceLevel(0, &targetSurface)))//이 때 레퍼런스 카운트가 되므로 한번 더 해제해줘야한다
		return E_FAIL;
	this->clearColor = clearColor;
	return NOERROR;
}

HRESULT Target::SetupGraphicDev(_uint index)
{
	if (graphic == nullptr || targetSurface == nullptr)
		return E_FAIL;

	//백버퍼를 소실되지않게 미리 저장해둔다
		graphic->GetRenderTarget(index, &oldSurface);

	if(FAILED(graphic->SetRenderTarget(index,targetSurface)))
		return E_FAIL;
	return NOERROR;
}

HRESULT Target::SetShader(Shader * shader, D3DXHANDLE constantName)
{
	if (shader == nullptr || targetTexture == nullptr)
		return E_FAIL;

	shader->SetTexture(constantName, targetTexture);
	return NOERROR;
}

HRESULT Target::ReleaseGraphicDev(_uint index)
{
	if (graphic == nullptr || targetSurface == nullptr)
		return E_FAIL;

	graphic->SetRenderTarget(index,oldSurface);

	SafeRelease(oldSurface);


	return NOERROR;
}

HRESULT Target::ClearTarget()
{
	if (graphic == nullptr)
		return E_FAIL;

	SetupGraphicDev(0);
	graphic->Clear(0, nullptr, D3DCLEAR_TARGET, clearColor, 1.f, 0);
	ReleaseGraphicDev(0);
	return NOERROR;
}

Target * Target::Create(LPDIRECT3DDEVICE9 graphicDevice, _uint width, _uint height, D3DFORMAT format, D3DXCOLOR clearColor)
{
	Target* instance = new Target(graphicDevice);
	if (FAILED(instance->Ready(width, height, format, clearColor)))
	{
		MSG("Failed Create Target");
		SafeRelease(instance);
	}
	return instance;
}

void Target::Free()
{
#ifdef _DEBUG
	SafeRelease(debugBuffer);
#endif
	SafeRelease(targetTexture);
	SafeRelease(oldSurface);
	SafeRelease(targetSurface);//텍스쳐를 한번 더해줘도 되지만 잘못쓴것같으니까 

	SafeRelease(graphic);

}
