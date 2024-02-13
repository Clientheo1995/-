#include "../Headers/GraphicDevice.h"


IMPLEMENT_SINGLETON(GraphicDevice)
GraphicDevice::GraphicDevice() :SDK(nullptr), Device(nullptr)
{
}

HRESULT GraphicDevice::MakeGraphicReady(HWND hWnd, MODE mode, _uint wincx, _uint wincy, LPDIRECT3DDEVICE9 * graphicDevice)
{
	HRESULT hr = 0;
	//��ġ �ʱ�ȭ

	//IDirect3D9 ��ü ����

	SDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (SDK == nullptr)
		return E_FAIL;


	//��ġ ����
	//HAL �� ���� ��ġ������ ����
	D3DCAPS9 d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	//GetDeviceCaps �׷��� ī�带 �����ؼ� ������ D3DCAPS9 ����ü�� ��Ƴ���
	if (FAILED(SDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		return E_FAIL;

	//���� �׷��� ��ġ�� ���ؽ� ���μ����� �����ϴ��� ����
	DWORD vp = 0;

	if (d3dcaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)//�ϵ��� ������ȯ�� ���� ����ϴ°�
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;//�ֱ� ������ �׷��� ī����� ���� �ϵ���� ������ �����ϴ�
	vp |= D3DCREATE_MULTITHREADED;
												  //IDirect3DDevice9 ��ü ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	//������ ȯ�� ���� - D3DPRESENT_PARAMETERS �� ��ҵ��� ���� ����� �Ѵ�
	d3dpp.BackBufferWidth = wincx;
	d3dpp.BackBufferHeight=wincy;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount =1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = mode;
	d3dpp.EnableAutoDepthStencil = TRUE;//���̹��ۿ� ���ٽ� ���۸� ��ġ�� �ڵ������Ұų�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���� ����� �ֻ����� �����  â����϶��� �ü���� �����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//����� �ֻ����� ���� �ֻ����� �ٸ� �� �ٷιٷ� �׷������ڴ�

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