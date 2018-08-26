/// 导入UNITY使用的FBX插件实现可以刷顶点颜色的功能

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
	// 初始化FBX环境
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

	// 反向注释掉
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

		return lStatus;
	}

	bool SaveScene(const char* pFilename, int pFileFormat, bool pEmbedMedia)
	{
		int lMajor, lMinor, lRevision;
		bool lStatus = true;

		// Create an exporter.
		FbxExporter* lExporter = FbxExporter::Create(m_pSdkMgr, "UnityFbxExport");

		if (pFileFormat < 0 || pFileFormat >= m_pSdkMgr->GetIOPluginRegistry()->GetWriterFormatCount())
		{
			// Write in fall back format in less no ASCII format found
			pFileFormat = m_pSdkMgr->GetIOPluginRegistry()->GetNativeWriterFormat();

			//Try to export in ASCII if possible
			int lFormatIndex, lFormatCount = m_pSdkMgr->GetIOPluginRegistry()->GetWriterFormatCount();

			for (lFormatIndex = 0; lFormatIndex<lFormatCount; lFormatIndex++)
			{
				if (m_pSdkMgr->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
				{
					FbxString lDesc = m_pSdkMgr->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
					const char *lASCII = "ascii";
					if (lDesc.Find(lASCII) >= 0)
					{
						pFileFormat = lFormatIndex;
						break;
					}
				}
			}
		}

		// Set the export states. By default, the export states are always set to 
		// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
		// shows how to change these states.
		IOS_REF.SetBoolProp(EXP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(EXP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED, pEmbedMedia);
		IOS_REF.SetBoolProp(EXP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(EXP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(EXP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

		// Initialize the exporter by providing a filename.
		if (lExporter->Initialize(pFilename, pFileFormat, m_pSdkMgr->GetIOSettings()) == false)
		{
			lExporter->Destroy();
			return false;
		}

		FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);

		// Export the scene.
		lStatus = lExporter->Export(m_pScene);

		// Destroy the exporter.
		lExporter->Destroy();
		return lStatus;
	}
