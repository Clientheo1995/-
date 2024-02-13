#include "../Headers/GraphicDevice.h"


IMPLEMENT_SINGLETON(GraphicDevice)
GraphicDevice::GraphicDevice() :SDK(nullptr), Device(nullptr)
{
}

HRESULT GraphicDevice::MakeGraphicReady(HWND hWnd, MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9 * graphicDevice)
{
	HRESULT hr = 0;
	//장치 초기화

	//IDirect3D9 객체 생성

	SDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (SDK == nullptr)
		return E_FAIL;


	//장치 조사
	//HAL 을 통해 장치정보를 얻어옴
	D3DCAPS9 d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	//GetDeviceCaps 그래픽 카드를 조사해서 정보를 D3DCAPS9 구조체에 담아낸다
	if (FAILED(SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		return E_FAIL;

	//현재 그래픽 장치가 버텍스 프로세싱을 지원하는지 조사
	DWORD vp = 0;

	if (d3dcaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)//하드웨어가 정점변환과 빛을 담당하는가
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;//최근 나오는 그래픽 카드들은 전부 하드웨어 지원이 가능하다
	vp |= D3DCREATE_MULTITHREADED;
												  //IDirect3DDevice9 객체 생성
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	//렌더링 환경 설정 - D3DPRESENT_PARAMETERS 의 요소들을 전부 써줘야 한다
	d3dpp.BackBufferWidth = wincx;
	d3dpp.BackBufferHeight=wincy;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount =1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = mode;
	d3dpp.EnableAutoDepthStencil = TRUE;//깊이버퍼와 스텐실 버퍼를 장치가 자동관리할거냐
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//현재 모니터 주사율에 맞춘다  창모드일때는 운영체제에 맞춘다
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//모니터 주사율과 게임 주사율이 다를 때 바로바로 그려버리겠다

	if (FAILED(SDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &Device)))
		return E_FAIL;
	if (graphicDevice != nullptr)
	{
		*graphicDevice = Device;
		AddRefCount(Device);
	}
	return S_OK;
}

void GraphicDevice::Free()
{
	if (Device->Release())
	{
		_CrtDumpMemoryLeaks();
		MSG("Device Release Failed");
	}
	if (SDK->Release())
	{
		_CrtDumpMemoryLeaks();
		MSG("SDK Release Failed");
	}
}