/// 导入UNITY使用的FBX插件实现可以刷顶点颜色的功能

#pragma once

#ifdef U3DDLL_EXPORTS

#define EXPORTS_U3DDLL _declspec(dllexport)

#else

#define EXPORTS_U3DDLL _declspec(dllimport)

#endif