
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "xa2Manager.h"

class CRenderer;
class CCamera;
class CTexManager;
class CXModelManager;
class CInputKeyboard;
class CInputMouse;
class CInputXinput;
class CMainController;
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

	static CRenderer *GetRenderer(void) { return m_pRenderer; }		// �����_���[�̎擾
	static CCamera *GetCamera(void){ return m_pCamera; }			// �J����

	// �}�l�[�W���n
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CTexManager *GetTexManager(void) { return m_pTexManager; }
	static CXModelManager *GetXModelManager(void) { return m_pXModelManager; }
	static CMainController *GetMainController(void) { return m_pMainController; }
	static XA2Manager *GetXA2Manager() { return m_pXA2Manager; }					// XA2sound�}�l�[�W��

	// ���͌n
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CInputKeyboard *GetKeyboard(void) { return m_pKeyboard; }
	static CInputMouse *GetMouse(void) { return m_pMouse; }
	static CInputXinput *GetXInput(void) { return m_pXInput; }

private:
	static CRenderer *m_pRenderer;		// �����_���[�̃|�C���^
	static CCamera *m_pCamera;			// �J����

	// �}�l�[�W���n
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CTexManager *m_pTexManager;
	static CXModelManager *m_pXModelManager;
	static CMainController *m_pMainController;
	static XA2Manager *m_pXA2Manager;				// XA2sound�}�l�[�W��

	// ���͌n
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CInputKeyboard *m_pKeyboard;
	static CInputMouse *m_pMouse;
	static CInputXinput *m_pXInput;
};

#endif