// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : カメラ
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include <algorithm>
#include <assert.h>
#include "camera.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "utility.h"

static const float CAM_MOVE_SPEED = 1.f;

// ======== ======== ======== ======== ======== ======== ======== ========
// コンストラクタ
// -------- -------- -------- -------- -------- -------- -------- --------
CCamera::CCamera()
{
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);		// 座標
	m_vecUp = XMFLOAT3(0.f, 0.f, 0.f);		// カメラの上方向
	m_posAt = XMFLOAT3(0.f, 0.f, 0.f);		// 注視点
	m_moveRot = XMFLOAT3(0.f, 0.f, 0.f);	// 回転量
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);
	m_len = 0.f;
	m_moveRotFlame = 0.98f;
	m_mtxView = XM::F4X4::Identity(&m_mtxView);	// ビュー座標
	m_pField = nullptr;
	m_pConstantBuffer = nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Init(void)
{
	m_pos = CAM_LEN;							// 座標
	m_vecUp = XMFLOAT3(0.f, 1.f, 0.f);			// カメラの上方向
	m_posAt = XMFLOAT3(0.f, 0.f, 0.f);			// 注視点
	m_moveRot = XMFLOAT3(0.f, 0.f, 0.f);		// 回転量
	m_moveRotFlame = 0.98f;
	m_pField = nullptr;
	m_mapCap = false;
	m_atObjType = AT_OBJTYPE::NONE;

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
	XMFLOAT3 vec3 = XMFLOAT3(m_posAt.x - m_pos.x, 0.f, m_posAt.z - m_pos.z);
	m_len = sqrt(vec3.x * vec3.x + vec3.z * vec3.z);

	// デバイスの取得
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// コンスタントバッファの作成
	D3D11_BUFFER_DESC bd;
	m_pConstantBuffer = nullptr;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	if (FAILED(pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer)))
	{
		MessageBox(nullptr, "コンスタントバッファの作成失敗", "警告", S_OK);
	}

	// 定数バッファ作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBufferModel);
	bd.CPUAccessFlags = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
		MessageBox(nullptr, "定数バッファ作成失敗", "Err", MB_ICONSTOP);
		return;
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Uninit(void)
{
	SAFE_RELEASE(m_pConstantBuffer);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Update(void)
{
	// キーボード
	CInputKeyboard *key = CManager::GetKeyboard();

	// 注視点座標の設定
	XMFLOAT3 rotObj = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 lookAt = XMFLOAT3(0.f, 0.f, 0.f);

	// カメラ回転
	if (key->GetPress(DIK_C))AddCamRoateY(0.02f);
	if (key->GetPress(DIK_Z))AddCamRoateY(-0.02f);
	if (key->GetPress(DIK_R))AddCamPosY(0.02f);
	if (key->GetPress(DIK_F))AddCamPosY(-0.02f);
	if (key->GetPress(DIK_E))m_len = max(1.f, m_len - 0.02f);
	if (key->GetPress(DIK_Q))m_len += (0.02f);

	// カメラ位置
	m_pos.x = m_len * cosf(m_rot.y) + m_posAt.x;
	m_pos.z = m_len * sinf(m_rot.y) + m_posAt.z;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 回転
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::AddCamRoateY(const float &moveRot)
{
	// Y
	m_rot.y += moveRot;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 回転
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::AddCamPosY(const float &move)
{
	// X
	m_pos.y += move;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 注視点設定
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::SetLockAt(XMFLOAT3 lookAt)
{
	m_posAt = lookAt;
}

//using DIRECTX;

// ======== ======== ======== ======== ======== ======== ======== ========
// カメラ設定
// -------- -------- -------- -------- -------- -------- -------- --------
void CCamera::Set(void)
{
	// デバイスの取得
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// スクリーンサイズ取得
	XMFLOAT2 screenSize = CManager::GetRenderer()->GetScreenSize();

	// マトリックスの初期化
	m_mtxWorld = XM::F4X4::Identity(&m_mtxWorld);		// ワールド
	m_mtxView = XM::F4X4::Identity(&m_mtxView);			// ビュー

	// ビュー
	XMVECTOR hEye = XMLoadFloat3(&m_pos);			// カメラの位置
	XMVECTOR hAt = XMLoadFloat3(&m_posAt);			// 焦点の位置
	XMVECTOR hUp = XMLoadFloat3(&m_vecUp);			// 上方向ベクトル
	XMMATRIX hWorld = XMLoadFloat4x4(&m_mtxWorld);	// ワールド変換行列
	XMMATRIX hView = XMLoadFloat4x4(&m_mtxView);	// ビュー変換行列
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);		// ビュー
	XMStoreFloat4x4(&m_mtxView, hView);				// 戻す

	//透視射影変換行列
	XMMATRIX hProjection = XMLoadFloat4x4(&m_mtxProj);
	hProjection = XMMatrixPerspectiveFovLH(ToRadian(60.0f), screenSize.x / screenSize.y, CAM_NEAR, CAM_FAR);
	XMStoreFloat4x4(&m_mtxProj, hProjection);		// 戻す

	//
	// 変数の更新
	//
	ConstantBuffer hConstantBuffer;
	hConstantBuffer.mView = XMMatrixTranspose(hView);
	hConstantBuffer.mProjection = XMMatrixTranspose(hProjection);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &hConstantBuffer, 0, 0);
	CBufferModel hBufferScene;
	hBufferScene.mWorld = XMMatrixTranspose(hWorld);
	m_lightPos = XMFLOAT4(10.0f, 40.0f, -70.0f, 1.0f);
	hBufferScene.mLight = m_lightPos;
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &hBufferScene, 0, 0);

	//コンテキストに設定
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);
}
