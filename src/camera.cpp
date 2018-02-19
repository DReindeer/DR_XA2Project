//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include <algorithm>
#include "camera.h"
#include "debugStr.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mainController.h"
#include "sceneField.h"
#include "car.h"

#include "xa2Listener.h"

static const float CAM_MOVE_SPEED = 1.f;

//================================================================================
// コンストラクタ
//--------------------------------------------------------------------------------
CCamera::CCamera() : m_near(CAM_NEAR * CManager::GetMasterScale()), m_far(CAM_FAR * CManager::GetMasterScale())
{
	D3DXMatrixIdentity(&m_mtxView);				// ビュー座標
	m_pField = nullptr;
}

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CCamera::Init()
{
	m_atObjType = AT_OBJTYPE::NONE;
	m_moveRot = D3DXVECTOR3(0.f, 0.f, 0.f);		// 回転量
	m_moveRotFlame = 0.98f;
	m_camLen = CAM_LEN * CManager::GetMasterScale();
	m_pos = m_camLen;						// 座標
	m_posOld = m_pos;
	m_pField = nullptr;
	m_mapCap = false;
	m_atAngle = D3DX_PI / 4.f;

	// 注視点座標の設定
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// タイプの取得
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE::FIELD)
			{
				// ダウンキャスト
				m_pField = ((CSceneField*)pScene);
				continue;
			}
		}
	}

	// カメラの距離の代入
	D3DXVECTOR3 vec3 = D3DXVECTOR3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);			// カメラの上方向

	// リスナー
	if (!m_pListener)
	{
		m_pListener = new XA2Listener();
		m_pListener->SetCone(D3DX_PI * 0.75f, 1.1f, 0.8f);	// コーン
		m_pListener->SetPos(m_pos);							// 座標
		m_pListener->SetFront(m_posAt - m_pos);				// 前方向
		m_pListener->SetTop(m_vecUp);						// 上方向
		m_pListener->SetVelocity(m_pos - m_posOld);			// 移動量
	}
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CCamera::Uninit()
{
	// リスナー
	if (m_pListener)
	{
		delete m_pListener;
		m_pListener = nullptr;
	}
}

//================================================================================
// 座標の保存
//--------------------------------------------------------------------------------
void CCamera::UpdateBigin()
{
	m_posOld = m_pos;
}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CCamera::Update()
{
	// 注視点座標の設定
	D3DXVECTOR3 rotObj = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (m_atObjType == AT_OBJTYPE::CONTROL_PLAYER)
	{
		std::string name = CMainController::GetName();
		if (name.empty())return;
		CCar *pCar = CCar::GetCar(name);
		rotObj = pCar->GetRot();
		lookAt = pCar->GetCamPosAt();
		lookAt.y += m_camLen.y;
		m_posAt = lookAt;

		//======================================
		// カメラは常にプレイヤーの頭の後ろ
		//--------------------------------------
		m_rot.y = rotObj.y * -1.f + D3DX_PI * 0.5f + m_moveRot.y;
		m_pos = m_posAt - D3DXVECTOR3(cosf(m_rot.y) * m_camLen.z, 0.f, sinf(m_rot.y) * m_camLen.z);
		m_moveRot.y += (0.f - m_moveRot.y) * m_moveRotFlame;
	}

	// カメラの距離の代入
	D3DXVECTOR3 vec3 = D3DXVECTOR3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	// リスナー更新
	//--------------------------------------------------------------------------------
	m_pListener->SetPos(m_pos);							// 座標

	D3DXVECTOR3 vecRight, vecFront = m_posAt - m_pos;	// 前方向
	m_pListener->SetFront(vecFront);					// 

	D3DXVec3Cross(&vecRight, &m_vecUp, &vecFront);		// 右方向
	D3DXVec3Cross(&m_vecUp, &vecFront, &vecRight);		// 上方向
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);				// 
	m_pListener->SetTop(m_vecUp);						// 

	D3DXVECTOR3 move = m_pos - m_posOld;				// 移動量
	m_pListener->SetVelocity(move);						//
}

//================================================================================
// 回転
//--------------------------------------------------------------------------------
void CCamera::AddCamRoate(const float &moveRot)
{
	// Y
	m_moveRot.y += moveRot;
	m_pos.x = m_len * cosf(m_moveRot.y) + m_posAt.x;
	m_pos.z = m_len * sinf(m_moveRot.y) + m_posAt.z;
}

//================================================================================
// 注視点設定
//--------------------------------------------------------------------------------
void CCamera::SetLockAt(D3DXVECTOR3 lookAt)
{
	lookAt.y += m_camLen.y;
	m_posAt = lookAt;
}

//================================================================================
// カメラ設定
//--------------------------------------------------------------------------------
void CCamera::Set()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixLookAtLH(&m_mtxView, &m_pos, &m_posAt, &m_vecUp);	// ビュー変換行列

	// デバイスにビュー変換行列を設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXVECTOR2 screenSize = CManager::GetRenderer()->GetScreenSize();

	// プロジェクション行列の作成
	if (1)
	{
		// パースペクティブ行列の作成
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,		// プロジェクション行列のアドレス
			m_atAngle,								// 注視角
			(float)screenSize.x / screenSize.y,		// アスペクト比 (スクリーンのサイズで割る)
			m_near,								// near
			m_far);								// far
	}
	else
	{
		// ビュー変換行列
		CMainController *pPlayer = CManager::GetMainController();
		D3DXVECTOR3 playerPos = pPlayer->GetPos();
		D3DXVECTOR3 pos = playerPos + D3DXVECTOR3(1.f, 64.f, 24.f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 lightVecAt = playerPos - pos;
		D3DXVec3Normalize(&lightVecAt, &lightVecAt);
		D3DXVECTOR3 vecRight, vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXVec3Cross(&vecRight, &lightVecAt, &vecUp);
		D3DXVec3Cross(&vecUp, &vecRight, &lightVecAt);
		D3DXVec3Normalize(&vecUp, &vecUp);
		D3DXMatrixLookAtLH(&m_mtxView, &pos, &playerPos, &vecUp);

		// デバイスにビュー変換行列を設定
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

		// オルソー行列の作成
		D3DXMatrixOrthoLH(&m_mtxProj, 32.f, 32.f, 0.1f, 200.f);
	}

	// デバイスにプロジェクション変換行列を設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//================================================================================
// スクリーン座標をワールド座標に変換
//--------------------------------------------------------------------------------
D3DXVECTOR3* CCamera::CalcScreenToWorld(
	D3DXVECTOR3* pout,	// 出力
	int Sx,				// スクリーンX座標
	int Sy,				// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0～1）
	int Screen_w,		// スクリーン幅
	int Screen_h,		// スクリーン高さ
	const D3DXMATRIX &View,	// ビュー行列
	const D3DXMATRIX &Prj)	// プロジェクション行列
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, nullptr, &View);
	D3DXMatrixInverse(&InvPrj, nullptr, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.f;
	VP._22 = -Screen_h / 2.f;
	VP._41 = Screen_w / 2.f;
	VP._42 = Screen_h / 2.f;
	D3DXMatrixInverse(&InvViewport, nullptr, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 VecS = D3DXVECTOR3(static_cast<float>(Sx), static_cast<float>(Sy), static_cast<float>(fZ));
	D3DXVec3TransformCoord(pout, &VecS, &tmp);

	return pout;
}