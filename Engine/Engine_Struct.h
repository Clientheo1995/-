#pragma once
namespace Engine
{
	typedef struct tagMeshTexture
	{
		enum TYPE { DIFFUSE, NORMAL, SPECULAR, EMISSIVE, LAST };
		LPDIRECT3DTEXTURE9 texture[LAST];
	}MESHTEXTURE;

	typedef struct _LIGHT :D3DLIGHT9
	{
		float height;
	}_LIGHT;

	typedef struct tagUISetUp
	{
		_float x;//������� �׷��� ��ǥ
		_float y;//������� �׷��� ��ǥ
		_float sizeX;//����ũ��
		_float sizeY;//����ũ��
		_float wincx;//������ ���� ũ��
		_float wincy;//������ ���� ũ��
		_uint textureNum;//���� �ؽ��� �� ��ȣ
		_bool is2D;
	}UISETUP;

	typedef struct tagItem
	{
		_tchar name[MAXCHAR];
		_tchar textureTag[MAXCHAR];
		_tchar meshTag[MAXCHAR];
	}ITEM;

	typedef struct tagComponentTag
	{
		_tchar* prototypeTag;
		_tchar* componentTag;
		_float frames;
	}COMPONENTTAG;

	typedef struct tagStatus
	{
		_int hp, stamina;
	}STATUS;
	typedef struct tagD3DXMeshContainer_Derived :public D3DXMESHCONTAINER
	{
		MESHTEXTURE* meshTexture;
		LPD3DXMESH originalMesh;
		_ulong numFrames;
		D3DXMATRIX* offsetMatrices;
		D3DXMATRIX** combinedTransformMatrices;
		D3DXMATRIX* renderingMatrices;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagD3DXFrame_Derived :public D3DXFRAME
	{
		D3DXMATRIX combinedTransform;
	}D3DXFRAME_DERIVED;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 textureUV;
	}VERTEXTEXTURE;

	typedef struct tag_Viewport_VertexTexture
	{
		D3DXVECTOR4 position;
		D3DXVECTOR2 textureUV;
	}VIEWPORT_VERTEX_TEXTURE;

	typedef struct tagCubeTexture
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 textureUV;
	}CUBETEXTURE;

	typedef struct tagNormalTexture
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 textureUV;
	}NORMALTEXTURE;

	typedef struct tagIndex16
	{
		unsigned short _1, _2, _3;
	}INDEX16;//16��Ʈ

	typedef struct tagIndex32
	{
		unsigned long _1, _2, _3;
	}INDEX32;
}