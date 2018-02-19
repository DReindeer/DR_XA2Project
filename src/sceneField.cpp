//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// インクルードファイル
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "scene3D.h"
#include "sceneField.h"
#include "sceneSkydome.h"
#include "manager.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "texManager.h"
#include "camera.h"
#include "light.h"
#include "renderer.h"

static const int FACE_X = 3;
static const int FACE_Z = 3;

float meshPosY[FACE_X + 1][FACE_Z + 1] =
{
	{ 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 3.f, 0.f },
	{ 0.f, 0.f, 3.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f }
};

//================================================================================
// 初期化処理
//--------------------------------------------------------------------------------
void CSceneField::Init( void )
{
	// ここでファイル読み込み & 面の数の取得
	m_numFaceX = FACE_X;
	m_numFaceZ = FACE_Z;

	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// 座標
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// 角度
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// 拡大率
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// サイズ
	m_vecVtxNom.clear();					// 頂点法線
	m_numVtx = 0;							// Vtx数
	m_numIndex = 0;							// インデックス数
	m_pIdxBuffer = nullptr;					// インデックスバッファ
	m_pVtxBuffer = nullptr;					// 頂点バッファ
	m_strFilePass.clear();					// テクスチャの名前
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CSceneField::Uninit()
{
	// インデックスバッファの解放
	if(m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// 親クラスの終了
	CScene3D::Uninit();
}

//================================================================================
// 更新処理
//--------------------------------------------------------------------------------
void CSceneField::Update( void )
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	WORD *pIdx;			// バッファをロックする
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	int id = 0;
	for (int nCntZ = 0; nCntZ < m_numFaceZ; nCntZ++)
	{
		if (nCntZ != 0)	// 一番初めじゃなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1));
			id = ((m_numFaceX + 1) * (nCntZ + 1));
			nCnt++;
		}
		// 2点打つ(手前, 奥)
		pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * nCntZ);
		id = ((m_numFaceX + 1) * nCntZ);
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1));
		id = ((m_numFaceX + 1) * (nCntZ + 1));
		nCnt++;

		for (int nCntX = 0; nCntX < m_numFaceX; nCntX++)
		{
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * nCntZ + nCntX + 1);
			id = ((m_numFaceX + 1) * nCntZ + nCntX + 1);
			nCnt++;
			// 2点打つ(手前, 奥)
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1) + nCntX + 1);
			id = ((m_numFaceX + 1) * (nCntZ + 1) + nCntX + 1);
			nCnt++;
		}
		if (nCntZ != m_numFaceZ - 1)	// 最後じゃなかったら
		{
			// 一点打つ
			pIdx[nCnt] = static_cast<WORD>(m_numFaceX + (nCntZ * (m_numFaceX + 1)));
			id = (m_numFaceX + (nCntZ * (m_numFaceX + 1)));
			nCnt++;
		}
	}
	//ロックの解除
	m_pIdxBuffer->Unlock();

	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			// 座標
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].pos = m_vecVtxPos.at(nCntZ * (m_numFaceX + 1) + nCntX);

			// 色
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

			// テクスチャ座標
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].tex = D3DXVECTOR2(1.f * nCntX, 1.f * nCntZ);
		}
	}

	for (int i = 0, max = m_vecVtxNom.size(); i < max; i++)
	{
		// 頂点法線の設定
		pVtx[i].nom = m_vecVtxNom[i];
	}
	// ロックの解除
	m_pVtxBuffer->Unlock();
}

//================================================================================
// ポリゴン描画
//--------------------------------------------------------------------------------
void CSceneField::Draw( void )
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxScl;					// 拡大行列
	D3DXMATRIX mtxRot;					// 回転行列
	D3DXMATRIX mtxPos;					// 平行行列

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 scl = GetScl();

	// ワールドマトリクスの作成
	D3DXMatrixIdentity(&m_mtxWorld);	// 行列を単位行列にする( 拡大率1,他0で初期化 )
	D3DXMatrixIdentity(&mtxScl);		//
	D3DXMatrixIdentity(&mtxRot);		//
	D3DXMatrixIdentity(&mtxPos);		//

	// ワールド変換行列を設定して変換
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);				// 拡大行列
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// 
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);			// 平行移動行列

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// インデックス番号をデバイスに設定
	pDevice->SetIndices(m_pIdxBuffer);

	// テクスチャをパイプラインにセット
	if(!m_strFilePass.empty())
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}else {pDevice->SetTexture(0, nullptr);}

	// 頂点バッファをデバイスのデータ ストリームにバインドする
	pDevice->SetStreamSource(0,	m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// プリミティブの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);
}

//--------------------------------------------------------------------------------
// 頂点作成
//--------------------------------------------------------------------------------
HRESULT CSceneField::SetVtx()
{
	m_numVtx = (m_numFaceX + 1) * (m_numFaceZ + 1);							// 頂点数
	m_numIndex = (2 + 2 * m_numFaceX) * m_numFaceZ + (m_numFaceZ - 1) * 2;	// インデックス数(ポリゴン描画時に必要な頂点数)

// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Deviceの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_numVtx,			// メモリのサイズ
		D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
		FVF_VERTEX_3D,						// 使用する頂点フォーマット(見ない)
		D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "3D用VtxBufferの作成に失敗しました", "警告", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_numIndex,		// バッファ量
		D3DUSAGE_WRITEONLY,			// 使用フラグ
		D3DFMT_INDEX16,				// 必要なバッファ量
		D3DPOOL_MANAGED,			// メモリの管理方法
		&m_pIdxBuffer,
		nullptr)))
	{
		// 失敗したらエラーを返す
		MessageBox(nullptr, "IdxBufferの作成に失敗しました", "警告", MB_OK);
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	// バッファをロックする
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1ブロックの幅と高さ
	D3DXVECTOR3 size = GetSize();
	m_faceSize.x = size.x / m_numFaceX;
	m_faceSize.y = size.z / m_numFaceZ;

	// 座標
	D3DXVECTOR3 pos = GetPos();

	// 座標の設定
	for(int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for(int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			// 座標
			m_vecVtxPos.push_back(D3DXVECTOR3(m_faceSize.x * nCntX - size.x * 0.5f + pos.x,
									meshPosY[nCntZ][nCntX] + pos.y,
									m_faceSize.y * (nCntZ)-size.z * 0.5f + pos.z));
		}
	}
	
	// 頂点法線の設定
	for (int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			if (nCntZ == 0 || nCntZ == m_numFaceZ || nCntX == 0 || nCntX == m_numFaceX)
			{
				m_vecVtxNom.push_back(D3DXVECTOR3(0.f, 1.f, 0.f));
				continue;
			}

			// 頂点法線を計算
			D3DXVECTOR3 dirX, dirZ;
			D3DXVECTOR3 nomX, nomZ;

			int prevX = (nCntZ) * (m_numFaceX + 1) + (nCntX - 1);
			int nextX = (nCntZ) * (m_numFaceX + 1) + (nCntX + 1);
			int prevZ = (nCntZ - 1) * (m_numFaceX + 1) + (nCntX);
			int nextZ = (nCntZ + 1) * (m_numFaceX + 1) + (nCntX);

			dirX = m_vecVtxPos[nextX] - m_vecVtxPos[prevX];
			dirZ = m_vecVtxPos[prevZ] - m_vecVtxPos[nextZ];
			nomX.x = -dirX.y;
			nomX.y = dirX.x;
			nomX.z = 0.f;
			nomZ.x = 0.f;
			nomZ.y = dirZ.z;
			nomZ.z = -dirZ.y;

			D3DXVECTOR3 nom = nomX + nomZ;
			D3DXVec3Normalize(&nom, &nom);
			m_vecVtxNom.push_back(nom);
		}
	}

	// ロックの解除
	m_pVtxBuffer->Unlock();

	return NOERROR;
}

float CSceneField::GetHeight(D3DXVECTOR3 pos, bool *hit)
{
	D3DXVECTOR3 v12, v20, v01;
	D3DXVECTOR3 nom;
	D3DXVECTOR3 p0, p1, p2, p3;
	float y;

	// 1ブロックの幅と高さ
	D3DXVECTOR3 size = GetSize();
	m_faceSize.x = size.x / m_numFaceX;
	m_faceSize.y = size.z / m_numFaceZ;

	// 四角ポリゴンの最初の頂点のインデックスを求める
	int iz = static_cast<int>((pos.z + (size.z * 0.5f)) / (m_faceSize.y));
	int ix = static_cast<int>((pos.x + (size.x * 0.5f)) / (m_faceSize.x));
	int i = iz * (m_numFaceX + 1) + ix;
	if ((pos.x + (size.x * 0.5f)) < 0 || ix > m_numFaceX - 1 || (pos.z + (size.z * 0.5f)) < 0 || iz > m_numFaceZ - 1)
	{
		if (hit)*hit = false;
		return 0.f;
	}

	// 四角ポリゴンの各頂点を求める
	p0 = m_vecVtxPos[i];
	p1 = m_vecVtxPos[i + 1];
	p2 = m_vecVtxPos[i + (m_numFaceX + 1)];
	p3 = m_vecVtxPos[i + (m_numFaceX + 1) + 1];

	D3DXVECTOR3 vtxVec = p1 - p2; 
	vtxVec.y = 0.f;
	D3DXVECTOR3 posVec = pos - p2;
	posVec.y = 0.f;
	D3DXVECTOR3 cross(0.f, 0.f, 0.f);
	D3DXVec3Cross(&cross, &vtxVec, &posVec);

	if (cross.y >= 0.f)
	{
		TriangleCloss(p1, p2, p0, &nom);
		y = p1.y - ((pos.x - p1.x) * nom.x + (pos.z - p1.z) * nom.z) / nom.y;
		if (hit)*hit = true;
		return y;
	}
	else 
	{
		TriangleCloss(p1, p3, p2, &nom);
		y = p1.y - ((pos.x - p1.x) * nom.x + (pos.z - p1.z) * nom.z) / nom.y;
		if (hit)*hit = true;
		return y;
	}
}

bool CSceneField::TriangleCloss(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 *nom)
{
	D3DXVECTOR3 v01 = p0 - p1;
	D3DXVECTOR3 v12 = p1 - p2;
	D3DXVECTOR3 v20 = p2 - p0;

	// 面法線
	D3DXVECTOR3 vv1 = v12 - v20;
	D3DXVECTOR3 vv2 = v01 - v12;
	D3DXVec3Cross(nom, &vv1, &vv2);
	D3DXVec3Normalize(nom, nom);
	return true;
}

//--------------------------------------------------------------------------------
// 作成処理
//--------------------------------------------------------------------------------
CSceneField *CSceneField::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CSceneFieldの生成
	CSceneField *pSceneField;
	pSceneField = new CSceneField();
	pSceneField->Init();

	pSceneField->SetPos(pos);
	pSceneField->SetRot(rot);
	pSceneField->SetSize(size);
	pSceneField->m_strFilePass = strFilePass;

	// 頂点設定
	pSceneField->SetVtx();
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneField;
}
