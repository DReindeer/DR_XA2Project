//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "bar.h"
#include "renderer.h"

std::unordered_map<std::string, CBar2D*> CBar2D::umapBar2D;

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CBar2D::Init()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Init();
	}
}

//================================================================================
// 終了
//--------------------------------------------------------------------------------
void CBar2D::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		if(m_pBar[i] == nullptr)continue;
		m_pBar[i]->Uninit();
	}
}

//================================================================================
// 更新
//--------------------------------------------------------------------------------
void CBar2D::Update()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Update();
	}

	// 中間にあるバーの追従
	followingBar();
}

//================================================================================
// 中間バーの処理
//--------------------------------------------------------------------------------
void CBar2D::followingBar()
{
	D3DXVECTOR3 folSize = m_pBar[1]->GetSize();		// 中間バーサイズ
	D3DXVECTOR3 hpBarSize = m_pBar[2]->GetSize();	// HPバーサイズ

	if (folSize.x <= hpBarSize.x)
	{
		// サイズと座標を一致させる処理
		m_pBar[1]->SetSize(hpBarSize);
		m_pBar[1]->SetPos(m_pBar[2]->GetPos());
		return;
	}

	// サイズと座標を一致させる処理
	if (folSize.x - 0.5f < hpBarSize.x)
	{
		m_pBar[1]->SetSize(hpBarSize);
		m_pBar[1]->SetPos(m_pBar[2]->GetPos());
	}
	else if(folSize.x > hpBarSize.x)
	{// 中間バー減衰
		m_pBar[1]->AddSize(D3DXVECTOR3(-0.5f, 0.f, 0.f));
		m_pBar[1]->AddPos(D3DXVECTOR3(-0.5f, 0.f, 0.f));
	}
}

//================================================================================
// 描画
//--------------------------------------------------------------------------------
void CBar2D::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Draw();
	}
}

//================================================================================
// 全バー更新
//--------------------------------------------------------------------------------
void CBar2D::UpdateAll()
{
	for (auto bar2D : umapBar2D) { bar2D.second->Update(); }
}

//================================================================================
// 全バー描画
//--------------------------------------------------------------------------------
void CBar2D::DrawAll()
{
	for (auto bar2D : umapBar2D) { bar2D.second->Draw(); }
}

//================================================================================
// 全バー解放
//--------------------------------------------------------------------------------
void CBar2D::ReleaseAll()
{
	for (auto bar2D : umapBar2D) 
	{
		if (bar2D.second != nullptr)
		{
			// 解放処理
			bar2D.second->Uninit();
			delete bar2D.second;
			bar2D.second = nullptr;
		}
	}
	// コンテナを空にする
	umapBar2D.clear();
}

//================================================================================
// 生成
//--------------------------------------------------------------------------------
CBar2D* CBar2D::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, int r, int g, int b, int a, float maxVal, std::string strName, int priority)
{
	assert(umapBar2D.count(strName) == 0 && "umapBar2Dに既に同名のパスが存在します");

	CBar2D *pBar2D = new CBar2D;

	pBar2D->m_maxVal = maxVal;
	pBar2D->m_basePos = D3DXVECTOR3(pos.x, pos.y, 0.f);
	pBar2D->m_strName = strName;

	for (int i = 0; i < 3; i++)
	{
		pBar2D->m_pBar[i] = new CScene2D(priority);
		pBar2D->m_pBar[i]->Init();
		pBar2D->m_pBar[i]->SetSize(D3DXVECTOR3(size.x, size.y, 0.f));
		pBar2D->m_pBar[i]->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.f));

		// スレッドセーフ(ロック)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		// D3Deviceの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 頂点情報の作成
		//---------------------
		if (FAILED(pDevice->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,			// メモリのサイズ
			D3DUSAGE_WRITEONLY,					// 使用方法(書き込みのみ)
			FVF_VERTEX_2D,						// 使用する頂点フォーマット(見ない)
			D3DPOOL_MANAGED,					// バッファの管理方法(勝手に復旧してくれる)
			pBar2D->m_pBar[i]->GetVtxBuffer(),							// 
			nullptr)))
		{
			// 失敗したらエラーを返す
			MessageBox(nullptr, "2D用VtxBufferの作成に失敗しました", "警告", MB_OK);
			return nullptr;
		}
	}
	pBar2D->m_pBar[0]->SetSize(D3DXVECTOR3(size.x + 2, size.y + 2, 0.f));
	pBar2D->m_pBar[0]->SetCol(int(r * 0.01f), int(g * 0.01f), int(b * 0.01f), a);
	pBar2D->m_pBar[1]->SetCol(int(r * 0.5f), int(g * 0.5f), int(b * 0.5f), a);
	pBar2D->m_pBar[2]->SetCol(r, g, b, a);

	umapBar2D[strName] = pBar2D;

	return umapBar2D[strName];
}

//================================================================================
// バーの値の増減設定
//--------------------------------------------------------------------------------
void CBar2D::AddVal(float addVal, std::string strName)
{
	if (umapBar2D.count(strName) == 0)return;
	CBar2D* pBar2D = umapBar2D[strName];

	D3DXVECTOR3 maxSize = pBar2D->m_pBar[0]->GetSize();
	D3DXVECTOR3 size = pBar2D->m_pBar[2]->GetSize();

	// 割合でバーの幅の増分を算出
	float val = addVal * maxSize.x / pBar2D->m_maxVal;
	
	if (size.x + val >= maxSize.x)
	{// 満タンだった場合は最大サイズに合わせる
		pBar2D->m_pBar[1]->SetSize(maxSize);
		pBar2D->m_pBar[1]->SetPos(pBar2D->m_basePos);
		pBar2D->m_pBar[2]->SetSize(maxSize);
		pBar2D->m_pBar[2]->SetPos(pBar2D->m_basePos);
	}
	else if (size.x + val <= 0.f)
	{// 0以下だった場合は0
		pBar2D->m_pBar[1]->SetSize(D3DXVECTOR3(0.f, maxSize.y, 0.f));
		pBar2D->m_pBar[1]->SetPos(D3DXVECTOR3(pBar2D->m_basePos.x, 0.f, 0.f));
		pBar2D->m_pBar[2]->SetSize(D3DXVECTOR3(0.f, maxSize.y, 0.f));
		pBar2D->m_pBar[2]->SetPos(D3DXVECTOR3(pBar2D->m_basePos.x, 0.f, 0.f));
	}
	else
	{
		pBar2D->m_pBar[2]->AddSize(D3DXVECTOR3(val, 0.f, 0.f));
		pBar2D->m_pBar[2]->AddPos(D3DXVECTOR3(val * 1.f, 0.f, 0.f));
	}
}
