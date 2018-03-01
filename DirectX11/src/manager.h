
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CRenderer;		// レンダラー
class CCamera;
class CTexManager;
class CXModelManager;
class CInputKeyboard;
class CInputMouse;
class CInputXinput;
class CMainController;

class CManager
{
public:
	CManager() {}
	~CManager() {}

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update(HWND hWnd);
	void Draw();

	static CRenderer *GetRenderer(void) { return m_pRenderer; }		// レンダラーの取得
	static CCamera *GetCamera(void){ return m_pCamera; }			// カメラ

	// マネージャ系
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CTexManager *GetTexManager(void) { return m_pTexManager; }
	static CXModelManager *GetXModelManager(void) { return m_pXModelManager; }
	static CMainController *GetMainController(void) { return m_pMainController; }

	// 入力系
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CInputKeyboard *GetKeyboard(void) { return m_pKeyboard; }
	static CInputMouse *GetMouse(void) { return m_pMouse; }
	static CInputXinput *GetXInput(void) { return m_pXInput; }

private:
	static CRenderer *m_pRenderer;		// レンダラーのポインタ
	static CCamera *m_pCamera;			// カメラ

	// マネージャ系
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CTexManager *m_pTexManager;
	static CXModelManager *m_pXModelManager;
	static CMainController *m_pMainController;

	// 入力系
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CInputKeyboard *m_pKeyboard;
	static CInputMouse *m_pMouse;
	static CInputXinput *m_pXInput;
};

#endif