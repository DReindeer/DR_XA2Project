// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3D系のシーンの親
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "manager.h"
#include "texManager.h"
#include "scene3D.h"
#include "renderer.h"
#include "utility.h"

#include <assert.h>
#include <tchar.h>

// -------- -------- -------- -------- -------- -------- -------- --------
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene3D::Init(void)
{
	m_pConstantBufferMat = nullptr;
	m_pConstantBufferModel = nullptr;
	m_pTexSampler = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelTexShader = nullptr;
	m_pVertexLayout = nullptr;

	// 変数の初期化
	SetPos(XMFLOAT3(0.f, 0.f, 0.f));		// 座標
	SetSize(XMFLOAT3(1.f, 1.f, 1.f));		// サイズ
	SetRot(XMFLOAT3(0.f, 0.f, 0.f));		// 回転量
	SetScl(XMFLOAT3(1.f, 1.f, 1.f));		// 拡大率
	m_posUV = XMFLOAT2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene3D::Uninit(void)
{
	SAFE_RELEASE(m_pConstantBufferMat);
	SAFE_RELEASE(m_pConstantBufferModel);
	SAFE_RELEASE(m_pTexSampler);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelTexShader);
	SAFE_RELEASE(m_pVertexLayout);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene3D::Update(void)
{
	// ワールド座標更新
	// -------- -------- -------- --------
	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene3D::Draw(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// UV(左上)
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene3D::SetTexturePos(XMFLOAT2 uv, XMFLOAT2 length)
{
	m_posUV = uv;
	m_heightUV = length.y;
	m_widthUV = length.x;
}
