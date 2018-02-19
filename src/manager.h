
#ifndef _MANAGER_H_
#define _MANAGER_H_

// STL
#include <thread>
#include <mutex>

#include "main.h"
#include "xa2Manager.h"

#if	0
#define THREAD_ON
#endif

class CMode;
class CCamera;
class CLight;
class CRenderer;		// �����_���[
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CInputXinput;
class CTexManager;
class CModelManager;
class CFontManager;
class CMainController;
class CMotionManager;
class CScore;
class CLightCamera;
class DepthShadow;
class DefferdRendering;
class XA2Manager;

class CManager
{
public:
	CManager() {}
	~CManager() {}

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update(HWND hWnd);
	void Draw();
	void static Create(HINSTANCE hInstance, HWND hWnd);

	static CRenderer *GetRenderer() { return m_pRenderer; }					
	static CInputKeyboard *GetKeyboard() { return m_pKeyboard; }
	static CInputMouse *GetMouse() { return m_pMouse; }
	static CInputJoyStick *GetJoyStick() { return m_pJoyStick; }
	static CInputXinput *GetXInput() { return m_pXInput; }	
	static CModelManager *GetModelManager() { return m_pModelManager; }				// ���f���}�l�[�W���̎擾
	static CTexManager *GetTexManager() { return m_pTexManager; }					// �e�N�X�`���}�l�[�W���̎擾
	static CFontManager *GetFontManager() { return m_pFontManager; }
	static CCamera *GetCamera() { return m_pCamera; }								// �J����
	static CLight *GetLight() { return m_pLight; }									// ���C�g
	static XA2Manager *GetXA2Manager() { return m_pXA2Manager; }					// XA2sound�}�l�[�W��
	static CMainController *GetMainController() { return m_pMainController; }
	static CMotionManager *GetMotionManager() { return m_pMotionManager; }
	static CLightCamera *GetLightCamera() { return m_pLightCamera; }
	static DepthShadow *GetDepthShadow() { return m_pDepthShadow; }					// �[�x�o�b�t�@�V���h�E
	static DefferdRendering *GetDefferdRendering() { return m_defferdRendering; }	// �f�t�@�[�h�����_�����O

	static void LoadMode(CMode *mode);
	static bool GetLoadFlag(){return m_loadingFlag;}

	static void InitMode(CMode *mode); 
	static void EndLoading(CMode *mode); 
	static void DestroyLoadingThread(CMode *mode);

	// world��ԏ�̃X�P�[�������ꗥ�ɕύX
	static const float &GetMasterScale() { return m_masterScale; }
	static const D3DVIEWPORT9 &GetViewPort() { return m_viewPortOld; }

	static void  SetMode(CMode *mode);
	static CMode *GetMode() { return m_pMode; }
	static CMode *GetLoadingMode() { return m_pLoadingMode; }

	// �ǂݍ��݃X���b�h�̎擾
	static std::thread *GetLoadingThread() { return &m_threadLoading; }

	// ���b�N
	static inline std::unique_lock<std::recursive_mutex> Locker()
	{
		return  std::unique_lock<std::recursive_mutex>(m_mutex);
	}

private:
	static CRenderer *m_pRenderer;					// �����_���[�̃|�C���^
	static CInputKeyboard* m_pKeyboard;
	static CInputMouse* m_pMouse;
	static CInputJoyStick* m_pJoyStick;
	static CInputXinput *m_pXInput;
	static CModelManager *m_pModelManager;			// ���f���}�l�[�W��
	static CTexManager *m_pTexManager;				// �e�N�X�`���}�l�[�W��
	static CFontManager *m_pFontManager;
	static CMotionManager *m_pMotionManager;
	static CCamera *m_pCamera;						// �J����
	static CLight *m_pLight;						// ���C�g
	static XA2Manager *m_pXA2Manager;				// XA2sound�}�l�[�W��
	static CMainController *m_pMainController;
	static CLightCamera *m_pLightCamera;			// ���C�g�J����
	static DepthShadow *m_pDepthShadow;				// �[�x�o�b�t�@�V���h�E
	static DefferdRendering *m_defferdRendering;	// �f�t�@�[�h�����_�����O

	static const float m_masterScale;				// world��ԏ�̃X�P�[�������ꗥ�ɕύX

	static CMode *m_pMode;
	static CMode *m_pLoadingMode;

	static bool m_loadingFlag;
	static bool m_loadingNow;

	static D3DVIEWPORT9 m_viewPortOld;

	// �ǂݍ��݃X���b�h
	static std::thread m_threadLoading;
	// std::recursive_mutex�N���X��
	// ����X���b�h����̍ċA�I�ȃ��b�N�擾�������܂��B
	static std::recursive_mutex m_mutex;

};

#endif