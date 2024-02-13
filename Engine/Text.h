#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL Text final:public Component
{
	//�ؽ�ó�� ȣ���Ҷ����� �����ּҸ� ��ȯ�Ѵ� - �̸� �����ξ��� ������ ���� ȣ���Ҷ����� �˻����� �ʾƵ� �ȴ�
public:
	enum TextTYPE {GENERAL, CUBE, _END};
private:
	_tchar text[MAXCHAR];
	RECT textBox;

	const _matrix* worldMat;
	LPD3DXSPRITE sprite;
	LPD3DXFONT font;

	explicit Text(LPDIRECT3DDEVICE9 graphicDevice);
	explicit Text(const Text& gameObject);
	virtual ~Text() = default;
	HRESULT SetupConstantTable();
public:
	virtual HRESULT SetWorld(const _matrix* mat) { worldMat = mat;  return NOERROR; }
	void SetRect(RECT rc) { textBox = rc; }
	virtual HRESULT ReadyProtoComponent(LPDIRECT3DDEVICE9 graphicDevice, D3DXFONT_DESC fontType);
	virtual HRESULT Ready(void* argument);
	virtual HRESULT Render();
	static Text* Create(LPDIRECT3DDEVICE9 graphicDevice, D3DXFONT_DESC fontType);
	virtual Component* Clone(void* argument);
	virtual void Free();
	LPD3DXFONT GetText(_uint index);
};

END