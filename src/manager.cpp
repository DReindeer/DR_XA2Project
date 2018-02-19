//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "camera.h"
#include "light.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "inputJoyStick.h"
#include "inputXInput.h"
#include "modelManager.h"
#include "texManager.h"
#include "fontManager.h"
#include "sceneFont.h"
#include "renderer.h"
#include "debugStr.h"
#include "scene.h"
#include "effect3DPoly.h"
#include "mainController.h"
#include "motionManager.h"
#include "modeLoading.h"
#include "depthShadow.h"
#include "deferredRendering.h"
#include "lightCamera.h"
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"

//================================================================================
// �萔��`
//--------------------------------------------------------------------------------
static const int MAX_EFFECT = 512;		// �G�t�F�N�g�̍ő吔

//================================================================================
// �ÓI�����o�ϐ��̐錾�Ə�����
//--------------------------------------------------------------------------------
CMode *CManager::m_pMode = nullptr;
CMode *CManager::m_pLoadingMode = nullptr;
CRenderer *CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CInputMouse* CManager::m_pMouse = nullptr;
CInputJoyStick* CManager::m_pJoyStick = nullptr;
CInputXinput *CManager::m_pXInput = nullptr;
CModelManager *CManager::m_pModelManager = nullptr;		// ���f���}�l�[�W��
CTexManager *CManager::m_pTexManager = nullptr;			// �e�N�X�`���}�l�[�W��
CFontManager *CManager::m_pFontManager = nullptr;
CMotionManager *CManager::m_pMotionManager = nullptr;
XA2Manager *CManager::m_pXA2Manager = nullptr;			// XA2sound�}�l�[�W��
CCamera *CManager::m_pCamera = nullptr;					// ���C�g
CLight *CManager::m_pLight = nullptr;					// �J����
CLightCamera *CManager::m_pLightCamera = nullptr;		// �J����
CMainController *CManager::m_pMainController = nullptr;
DepthShadow *CManager::m_pDepthShadow = nullptr;		// �[�x�o�b�t�@�V���h�E
DefferdRendering *CManager::m_defferdRendering = nullptr;	// �f�t�@�[�h�����_�����O
const float CManager::m_masterScale = 1.f;				// world��ԏ�̃X�P�[�������ꗥ�ɕύX

D3DVIEWPORT9 CManager::m_viewPortOld;

bool CManager::m_loadingNow = false;
bool CManager::m_loadingFlag = false; 
std::thread CManager::m_threadLoading;
std::recursive_mutex CManager::m_mutex;

//================================================================================
// ������
//--------------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �e�N�X�`���}�l�[�W���̐���
	m_pTexManager = new CTexManager;
	m_pTexManager->Init();

	// �����_���[�̐���
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	// �J����
	m_pCamera = new CCamera();
	m_pCamera->Init();

	Create(hInstance, hWnd);

	return S_OK;
}

void CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ���f���}�l�[�W���̐���
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();

	// �t�H���g�}�l�[�W��
//	m_pFontManager = new CFontManager;
//	m_pFontManager->Init(hInstance);

	// �L�[�{�[�h
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);
	// �W���C�X�e�B�b�N
	m_pJoyStick = new CInputJoyStick();
	m_pJoyStick->Init(hInstance, hWnd);
	// X�C���v�b�g
	m_pXInput = new CInputXinput();
	m_pXInput->Init(hInstance);
	// �}�E�X
	m_pMouse = new CInputMouse();
	m_pMouse->Init(hInstance, hWnd);

	// xaudio2
	m_pXA2Manager = new XA2Manager(hWnd);

	// ���C�g
//	m_pLight = new CLight();
//	m_pLight->Init();

	// ���C�g�J����
	m_pLightCamera = new CLightCamera();
	m_pLightCamera->Init();

	// ���C���R���g���[��
	m_pMainController = new CMainController();
	m_pMainController->Init();

	// 3D�G�t�F�N�g����
	CEffect3DPoly::Create(MAX_EFFECT, TEXNAME_EFFECT_WTOBLINE);

	// �[�x�o�b�t�@�V���h�E�\���p�̃|���S��
	m_pDepthShadow = DepthShadow::Create();

	// �f�B�t�@�[�h�����_�����O
	m_defferdRendering = m_defferdRendering->Create();

	// �r���[�|�[�g�̎擾
	m_pRenderer->GetDevice()->GetViewport(&m_viewPortOld);

	return;
}

// ------- �I��
void CManager::Uninit()
{
	if (m_pDepthShadow)
	{
		m_pDepthShadow->Uninit();
		delete m_pDepthShadow;
		m_pDepthShadow = nullptr;
	}

	if (m_defferdRendering)
	{
		m_defferdRendering->Uninit();
		delete m_defferdRendering;
		m_defferdRendering = nullptr;
	}

	// ���[�h�̏I��
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	// Scene�S�����[�X
	CScene::ReleaseAll();

	// �f�o�b�O���������[�X
#ifdef _DEBUG
//	CDebugStr::ReleaseAll();
#endif

	// ���C���R���g���[��
	if (m_pCamera != nullptr)
	{
		m_pMainController->Uninit();
		delete m_pMainController;
		m_pMainController = nullptr;
	}

	// �J�����̔j��
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLightCamera)
	{
		m_pLightCamera->Uninit();
		delete m_pLightCamera;
		m_pLightCamera = nullptr;
	}

	// �L�[�{�[�h�̔j��
	if (m_pKeyboard)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	// X�C���v�b�g�̔j��
	if (m_pXInput)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = nullptr;
	}

	// �W���C�X�e�B�b�N�̔j��
	if (m_pJoyStick)
	{
		m_pJoyStick->Uninit();
		delete m_pJoyStick;
		m_pJoyStick = nullptr;
	}

	// �}�E�X�̔j��
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// ���f���}�l�[�W���̔j��
	if (m_pModelManager != nullptr)
	{
		m_pModelManager->Uninit();
		delete m_pModelManager;
		m_pModelManager = nullptr;
	}

	// �e�N�X�`���}�l�[�W���̔j��
	if (m_pTexManager != nullptr)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}

	// ���[�V�����}�l�[�W���̔j��
	if (m_pMotionManager != nullptr)
	{
		m_pMotionManager->ReleaseAll();
		delete m_pMotionManager;
		m_pMotionManager = nullptr;
	}

	if (m_pFontManager != nullptr)
	{
		m_pFontManager->Uninit();
		delete m_pFontManager;
		m_pFontManager = nullptr;
	}

	// xa2SoundManager
	if (m_pXA2Manager != nullptr)
	{
		delete m_pXA2Manager;
		m_pXA2Manager = nullptr;
	}

	// �����_���[�̔j��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

// ------- �X�V
void CManager::Update(HWND hWnd)
{
	// �L�[�{�[�h
	if (m_pKeyboard)m_pKeyboard->Update();
	// �W���C�X�e�B�b�N
	if (m_pJoyStick)m_pJoyStick->Update(); 
	// X�C���v�b�g
	if (m_pXInput)m_pXInput->Update();
	// �}�E�X
	if (m_pMouse)m_pMouse->Update(hWnd);

	// �J�����̑O�X�V
	if (m_pCamera != nullptr)m_pCamera->UpdateBigin();

	// �e���[�h�̍X�V
	m_pMode->Update();

	// ���C���R���g���[��
	m_pMainController->Update();

	// scene�S�X�V
	CScene::UpdateAll();

	// �J�����̍X�V
	if (m_pCamera != nullptr)m_pCamera->Update();

	// ���C�g�̍X�V
	m_pLightCamera->Update();

	// �����_�����O�n�t���O�X�V
	m_pDepthShadow->Update();
	m_defferdRendering->Update();

	// �T�E���h�X�V
	m_pXA2Manager->GetSourceVoiceManager()->Update();
}

// ------- �`��
void CManager::Draw()
{
	// Direct3D�ɂ��`��̊J�n
	m_pRenderer->GetDevice()->BeginScene();

	// �f�t�@�[�h�����_�����O�̐ݒ�
	//--------------------------------------------------------------------------------
	if (m_defferdRendering)
	{
		m_defferdRendering->DrawDeviceBegin();

		// �e���[�h�̕`��
		m_pMode->Draw();

		// �[�x�o�b�t�@�V���h�E���s���I�u�W�F�N�g��S�`��
		CScene::DrawDefferdRenderingAll();

		// �f�t�@�[�h�����_�����O�I��
		m_defferdRendering->DrawDeviceEnd();
	}
	//--------------------------------------------------------------------------------

	// �`��̊J�n
	if (m_pRenderer->DrawBigin() == false)return;

	// �J�����̃}�g���N�X�̐ݒ�
	m_pCamera->Set();

	// �e���[�h�̕`��
	m_pMode->Draw();

	// scene�S�`��
	CScene::DrawAll();

	// �`��J�n
	m_pRenderer->DrawEnd();

	// Direct3D�ɂ��`��̏I��
	m_pRenderer->GetDevice()->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pRenderer->GetDevice()->Present(nullptr, nullptr, nullptr, nullptr);
}

void CManager::LoadMode(CMode *mode)
{
	m_loadingNow = true;

	// �O�񃂁[�h�̏I������
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

#ifdef THREAD_ON
	// ���[�h���[�h�̏�����
	m_pMode = new ModeLoading(mode);
	m_pMode->Init();

	// �T�u�X���b�h�ň����̃��[�h�̏���������
	m_threadLoading = std::thread(InitMode, mode);

	// ���[�h���̃��[�h���ꎞ�ۑ�
	m_pLoadingMode = mode;
#else
	SetMode(mode);
#endif
}

// -------- ���[�h������
void CManager::InitMode(CMode *mode)
{
	if (mode != nullptr)
	{
		mode->Init();
	}

	m_loadingFlag = true;
}

// 
void CManager::EndLoading(CMode *mode)
{
	// ���[�h���[�h�̏I������
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}

	// �T�u�X���b�h�̏I����ҋ@
	m_threadLoading.join();

	// ���[�h���̃��[�h��NULL�ɕύX
	m_pLoadingMode = nullptr;

	// ���[�h�؂�ւ�
	m_pMode = mode;

	m_loadingFlag = false;
	m_loadingNow = false;
}

void CManager::DestroyLoadingThread(CMode *mode)
{
	// ���[�h���[�h�̏I������
	if (mode != nullptr)
	{
		mode->Uninit();
		delete mode;
	}

	// ���[�h���̃��[�h��NULL�ɕύX
	m_pLoadingMode = nullptr;

	// �T�u�X���b�h�̏I����ҋ@
	m_threadLoading.join();

	m_loadingFlag = false;
	m_loadingNow = false;
}

// -------- ���[�h�ύX
void CManager::SetMode(CMode *mode)
{
	// ���[�h�̏I������
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}

	m_pMode = mode;
	if (m_pMode != nullptr)
	{
		m_pMode->Init();
	}
}

