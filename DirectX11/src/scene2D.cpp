// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3Dポリゴンのシーン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "manager.h"
#include "texManager.h"
#include "scene2D.h"
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "utility.h"

#include <assert.h>
#include <tchar.h>

ID3D11Buffer *CScene2D::m_pConstantBuffer2DCam = nullptr;

// -------- -------- -------- -------- -------- -------- -------- --------
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Init(void)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexSampler = nullptr;

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
// 頂点情報の作成
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::CreateVtxBuffer(void)
{
	// デバイスの取得
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// シェーダー読み込み
	BinFile vscode(L"data/SHADER/vs2D.cso");
	BinFile psTexcode(L"data/SHADER/ps2DTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// 頂点座標の設定
	m_pVtxData[0].pos = XMFLOAT3(-0.5f, -0.5f, 0.f);
	m_pVtxData[1].pos = XMFLOAT3(0.5f, -0.5f, 0.f);
	m_pVtxData[2].pos = XMFLOAT3(-0.5f, 0.5f, 0.f);
	m_pVtxData[3].pos = XMFLOAT3(0.5f, 0.5f, 0.f);
	// 頂点色の設定
	m_pVtxData[0].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[1].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[2].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_pVtxData[3].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	// UV座標の設定
	m_pVtxData[0].tex = XMFLOAT2(0.f, 0.f);
	m_pVtxData[1].tex = XMFLOAT2(1.f, 0.f);
	m_pVtxData[2].tex = XMFLOAT2(0.f, 1.f);
	m_pVtxData[3].tex = XMFLOAT2(1.f, 1.f);

	// 頂点バッファ作成
	// -------- -------- -------- -------- -------- -------- -------- --------
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX_2D) * NUM_VERTEX;		// バッファサイズ
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
		0, 1, 2, 3
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 4;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))) {
		MessageBox(nullptr, "インデックスバッファの作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// 入力レイアウト定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 4*3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 4*3+4*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	// コンスタントバッファの作成
	if (m_pConstantBuffer2DCam == nullptr)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(CCamera::ConstantBuffer);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = sizeof(float);
		if (FAILED(pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer2DCam)))
		{
			MessageBox(nullptr, "コンスタントバッファの作成失敗", "警告", S_OK);
		}
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Uninit(void)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexSampler);

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
void CScene2D::Update(void)
{
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE msr;
	/*
	HRESULT hr = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (FAILED(hr))
	{
		return;
	}

	void* buf = msr.pData;
	if (buf)
	{
		XMFLOAT3 pos = GetPos();
		XMFLOAT3 size = GetSize();
		XMFLOAT3 scl = GetScl();
		XMFLOAT3 rot = GetRot();
		float len = sqrt(size.x * size.x + size.y * size.y);
		float ang = atan2(size.y, size.x);
		m_pVtxData[0].pos = XMFLOAT3(cosf(ang + PI + rot.z) * len * scl.x + pos.x,
			sinf(ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
		m_pVtxData[1].pos = XMFLOAT3(cosf(-ang + rot.z) * len * scl.x + pos.x,
			sinf(-ang + rot.z) * len * scl.y + pos.y, 0.f);
		m_pVtxData[2].pos = XMFLOAT3(cosf(-ang + PI + rot.z) * len * scl.x + pos.x,
			sinf(-ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
		m_pVtxData[3].pos = XMFLOAT3(cosf(ang + rot.z) * len * scl.x + pos.x,
			sinf(ang + rot.z) * len * scl.y + pos.y, 0.f);

		// UV座標の設定
		m_pVtxData[0].tex = XMFLOAT2(m_posUV.x, m_posUV.y);
		m_pVtxData[1].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y);
		m_pVtxData[2].tex = XMFLOAT2(m_posUV.x, m_posUV.y + m_heightUV);
		m_pVtxData[3].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y + m_heightUV);

		// msr.pData = m_pVtxData;
		// memcpy(buf, m_pVtxData, sizeof(VERTEX_2D) * 4); // 4頂点分コピー
		pDeviceContext->Unmap(m_pVertexBuffer, 0);
	}
	*/

	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();
	float len = sqrt(size.x * size.x + size.y * size.y);
	float ang = atan2(size.y, size.x);
	m_pVtxData[0].pos = XMFLOAT3(cosf(ang + PI + rot.z) * len * scl.x + pos.x,
		sinf(ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[1].pos = XMFLOAT3(cosf(-ang + rot.z) * len * scl.x + pos.x,
		sinf(-ang + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[2].pos = XMFLOAT3(cosf(-ang + PI + rot.z) * len * scl.x + pos.x,
		sinf(-ang + PI + rot.z) * len * scl.y + pos.y, 0.f);
	m_pVtxData[3].pos = XMFLOAT3(cosf(ang + rot.z) * len * scl.x + pos.x,
		sinf(ang + rot.z) * len * scl.y + pos.y, 0.f);

	// UV座標の設定
	m_pVtxData[0].tex = XMFLOAT2(m_posUV.x, m_posUV.y);
	m_pVtxData[1].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y);
	m_pVtxData[2].tex = XMFLOAT2(m_posUV.x, m_posUV.y + m_heightUV);
	m_pVtxData[3].tex = XMFLOAT2(m_posUV.x + m_widthUV, m_posUV.y + m_heightUV);

	pDeviceContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_pVtxData, 0, 0);
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene2D::Draw(void)
{
	// 取得
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	CCamera *cam = CManager::GetCamera();

	// ワールドマトリクス作成
	// -------- -------- -------- --------
	XMFLOAT3 pos = GetPos();
	XMFLOAT3 size = GetSize();
	XMFLOAT3 scl = GetScl();
	XMFLOAT3 rot = GetRot();

	XMMATRIX mtxScl = XMMatrixScalingFromVector(XMLoadFloat3(&scl));				// 拡大率
	XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);			// オイラー角（ピッチ(rx)、ヨー(ry)、ロール(rz))から行列
	XMMATRIX mtxPos = XMMatrixTranslationFromVector(XMLoadFloat3(&pos));			// 座標
	XMFLOAT4X4 mtxWorld = XM::F4X4::WorldMatrixMultiply(&mtxScl, &mtxRot, &mtxPos);	// スケール、回転、移動の行列合成

	// ワールドマトリクスをシェーダーへ渡す準備
	// -------- -------- -------- --------
	CCamera::CBufferModel mtx;
	mtx.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&mtxWorld));// ワールド変換行列
	mtx.mLight = cam->GetLightPos();
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &mtx, 0, 0);

	// 変数の再設定
	// -------- -------- -------- --------
	SetPos(pos);
	SetSize(size);
	SetScl(scl);
	SetRot(rot);

	//透視射影変換行列
	CRenderer *pRenderer = CManager::GetRenderer();
	XMFLOAT2 screensize = pRenderer->GetScreenSize();
	XMMATRIX hProjection = XMMATRIX(
		2 / screensize.x, 0.0f, 0.0f, 0.0f,
		0.0f, -2 / screensize.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f);
	//
	// 変数の更新
	//
	CCamera::ConstantBuffer hConstantBuffer;
	hConstantBuffer.mView = XMMatrixTranspose(XMLoadFloat4x4(&cam->GetView()));
	hConstantBuffer.mProjection = XMMatrixTranspose(hProjection);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer2DCam, 0, nullptr, &hConstantBuffer, 0, 0);
	CCamera::CBufferModel hBufferScene;
	hBufferScene.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&cam->GetWorld()));
	hBufferScene.mLight = XMFLOAT4(0.f, 1.f, 0.f, 1.0f);
	pDeviceContext->UpdateSubresource(m_pConstantBufferModel, 0, nullptr, &hBufferScene, 0, 0);
	//コンテキストに設定
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer2DCam);


	// レイアウトの設定
	// -------- -------- -------- --------
	UINT hStrides = sizeof(VERTEX_2D);
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
		// テクスチャーをシェーダーに渡す
		pDeviceContext->PSSetSamplers(0, 1, &m_pTexSampler);
		pDeviceContext->PSSetShaderResources(0, 1, &pTex);
		// テクスチャ用ピクセルシェーダー 
		pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
	}
	else
	{
		// テクスチャ無し用ピクセルシェーダー
		pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// プリミティブをレンダリング
	pDeviceContext->DrawIndexed(4, 0, 0);

	// 取り敢えず戻す
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// 頂点シェーダーをコンテキストに設定
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// ピクセルシェーダーをコンテキストに設定
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 作成処理
// -------- -------- -------- -------- -------- -------- -------- --------
CScene2D *CScene2D::Create(const XMFLOAT2 &pos, const float &rot, const XMFLOAT2 &scl, const std::string &strFilePass, int priority)
{
	// CScene2Dの生成
	CScene2D *pScene2D;
	pScene2D = new CScene2D(priority);
	pScene2D->Init();

	pScene2D->SetPos(XMFLOAT3(pos.x, pos.y, 0.f));
	pScene2D->SetRot(XMFLOAT3(0.f, 0.f, rot));
	pScene2D->SetSize(XMFLOAT3(scl.x, scl.y, 1.f));
	pScene2D->SetScl(XMFLOAT3(1.f, 1.f, 1.f));
	pScene2D->m_strFilePass = strFilePass;

	// 頂点, シェーダーの作成
	pScene2D->CreateVtxBuffer();
	
	// テクスチャのセット
	CManager::GetTexManager()->SetTex(strFilePass);

	return pScene2D;
}
