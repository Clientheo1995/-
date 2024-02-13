#pragma once
namespace Engine
{
typedef unsigned char _ubyte;
typedef signed char _byte;

typedef unsigned short _ushort;
typedef signed short _short;

typedef unsigned int _uint;
typedef signed int _int;

typedef unsigned long _ulong;
typedef signed long _long;

typedef float _float;
typedef double _double;

typedef bool _bool;

typedef wchar_t _tchar;

typedef D3DXVECTOR2 _vec2;
typedef D3DXVECTOR3 _vec3;
typedef class MyVector3:public D3DXVECTOR3
{
	//블라블라
	//연산자 오버로딩도 하고
	//아무튼 이런식으로도 선언한다함
}_Myvec;
typedef D3DXVECTOR4 _vec4;
typedef D3DXMATRIX _matrix;

}
