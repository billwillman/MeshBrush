/// ����UNITYʹ�õ�FBX���ʵ�ֿ���ˢ������ɫ�Ĺ���

#include "fbxsdk.h"

#define U3DDLL_EXPORTS

#include "UnityFbxImport.h"
static FbxManager* m_pSdkMgr = NULL;
static FbxIOSettings* m_pSettings = NULL;
static FbxScene* m_pScene = NULL;

//
	// ��ʼ��FBX����
	bool InitFbxSdkEnv()
	{
		if (m_pSdkMgr)
			return true;
		m_pSdkMgr = FbxManager::Create();
		if (!m_pSdkMgr)
		{
			return false;
		}

		m_pSettings = FbxIOSettings::Create(m_pSdkMgr, IOSROOT);
		if (!m_pSettings)
			return false;
		m_pSdkMgr->SetIOSettings(m_pSettings);

		FbxString pluginPath = FbxGetApplicationDirectory();
		if (!m_pSdkMgr->LoadPluginsDirectory(pluginPath))
			return false;
		m_pScene = FbxScene::Create(m_pSdkMgr, "UnityFbxScene");
		if (!m_pScene)
			return false;
		return true;
	}

	// ����ע�͵�
	void UnInitFbxSdk()
	{
		if (m_pSdkMgr)
		{
			m_pSdkMgr->Destroy();
			m_pSdkMgr = NULL;
		}
	}
