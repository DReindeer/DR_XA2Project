//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "mainController.h"
#include "inputKeyboard.h"
#include "inputXInput.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugStr.h"
#include "modelManager.h"
#include "texManager.h"
#include "motionManager.h"
#include "sceneFieldX.h"
#include "car.h"
#include "course.h"
// XAudio2
#include "xa2SourceVoiceManager.h"
#include "xa2SourceVoice2D.h"
#include "xa2Listener.h"

std::string CMainController::m_strName;

//================================================================================
// 初期化処理
//--------------------------------------------------------------------------------
void CMainController::Init()
{

}

//================================================================================
// 終了処理
//--------------------------------------------------------------------------------
void CMainController::Uninit()
{

}

//================================================================================
// 更新処理
//--------------------------------------------------------------------------------
void CMainController::Update()
{
	CInputKeyboard *keyboard = CManager::GetKeyboard();	// キーボードの取得
	CInputXinput *xInput = CManager::GetXInput();

	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);

	// モード取得
	CMode *pMode = CManager::GetMode();
	if (!pMode)return;
	if (pMode->GetModeType() == CMode::MODETYPE::LOADING)return;

	CModeGame *pModeGame = nullptr;
	bool noGoal = true;
	bool start = false;
	if (pMode->GetModeType() == CMode::GAME)
	{
		pModeGame = static_cast<CModeGame*>(CManager::GetMode());
		CCourse *pCourse = pModeGame->GetCourse();				// コース
		noGoal = pCourse->GetGoalCount() == 0;
		start = pCourse->GetReadyCount() == 0;

		if (pCar)
		{
			// デバッグ用瞬間移動処理
			if(keyboard->GetPress(DIK_0)){
				pCar->SetPos(pCourse->GetCheckPoint(0));
			}
			else if (keyboard->GetPress(DIK_1)) {
				pCar->SetPos(pCourse->GetCheckPoint(1));
			}
			else if (keyboard->GetPress(DIK_2)) {
				pCar->SetPos(pCourse->GetCheckPoint(2));
			}
			else if (keyboard->GetPress(DIK_3)) {
				pCar->SetPos(pCourse->GetCheckPoint(3));
			}
			else if (keyboard->GetPress(DIK_4)) {
				pCar->SetPos(pCourse->GetCheckPoint(4));
			}
	
			// 車の更新
			pCar->Update();
			if (!start) {
				if (keyboard->GetTrigger(DIK_W) || xInput->GetRTrigger() > 42)
				{
					// SE_アクセル
					// -------- -------- -------- --------
					pCar->SoundEngin();
				}
				return;
			}
	
			// X,Z平面上の平行移動
			if (keyboard->GetPress(DIK_RBRACKET) || xInput->GetPress(XI_X))	// ブレーキ
			{
				// ブレーキ
				pCar->Brake(); 
			}
			else
			{
				int rT = xInput->GetRTrigger();
				// ↑
				if (keyboard->GetPress(DIK_W) && noGoal) { pCar->MoveForward(); }
				else if (rT > 0 && noGoal) { pCar->MoveForward(rT / 255.f); }
				// ↓
				else if ((keyboard->GetPress(DIK_S) || xInput->GetPress(XI_Y)) && noGoal) { pCar->MoveBackward(); }
				// 慣性
				else { pCar->MoveFrictional(); }
			}
	
			int LX = xInput->GetStickLX();
	
			if (keyboard->GetPress(DIK_A) && noGoal) {
				pCar->TurnRight(); 
			}				// ←
			else if (keyboard->GetPress(DIK_D) && noGoal) {
				pCar->TurnLeft(); 
			}				// →
			else if (LX > 2000 && noGoal){ 
				pCar->TurnRight(32767.f / LX); 
			}				// ←
			else if (LX < -2000 && noGoal) {
				pCar->TurnLeft(-32768.f / LX);
			}				// →
	
			// カメラ注視点設定
			SetCamPosAt();
	
#if _DEBUG
			// カメラを上空へ
			CCamera *pCamera = CManager::GetCamera();
			// スクショ位置に移動
			if (keyboard->GetPress(DIK_P))
			{
				// フィールドだけ描画
				CScene::DrawTypeOnly(CScene::CLASSTYPE::SCENE_FIELD);
				CSceneFieldX *pFieldX = (CSceneFieldX*)CScene::GetSceneClassType(CScene::CLASSTYPE::SCENE_FIELD);
				D3DXVECTOR3 size = pFieldX->GetSize();
	
				pCamera->SetCamPos(D3DXVECTOR3(size.x * 0.5f, 1000.f, size.z * -0.5f));
				pCamera->SetCamPosAt(D3DXVECTOR3(size.x * 0.5f, 0.f, size.z * -0.5f));
				pCar->SetPos(D3DXVECTOR3(size.x * 0.5f, 1000.f, size.z * -0.5f));
				pCamera->SetVecUp(D3DXVECTOR3(0.f, 0.f, -1.f));
			}
			else
			{
				CScene::DrawTypeOnly(CScene::CLASSTYPE::CLASSTYPE_NONE);
				pCamera->SetVecUp(D3DXVECTOR3(0.f, 1.f, 0.f));
			}
#endif
		}
	}
}

//================================================================================
// 描画処理
//--------------------------------------------------------------------------------
void CMainController::Draw()
{

}

//================================================================================
// 座標
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetPos() const
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetPos();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// 1f前の座標
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetPosOld() const
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetPosOld();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// 回転
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetRot() const
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetRot();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//================================================================================
// 移動量
//--------------------------------------------------------------------------------
D3DXVECTOR3 CMainController::GetMove() const
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetMove();
	else return D3DXVECTOR3(0.f, 0.f, 0.f);
}

float CMainController::GetMaxSpeed()const
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	if (pCar) return pCar->GetMaxSpeed();
	else return 0.f;
}

//================================================================================
// カメラ注視点の設定
//--------------------------------------------------------------------------------
void CMainController::SetCamPosAt()
{
	// 車の取得
	CCar *pCar = CCar::GetCar(m_strName);
	D3DXVECTOR3 camPosAt = pCar->GetCamPosAt();

	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetLockAt(camPosAt);
	pCamera->SetMoveRotFlame(0.5f);
}