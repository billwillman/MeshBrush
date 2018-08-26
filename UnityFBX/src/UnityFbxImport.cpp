/// ����UNITYʹ�õ�FBX���ʵ�ֿ���ˢ������ɫ�Ĺ���

#include "fbxsdk.h"

#define U3DDLL_EXPORTS

#include "UnityFbxImport.h"
static FbxManager* m_pSdkMgr = NULL;
static FbxIOSettings* m_pSettings = NULL;
static FbxScene* m_pScene = NULL;

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(m_pSdkMgr->GetIOSettings()))
#endif

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

	bool LoadScene(const char* fileName)
	{
		if (!fileName)
			return false;
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor, lSDKMinor, lSDKRevision;
		int lAnimStackCount;
		int i;
		bool lStatus;

		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
		FbxImporter* lImporter = FbxImporter::Create(m_pSdkMgr, "UnityFbxImport");
		const bool lImportStatus = lImporter->Initialize(fileName, -1, m_pSdkMgr->GetIOSettings());
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		if (!lImportStatus)
		{
			lImporter->Destroy();
			return false;
		}

		if (lImporter->IsFBX())
		{
			lAnimStackCount = lImporter->GetAnimStackCount();
			for (i = 0; i < lAnimStackCount; i++)
			{
				FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);
			}

			IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
			IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
			IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
			IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
			IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
			IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
			IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		lStatus = lImporter->Import(m_pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			lImporter->Destroy();
			return false;
		}

		lImporter->Destroy();

		return true;
	}
