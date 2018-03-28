// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �}�l�[�W��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "inputXInput.h"
#include "renderer.h"
#include "texManager.h"
#include "modelManager.h"
#include "mainController.h"

#include "scene.h"
#include "scene3DPoly.h"
#include "scene2D.h"
#include "sceneBox.h"
#include "sceneXModel.h"
#include "sceneXModelListener.h"

// XAudio2
#include "xa2Manager.h"
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2SoundResourceManager.h"


// ======== ======== ======== ======== ======== ======== ======== ========
// �萔��`
// -------- -------- -------- -------- -------- -------- -------- --------
static const int MAX_EFFECT = 512;		// �G�t�F�N�g�̍ő吔

static const char* TEX_TEST = "data/TEXTURE/NeoYurumu/NEOYurumu.png";
static const char* TEX_FIELD = "data/TEXTURE/FIELD/kusa.jpg";

// ======== ======== ======== ======== ======== ======== ======== ========
// �ÓI�����o�ϐ��̐錾�Ə�����
// -------- -------- -------- -------- -------- -------- -------- --------
CRenderer *CManager::m_pRenderer = nullptr;
CCamera *CManager::m_pCamera = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CInputMouse* CManager::m_pMouse = nullptr;
CInputXinput *CManager::m_pXInput = nullptr;
CTexManager *CManager::m_pTexManager = nullptr;
CXModelManager *CManager::m_pXModelManager = nullptr;
CMainController *CManager::m_pMainController = nullptr;
XA2Manager *CManager::m_pXA2Manager = nullptr;			// XA2sound�}�l�[�W��

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	if (!XMVerifyCPUSupport()) {
		MessageBox(nullptr, "DirectXMath��Ή�CPU", "DirectXMath", MB_OK);
		return E_FAIL;
	}

	// �����_���[�̐���
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	// �J����
	m_pCamera = new CCamera();
	m_pCamera->Init();

	// �e�N�X�`���}�l�[�W��
	m_pTexManager = new CTexManager();
	m_pTexManager->Init();

	// ���f���}�l�[�W��
	m_pXModelManager = new CXModelManager();
	m_pXModelManager->Init();

	// ���C���R���g���[��
	m_pMainController = new CMainController();
	m_pMainController->Init();

	// �L�[�{�[�h
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);
	// X�C���v�b�g
	m_pXInput = new CInputXinput();
	m_pXInput->Init(hInstance);
	// �}�E�X
	m_pMouse = new CInputMouse();
	m_pMouse->Init(hInstance, hWnd);

	// �����_�����O�e�X�g
	CScene3DPoly *pField = CScene3DPoly::Create(XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3(0.f, PI * -0.5f, 0.f), XMFLOAT3(10.f, 10.f, 10.f), TEX_FIELD, 4);
	CSceneXModelListener *pXModel = CSceneXModelListener::Create(XMFLOAT3(0.f, 0.5f, 0.f), 
		XMFLOAT3(0.f, PI * 0.5f, 0.f), XMFLOAT3(0.1f, 0.1f, 0.1f), "data/MODEL/car/body.x", 4);

	CScene2D *p2D = CScene2D::Create(XMFLOAT2(50.f, 50.f), 0.f, XMFLOAT2(50.f, 50.f), TEX_TEST, 5);

	// xaudio2
	m_pXA2Manager = new XA2Manager(hWnd);

	// �\�[�X�{�C�X�쐬
	XA2SourceVoice2D *pXA2SourceVoice = new XA2SourceVoice2D;
	pXA2SourceVoice->Create("data/BGM/BGM_Yurumu.ogg", -1, XA2Manager::GetLoadOggStreaming());

	// �T�E���h�I�u�W�F�N�g�ǉ�
	XA2SourceVoiceManager *pSoundObjectManager = XA2Manager::GetSourceVoiceManager();
	pSoundObjectManager->SetXA2SoundObject(pXA2SourceVoice, this);

	pXA2SourceVoice->Play("data/BGM/BGM_Yurumu.ogg");

	return S_OK;
}

// ------- �I��
void CManager::Uninit(void)
{
	// Scene�S�����[�X
	CScene::ReleaseAll();

	// �o�^�ς݃\�[�X�{�C�X�S���
	XA2SourceVoiceManager *pSoundObjectManager = XA2Manager::GetSourceVoiceManager();
	pSoundObjectManager->StopAndUninit();

	// �T�E���h���\�[�X�S���
	XA2Manager::GetSoundResourceManager()->Uninit();

	// xa2SoundManager
	if (m_pXA2Manager != nullptr)
	{
		delete m_pXA2Manager;
		m_pXA2Manager = nullptr;
	}

	// �L�[�{�[�h
	if (m_pKeyboard)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	// ���C���R���g���[��
	if (m_pMainController)
	{
		m_pMainController->Uninit();
		delete m_pMainController;
		m_pMainController = nullptr;
	}

	// �e�N�X�`���}�l�[�W��
	if (m_pTexManager)
	{
		m_pTexManager->Uninit();
		delete m_pTexManager;
		m_pTexManager = nullptr;
	}

	// ���f���}�l�[�W��
	if (m_pXModelManager)
	{
		m_pXModelManager->Uninit();
		delete m_pXModelManager;
		m_pXModelManager = nullptr;
	}

	// X�C���v�b�g
	if (m_pXInput)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = nullptr;
	}

	// �}�E�X
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// �J����
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// �����_���[
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
	// X�C���v�b�g
	if (m_pXInput)m_pXInput->Update();
	// �}�E�X
	if (m_pMouse)m_pMouse->Update(hWnd);

	// ���C���R���g���[��
	if (m_pMainController)m_pMainController->Update();

	// �J�����̍X�V
	if (m_pCamera)m_pCamera->Update();

	// scene�S�X�V
	CScene::UpdateAll();

	// �T�E���h�X�V
	m_pXA2Manager->GetSourceVoiceManager()->Update();
}

// ------- �`��
void CManager::Draw(void)
{
	// �`��̊J�n
	if (m_pRenderer->DrawBigin() == false)return;

	// �J�����̃}�g���N�X�̐ݒ�
	if(m_pCamera)m_pCamera->Set();

	// scene�S�`��
	CScene::DrawAll();

	// ���C���R���g���[��
	if (m_pMainController)m_pMainController->Draw();

	// �`��̏I��
	m_pRenderer->DrawEnd();
}
