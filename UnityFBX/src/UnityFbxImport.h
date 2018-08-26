/// 导入UNITY使用的FBX插件实现可以刷顶点颜色的功能

#pragma once

 
#undef EXPORTS_U3DDLL
#ifdef U3DDLL_EXPORTS
#define EXPORTS_U3DDLL _declspec(dllexport)

#else

#define EXPORTS_U3DDLL _declspec(dllimport)

#endif


extern"C"
{
	EXPORTS_U3DDLL bool InitFbxSdkEnv();
	EXPORTS_U3DDLL void UnInitFbxSdk();
}
