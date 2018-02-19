
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
class CRenderer;		// レンダラー
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
	static CModelManager *GetModelManager() { return m_pModelManager; }				// モデルマネージャの取得
	static CTexManager *GetTexManager() { return m_pTexManager; }					// テクスチャマネージャの取得
	static CFontManager *GetFontManager() { return m_pFontManager; }
	static CCamera *GetCamera() { return m_pCamera; }								// カメラ
	static CLight *GetLight() { return m_pLight; }									// ライト
	static XA2Manager *GetXA2Manager() { return m_pXA2Manager; }					// XA2soundマネージャ
	static CMainController *GetMainController() { return m_pMainController; }
	static CMotionManager *GetMotionManager() { return m_pMotionManager; }
	static CLightCamera *GetLightCamera() { return m_pLightCamera; }
	static DepthShadow *GetDepthShadow() { return m_pDepthShadow; }					// 深度バッファシャドウ
	static DefferdRendering *GetDefferdRendering() { return m_defferdRendering; }	// デファードレンダリング

	static void LoadMode(CMode *mode);
	static bool GetLoadFlag(){return m_loadingFlag;}

	static void InitMode(CMode *mode); 
	static void EndLoading(CMode *mode); 
	static void DestroyLoadingThread(CMode *mode);

	// world空間上のスケール感を一律に変更
	static const float &GetMasterScale() { return m_masterScale; }
	static const D3DVIEWPORT9 &GetViewPort() { return m_viewPortOld; }

	static void  SetMode(CMode *mode);
	static CMode *GetMode() { return m_pMode; }
	static CMode *GetLoadingMode() { return m_pLoadingMode; }

	// 読み込みスレッドの取得
	static std::thread *GetLoadingThread() { return &m_threadLoading; }

	// ロック
	static inline std::unique_lock<std::recursive_mutex> Locker()
	{
		return  std::unique_lock<std::recursive_mutex>(m_mutex);
	}

private:
	static CRenderer *m_pRenderer;					// レンダラーのポインタ
	static CInputKeyboard* m_pKeyboard;
	static CInputMouse* m_pMouse;
	static CInputJoyStick* m_pJoyStick;
	static CInputXinput *m_pXInput;
	static CModelManager *m_pModelManager;			// モデルマネージャ
	static CTexManager *m_pTexManager;				// テクスチャマネージャ
	static CFontManager *m_pFontManager;
	static CMotionManager *m_pMotionManager;
	static CCamera *m_pCamera;						// カメラ
	static CLight *m_pLight;						// ライト
	static XA2Manager *m_pXA2Manager;				// XA2soundマネージャ
	static CMainController *m_pMainController;
	static CLightCamera *m_pLightCamera;			// ライトカメラ
	static DepthShadow *m_pDepthShadow;				// 深度バッファシャドウ
	static DefferdRendering *m_defferdRendering;	// デファードレンダリング

	static const float m_masterScale;				// world空間上のスケール感を一律に変更

	static CMode *m_pMode;
	static CMode *m_pLoadingMode;

	static bool m_loadingFlag;
	static bool m_loadingNow;

	static D3DVIEWPORT9 m_viewPortOld;

	// 読み込みスレッド
	static std::thread m_threadLoading;
	// std::recursive_mutexクラスは
	// 同一スレッドからの再帰的なロック取得を許可します。
	static std::recursive_mutex m_mutex;

};

#endif