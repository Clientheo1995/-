#include "Renderer.h"
#include "GameObject.h"
#include "LightMNG.h"
#include "Shader.h"
#include "PipeLine.h"

_bool Compare(GameObject * sour, GameObject * dest)
{
	return sour->GetViewZ() > dest->GetViewZ();
}
Renderer::Renderer(LPDIRECT3DDEVICE9 graphicDevice) :Component(graphicDevice), targetMNG(TargetMNG::GetInstance()), lightMNG(LightMNG::GetInstance()), pipeLine(PipeLine::GetInstance())
{
	AddRefCount(targetMNG);
	AddRefCount(lightMNG);
	AddRefCount(pipeLine);
}

HRESULT Renderer::ReadyProto()
{
	if (targetMNG == nullptr)
		return E_FAIL;

	D3DVIEWPORT9 viewPort;
	GetGraphic()->GetViewport(&viewPort);

	//디퓨즈 타겟
	if (FAILED(targetMNG->Add(GetGraphic(), L"Target_Diffuse", viewPort.Width, viewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	//노말타겟
	if (FAILED(targetMNG->Add(GetGraphic(), L"Target_Normal", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	//깊이 타겟
	if (FAILED(targetMNG->Add(GetGraphic(), L"Target_Depth", viewPort.Width, viewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	//그림자
	if (FAILED(targetMNG->Add(GetGraphic(), L"Target_Shade", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	//명암 스펙큘러
	if (FAILED(targetMNG->Add(GetGraphic(), L"Target_Specular", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	//디퍼드
	if (FAILED(targetMNG->AddMRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;

	if (FAILED(targetMNG->AddMRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;

	if (FAILED(targetMNG->AddMRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	//라이트 어큐물레이트
	if (FAILED(targetMNG->AddMRT(L"MRT_LightACC", L"Target_Shade")))
		return E_FAIL;

	if (FAILED(targetMNG->AddMRT(L"MRT_LightACC", L"Target_Specular")))
		return E_FAIL;

	lightAcc = Shader::Create(GetGraphic(), L"../Bin/Shader/LightAcc_Shader.fx");
	if (lightAcc == nullptr)
		return E_FAIL;

	blend = Shader::Create(GetGraphic(), L"../Bin/Shader/Blend_Shader.fx");
	if (blend == nullptr)
		return E_FAIL;


	buffer = ViewportRect::Create(GetGraphic(), 0.f - 0.5f, 0.f - 0.5f, viewPort.Width, viewPort.Height);

	if (buffer == nullptr)
		return E_FAIL;

	if (FAILED(targetMNG->ReadyDebugBuffer(L"Target_Diffuse", 0.f, 0.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(targetMNG->ReadyDebugBuffer(L"Target_Normal", 0.f, 100.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(targetMNG->ReadyDebugBuffer(L"Target_Depth", 0.f, 200.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(targetMNG->ReadyDebugBuffer(L"Target_Shade", 100.f, 0.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(targetMNG->ReadyDebugBuffer(L"Target_Specular", 100.f, 100.f, 100.f, 100.f)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Renderer::Ready(void * argument)
{
	return NOERROR;
}

HRESULT Renderer::AddRenderList(RENDERGROUP group, GameObject* gameObject)
{
	if (gameObject == nullptr || LAST <= group)
		return E_FAIL;
	renderList[group].push_back(gameObject);
	AddRefCount(gameObject);
	return NOERROR;
}

HRESULT Renderer::AddLateRenderList(GameObject * gameObject)
{
	if (gameObject == nullptr)
		return E_FAIL;
	//lateRenderList.push_back(gameObject);
	AddRefCount(gameObject);
	return NOERROR;
}

HRESULT Renderer::LateRender()
{
	//for (auto& gameObject : lateRenderList)
	//{
	//	if (gameObject != nullptr)
	//	{
	//		gameObject->Render();
	//		SafeRelease(gameObject);
	//	}
	//}
	//lateRenderList.clear();
	return NOERROR;
}

HRESULT Renderer::Render()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonAlpha()))
		return E_FAIL;
	if (FAILED(Render_Light()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_Alpha()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	if (renderDebug)
	{
	targetMNG->RenderDebugBuffer(L"MRT_Deferred");
	targetMNG->RenderDebugBuffer(L"MRT_LightACC");
	}
	return NOERROR;
}

HRESULT Renderer::Render_Priority()
{
	for (auto& gameObject : renderList[PRERENDER])
	{
		if (gameObject != nullptr)
		{
			gameObject->Render();
			SafeRelease(gameObject);
		}
	}
	renderList[PRERENDER].clear();
	return NOERROR;
}

HRESULT Renderer::Render_NonAlpha()
{
	if (targetMNG == nullptr)
		return E_FAIL;
	//디퓨즈 + 노멀타겟을 장치에 셋팅
	if (FAILED(targetMNG->BeginMRT(L"MRT_Deferred")))
		return E_FAIL;

	//디퓨즈 + 노멀타겟에 객체들의 임의의 정보를 저장해준다
	for (auto& gameObject : renderList[NONALPHA])
	{
		if (gameObject != nullptr)
		{
			gameObject->Render();
				SafeRelease(gameObject);
		}
	}
	renderList[NONALPHA].clear();

	if (FAILED(targetMNG->EndMRT(L"MRT_Deferred")))
		return E_FAIL;
	return NOERROR;
}

HRESULT Renderer::Render_Light()
{
	if (targetMNG == nullptr || lightAcc == nullptr || pipeLine == nullptr)
		return E_FAIL;

	if (FAILED(targetMNG->BeginMRT(L"MRT_LightACC")))
		return E_FAIL;

	if (FAILED(targetMNG->SetShader(lightAcc, "NormalTexture", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(targetMNG->SetShader(lightAcc, "DepthTexture", L"Target_Depth")))
		return E_FAIL;

	_matrix viewMat = pipeLine->GetTransform(D3DTS_VIEW);
	_matrix projMat = pipeLine->GetTransform(D3DTS_PROJECTION);

	D3DXMatrixInverse(&viewMat, nullptr, &viewMat);
	D3DXMatrixInverse(&projMat, nullptr, &projMat);

	lightAcc->SetValue("viewInv", &viewMat, sizeof(_matrix));
	lightAcc->SetValue("projInv", &projMat, sizeof(_matrix));
	lightAcc->SetValue("camPos", &viewMat.m[3][0], sizeof(_vec4));

	lightAcc->ShaderBegin();

	//장치에 셰이드+ 타겟이 셋팅되어있다
	lightMNG->Render(lightAcc);

	lightAcc->ShaderEnd();

	if (FAILED(targetMNG->EndMRT(L"MRT_LightACC")))
		return E_FAIL;
	return NOERROR;
}

HRESULT Renderer::Render_Blend()
{
	if (buffer == nullptr || blend == nullptr)
		return E_FAIL;

	if (FAILED(targetMNG->SetShader(blend, "DiffuseTexture", L"Target_Diffuse")))
		return E_FAIL;

	if (FAILED(targetMNG->SetShader(blend, "ShadeTexture", L"Target_Shade")))
		return E_FAIL;

	if (FAILED(targetMNG->SetShader(blend, "SpecularTexture", L"Target_Specular")))
		return E_FAIL;

	blend->ShaderBegin();
	blend->PassBegin(0);

	buffer->Render();

	blend->PassEnd();
	blend->ShaderEnd();

	return NOERROR;
}

HRESULT Renderer::Render_Alpha()
{
	renderList[ALPHA].sort(Compare);
	for (auto& gameObject : renderList[ALPHA])
	{
		if (gameObject != nullptr)
		{
			gameObject->Render();
			SafeRelease(gameObject);
		}
	}
	renderList[ALPHA].clear();
	return NOERROR;
}

HRESULT Renderer::Render_UI()
{
	renderList[UI].sort(Compare);
	for (auto& gameObject : renderList[UI])
	{
		if (gameObject != nullptr)
		{
			gameObject->Render();
			SafeRelease(gameObject);
		}
	}
	renderList[UI].clear();
	return NOERROR;
}

Renderer * Renderer::Create(LPDIRECT3DDEVICE9 graphicDevice)
{
	Renderer* instance = new Renderer(graphicDevice);
	if (FAILED(instance->ReadyProto()))
	{
		MSG("Failed Create Renderer");
		SafeRelease(instance);
	}
	return instance;
}

Component * Renderer::Clone(void * argument)
{
	//렌더러는 한개만 있어야 하므로 (싱글톤을 쓰지 않는 방법)
	AddRef();
	return this;
}

void Renderer::Free()
{
	for (size_t i = 0; i < LAST; i++)
	{
		for (auto& gameObject : renderList[i])
			SafeRelease(gameObject);
		renderList[i].clear();
	}
	//for (auto& gameObject : lateRenderList)
	//	SafeRelease(gameObject);
	//lateRenderList.clear();

	SafeRelease(pipeLine);
	SafeRelease(lightAcc);
	SafeRelease(blend);
	SafeRelease(buffer);
	SafeRelease(lightMNG);
	SafeRelease(targetMNG);
	Component::Free();
}
