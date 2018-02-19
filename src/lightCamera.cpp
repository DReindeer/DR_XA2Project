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
#include "lightcamera.h"
#include "debugStr.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "sceneModel.h"
#include "mainController.h"
#include "sceneField.h"
#include "car.h"
#include "modelManager.h"
#include "modeGame.h"
#include "course.h"

static const float LIGHTCAM_NEAR = 1.f;
static const float LIGHTCAM_FAR = 40.f;
static const D3DXVECTOR3 LIGHT_POS = D3DXVECTOR3(0.f, 8.f, 5.f);

//================================================================================
// コンストラクタ
//--------------------------------------------------------------------------------
CLightCamera::CLightCamera() : m_near(LIGHTCAM_NEAR), m_far(LIGHTCAM_FAR)
{
	m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);			// 座標
	m_posAt = D3DXVECTOR3(0.f, 0.f, 0.f);		// 注視点
	m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);		// カメラの上方向
}

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CLightCamera::Init()
{
	m_pos = D3DXVECTOR3(-5.f, 16.f, 3.f);

	m_pModelManager = new CModelManager;
	m_p3DObj = CSceneModel::Create(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.01f, 0.01f, 0.01f), "data/MODEL/MD/Sheep.x", 4, m_pModelManager);
	m_p3DObj->SetOverall();
	m_p3DObj->SetDrawFlag(false);
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CLightCamera::Uninit()
{
	if (m_pModelManager)
	{
		m_pModelManager->ReleaseAll();
		delete m_pModelManager;
		m_pModelManager = nullptr;
	}
}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CLightCamera::Update()
{

}

//================================================================================
// WVの返却
//--------------------------------------------------------------------------------
D3DXMATRIX CLightCamera::GetMtxView()
{
	return m_mtxView;
}

//================================================================================
// WVPの返却
//--------------------------------------------------------------------------------
D3DXMATRIX CLightCamera::GetMtxProj()
{
	return m_mtxProj;
}

const D3DXVECTOR3 &CLightCamera::GetDir()
{
	return m_lightVecAt;
}

//================================================================================
// カメラ設定
//--------------------------------------------------------------------------------
void CLightCamera::SetOrthoLH()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxView);			// ビュー座標

	// モード取得
	CMode *pMode = CManager::GetMode();
	if (!pMode)return;
	if (0 /*pMode->GetModeType() == CMode::MODETYPE::GAME*/)
	{
		// コース情報取得
		CModeGame *pModeGame = (CModeGame*)pMode;
		CCourse *pCourse = pModeGame->GetCourse();
		D3DXVECTOR3 size = pCourse->GetCourseSize();
		D3DXVECTOR3 atPos = m_pos = pCourse->GetCourseCenterPos();
		atPos.y = pCourse->GetCheckPoint(4).y;
		m_pos.y = pCourse->GetStartPos().y + size.y + 14.f *2.5f;

//		m_pos = atPos + D3DXVECTOR3(0.f, pCourse->GetStartPos().y * 1.2f, 0.f);
		m_lightVecAt = atPos - m_pos;
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&m_lightVecAt, &m_lightVecAt);
		D3DXVECTOR3 vecRight, vecUp = D3DXVECTOR3(0.f, 0.f, 1.f);
//		D3DXVec3Cross(&vecRight, &m_lightVecAt, &vecUp);
//		D3DXVec3Cross(&vecUp, &vecRight, &m_lightVecAt);
		D3DXVec3Normalize(&vecUp, &vecUp);
		// ビュー変換行列
		D3DXMatrixLookAtLH(&m_mtxView, &m_pos, &atPos, &vecUp);

		// オルソー行列の作成
		m_far = size.y + 18.f;
		D3DXMatrixOrthoLH(&m_mtxProj, size.x * 1.1f, size.z * 1.1f, m_near, m_far);
	}
	else
	{
		D3DXVECTOR3 atPos = D3DXVECTOR3(0.f, 0.f, 0.f) + D3DXVECTOR3(0.f, 0.f, 0.f);
		m_pos = atPos + LIGHT_POS;
		m_lightVecAt = atPos - m_pos;
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3Normalize(&m_lightVecAt, &m_lightVecAt);
		D3DXVECTOR3 vecRight, vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXVec3Cross(&vecRight, &m_lightVecAt, &vecUp);
		D3DXVec3Cross(&vecUp, &vecRight, &m_lightVecAt);
		D3DXVec3Normalize(&vecUp, &vecUp);
		// ビュー変換行列
		D3DXMatrixLookAtLH(&m_mtxView, &m_pos, &atPos, &vecUp);

		// オルソー行列の作成
		D3DXMatrixOrthoLH(&m_mtxProj, 64.f, 64.f, m_near, m_far);
	}

	// ワールド座標
	D3DXMATRIX mtxRot, mtxPos;			
	D3DXMatrixIdentity(&m_mtxWorld);	//
	D3DXMatrixIdentity(&mtxRot);		//
	D3DXMatrixIdentity(&mtxPos);		// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// 回転行列
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);			// 平行行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにビュー変換行列を設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// デバイスにプロジェクション変換行列を設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);

	m_p3DObj->SetPos(m_pos);
	m_p3DObj->SetRot(m_rot);
}
