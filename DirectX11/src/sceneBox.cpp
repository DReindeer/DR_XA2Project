// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : BOXのシーン(使わないかも)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "manager.h"
#include "texManager.h"
#include "sceneBox.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "inputKeyboard.h"
#include "utility.h"

#include <assert.h>
#include <tchar.h>

// -------- -------- -------- -------- -------- -------- -------- --------
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Init(void)
{
	// 変数の初期化
	SetPos(XMFLOAT3(0.f, 0.f, 0.f));		// 座標
	SetSize(XMFLOAT3(1.f, 1.f, 1.f));		// サイズ
	SetRot(XMFLOAT3(0.f, 0.f, 0.f));		// 回転量
	SetScl(XMFLOAT3(1.f, 1.f, 1.f));		// 拡大率
	m_move = XMFLOAT3(0.f, 0.f, 0.f);
	m_posUV = XMFLOAT2(0.f, 0.f);
	m_widthUV = 1.f;
	m_heightUV = 1.f;
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 頂点情報の作成
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::CreateVtxBuffer(void)
{
	// デバイスの取得
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// シェーダー読み込み
	BinFile vscode(L"data/SHADER/vs3D.cso");
	BinFile psTexcode(L"data/SHADER/psTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// 頂点座標の設定
	m_pVtxData[0].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[1].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	m_pVtxData[2].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);
	m_pVtxData[3].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
	m_pVtxData[4].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	m_pVtxData[5].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_pVtxData[6].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	m_pVtxData[7].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	m_pVtxData[8].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[9].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);

	m_pVtxData[10].pos = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	m_pVtxData[11].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	m_pVtxData[12].pos = XMFLOAT3(0.5f, 0.5f, -0.5f);
	m_pVtxData[13].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);

	m_pVtxData[14].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	m_pVtxData[15].pos = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	m_pVtxData[16].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	m_pVtxData[17].pos = XMFLOAT3(0.5f, -0.5f, -0.5f);

	// 法線の設定
	m_pVtxData[0].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[1].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));
	m_pVtxData[2].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, -1.f));
	m_pVtxData[3].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, -1.f));
	m_pVtxData[4].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, 1.f));
	m_pVtxData[5].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, 1.f));
	m_pVtxData[6].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, 1.f));
	m_pVtxData[7].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, 1.f));
	m_pVtxData[8].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[9].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));

	m_pVtxData[10].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, -1.f));
	m_pVtxData[11].nom = XM::F3::Normalize(XMFLOAT3(-1.f, 1.f, 1.f));
	m_pVtxData[12].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, -1.f));
	m_pVtxData[13].nom = XM::F3::Normalize(XMFLOAT3(1.f, 1.f, 1.f));

	m_pVtxData[14].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, 1.f));
	m_pVtxData[15].nom = XM::F3::Normalize(XMFLOAT3(-1.f, -1.f, -1.f));
	m_pVtxData[16].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, 1.f));
	m_pVtxData[17].nom = XM::F3::Normalize(XMFLOAT3(1.f, -1.f, -1.f));
	
	// UV座標の設定
	m_pVtxData[0].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[1].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[2].tex = XMFLOAT2(1.f, 1.f);
	m_pVtxData[3].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[4].tex = XMFLOAT2(2.f, 1.f);
	m_pVtxData[5].tex = XMFLOAT2(2.f, 0.f);
	m_pVtxData[6].tex = XMFLOAT2(3.f, 1.f);
	m_pVtxData[7].tex = XMFLOAT2(3.f, 0.f);
	m_pVtxData[8].tex = XMFLOAT2(4.f, 1.f);
	m_pVtxData[9].tex = XMFLOAT2(4.f, 0.f);

	m_pVtxData[10].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[11].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[12].tex = XMFLOAT2(1.f, 1.f);
	m_pVtxData[13].tex = XMFLOAT2(1.f, 0.f);

	m_pVtxData[14].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[15].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[16].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[17].tex = XMFLOAT2(1.f, 1.f);

	for (int i = 0; i < 18; i++)
	{
		// 頂点色の設定
		m_pVtxData[i].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}

	// 頂点バッファ作成
	// -------- -------- -------- -------- -------- -------- -------- --------
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_BOX_VERTEX;		// バッファサイズ
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 頂点バッファーとしてバインド
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_pVtxData;						// リソースデータのポインタ
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(nullptr, "頂点バッファ作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// インデックスバッファの作成
	// -------- -------- -------- -------- -------- -------- -------- --------
	WORD indices[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 
		10, 10, 11, 12, 13, 13, 
		14, 14, 15, 16, 17
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 22;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))){
		MessageBox(nullptr, "インデックスバッファの作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// 入力レイアウト定義
	// ５番目のパラメータは先頭からのバイト数なので，COLORにはPOSITIONのfloat型4バイト×3を記述
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT		, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 4 * 3 * 2 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 入力レイアウト作成
	m_pVertexLayout = nullptr;
	UINT elem_num = ARRAYSIZE(layout);
	if (FAILED(pDevice->CreateInputLayout(layout, elem_num, vscode.get(), vscode.size(), &m_pVertexLayout))) {
		MessageBox(nullptr, "入力レイアウト作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// 頂点シェーダ作成
	if (FAILED(pDevice->CreateVertexShader(vscode.get(), vscode.size(), nullptr, &m_pVertexShader))) {
		MessageBox(nullptr, "頂点シェーダー作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(pDevice->CreatePixelShader(pscode.get(), pscode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "ピクセルシェーダー作成失敗", "Err", MB_ICONSTOP);
		return;
	}
	if (FAILED(pDevice->CreatePixelShader(psTexcode.get(), psTexcode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "ピクセルシェーダー(テクスチャ用)作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pTexSampler);

	// 定数バッファ作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CCamera::CBufferModel);
	bd.CPUAccessFlags = 0;
	if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
		MessageBox(nullptr, "定数バッファ作成失敗", "Err", MB_ICONSTOP);
		return;
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Uninit(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexSampler);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// 描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneBox::Draw(void)
{
	// D3Deviceの取得
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	CCamera *cam = CManager::GetCamera();

	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();
	XMFLOAT3 move = GetMove();

	// ワールドマトリクス作成
	// -------- -------- -------- --------
	XMMATRIX mtxScl = XMMatrixScalingFromVector(XMLoadFloat3(&scl));							// 拡大
	XMMATRIX mtxRot;																			// 
	XMFLOAT3 up = XMFLOAT3(0.f, 1.f, 0.f);														// 
	XMVECTOR lookAt = XMLoadFloat3(&pos) + (XMLoadFloat3(&XM::F3::LookAt(rot.y + PI * 0.5f)));	// 
	XM::CalcLookAtMatrixAxisFix(&mtxRot, &XMLoadFloat3(&pos), &lookAt, &XMLoadFloat3(&up));		// 回転
	XMMATRIX mtxPos = XMMatrixTranslationFromVector(XMLoadFloat3(&pos));						// 座標
	XMFLOAT4X4 mtxWorld = XM::F4X4::WorldMatrixMultiply(&mtxScl, &mtxRot, &mtxPos);				// スケール、回転、移動の行列合成

	// ワールドマトリクスをシェーダーへ渡す準備
	// -------- -------- -------- --------
	CCamera::CBufferModel mtx;
	mtx.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&mtxWorld));	// ワールド変換行列
	mtx.mLight = cam->GetLightPos();
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &mtx, 0, 0);

	// レイアウトの設定
	// -------- -------- -------- --------
	UINT hStrides = sizeof(VERTEX_3D);
	UINT hOffsets = 0;
	pDeviceContext->IASetInputLayout(m_pVertexLayout);									// 入力レイアウトをコンテキストに設定
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &hStrides, &hOffsets);	// 頂点バッファをコンテキストに設定
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);			// インデックスバッファの設定
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// プリミティブ

	// テクスチャの取得
	ID3D11ShaderResourceView *pTex = CManager::GetTexManager()->GetTexInterface(m_strFilePass);

	// シェーダーの設定
	// -------- -------- -------- --------
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);				// 頂点シェーダーをコンテキストに設定
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);	// ワールド座標をシェーダーに渡す
	if (pTex)
	{
		// テクスチャ用ピクセルシェーダー 
		pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
		// テクスチャーをシェーダーに渡す
		pDeviceContext->PSSetSamplers(0, 1, &m_pTexSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &pTex);
	}
	else
	{
		// テクスチャ無し用ピクセルシェーダー 
		pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// プリミティブをレンダリング
	pDeviceContext->DrawIndexed(22, 0, 0);

	// 取り敢えず戻す
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// 頂点シェーダーをコンテキストに設定
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// ピクセルシェーダーをコンテキストに設定
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 作成処理
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneBox *CSceneBox::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &size, const std::string &strFilePass, int priority)
{
	// CSceneBoxの生成
	CSceneBox *pScene3D;
	pScene3D = new CSceneBox(priority);
	pScene3D->Init();

	pScene3D->SetPos(pos);
	pScene3D->SetRot(rot);
	pScene3D->SetSize(size);
	pScene3D->m_strFilePass = strFilePass;

	// 頂点, シェーダーの作成
	pScene3D->CreateVtxBuffer();

	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene3D;
}
