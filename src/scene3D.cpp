//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "scene3D.h"
#include "renderer.h"

CScene3D::SHADER_SET *CScene3D::m_pShaderSet = nullptr;	// シェーダー

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CScene3D::CScene3D(int priority): CScene(priority), m_r(255), m_g(255), m_b(255), m_a(255), m_pMat(nullptr)
{
	m_classType = CLASSTYPE::SCENE_3DPOLY;
	D3DXMatrixIdentity(&m_mtxWorld);			// ワールド座標
	m_pVtxBuffer = nullptr;						// Vtxバッファ
	m_numVtx = 0;
	m_numIndex = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	if (m_pShaderSet == nullptr)
	{
		// シェーダーの設定
		CreateShader();

		// 新しいメッシュの頂点要素
		//----------------------------------------------------------------
		// デクラレーション
		D3DVERTEXELEMENT9 decl[] = {
			// パイプライン, オフセット, 型, , セマンティクス, ,
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()	// 終端子マクロ(ターミネータ)
		};
		pDevice->CreateVertexDeclaration(decl, &m_pShaderSet->pVertexDeclaration);
	}
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CScene3D::~CScene3D()
{
	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	if (m_pMat)
	{
		delete m_pMat;
		m_pMat = nullptr;
	}

	// Vtxバッファの解放
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

void CScene3D::CreateShader()
{
	// 生成
	m_pShaderSet = new SHADER_SET;
	m_pShaderSet->pVertexDeclaration = nullptr;
	m_pShaderSet->pPConstantTable = nullptr;
	m_pShaderSet->pPConstantTableNoTex = nullptr;
	m_pShaderSet->pPixelShader = nullptr;
	m_pShaderSet->pPixelShaderNoTex = nullptr;
	m_pShaderSet->pVConstantTable = nullptr;
	m_pShaderSet->pVertexShader = nullptr;

	// スレッドセーフ(ロック)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// シェーダー読み込み用変数
	LPD3DXBUFFER err = NULL;
	LPD3DXBUFFER code = NULL;

	// 頂点シェーダーのコンパイル
	//----------------------------------------------------------------
	HRESULT hr = D3DXCompileShaderFromFile("hlsl/SpeclarVS.hlsl", NULL, NULL, "main", "vs_3_0", 0, &code, &err, &m_pShaderSet->pVConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(VS)", MB_OK);
		err->Release();
		return;
	}
	// 頂点シェーダーの生成
	hr = pDevice->CreateVertexShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダー生成失敗", "CreateVertexShader(VS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/SpeclarPS.hlsl", NULL, NULL, "main", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTable);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// ピクセルシェーダーのコンパイル(テクスチャ無し)
	//----------------------------------------------------------------
	hr = D3DXCompileShaderFromFile("hlsl/SpeclarPS.hlsl", NULL, NULL, "NoTexture", "ps_3_0", 0, &code, &err, &m_pShaderSet->pPConstantTableNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)err->GetBufferPointer(), "D3DXCompileShaderFromFile(PS)", MB_OK);
		err->Release();
		return;
	}
	// ピクセルシェーダーの生成
	hr = pDevice->CreatePixelShader((LPDWORD)code->GetBufferPointer(), &m_pShaderSet->pPixelShaderNoTex);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダー生成失敗", "CreatePixelShader(PS)", MB_OK);
		err->Release();
		return;
	}

	// リリース
	if (err)err->Release();
	if (code)code->Release();
}
