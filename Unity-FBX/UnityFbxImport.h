/// ����UNITYʹ�õ�FBX���ʵ�ֿ���ˢ������ɫ�Ĺ���

#pragma once

#ifdef U3DDLL_EXPORTS

#define EXPORTS_U3DDLL _declspec(dllexport)

#else

#define EXPORTS_U3DDLL _declspec(dllimport)

#endif