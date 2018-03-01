// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : Xモデルのシーン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
#include "XModelLoader.h"
#include "sceneXModel.h"
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
void CSceneXModel::Init(void)
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
void CSceneXModel::CreateVtxBuffer(void)
{
	// デバイスの取得
	// -------- -------- -------- -------- -------- -------- -------- --------
	ID3D11DeviceContext* pDeviceContext = CRenderer::GetDeviceContext();
	ID3D11Device* pDevice = CManager::GetRenderer()->GetDevice();

	// シェーダー読み込み
	BinFile vscode(L"data/SHADER/vs3D.cso");
	BinFile psTexcode(L"data/SHADER/psTex.cso");
	BinFile pscode(L"data/SHADER/ps.cso");

	// 入力レイアウト定義
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
	if (FAILED(pDevice->CreatePixelShader(pscode.get(), pscode.size(), nullptr, &m_pPixelShader))) {
		MessageBox(nullptr, "ピクセルシェーダー作成失敗", "Err", MB_ICONSTOP);
		return;
	}
	if (FAILED(pDevice->CreatePixelShader(psTexcode.get(), psTexcode.size(), nullptr, &m_pPixelTexShader))) {
		MessageBox(nullptr, "ピクセルシェーダー(テクスチャ用)作成失敗", "Err", MB_ICONSTOP);
		return;
	}

	// メッシュ分割されたモデルの取得
	// -------- -------- -------- -------- -------- -------- -------- --------
	std::vector<XModel::MESH*> *vecpModel = CManager::GetXModelManager()->GetModel(m_strFilePass);
	m_vecModelBuffer.resize(vecpModel->size());
	int cntMesh = 0;
	for (auto pModel : *vecpModel)
	{
		// メッシュの頂点データ
		m_vecModelBuffer.at(cntMesh).vecVtxData.resize(pModel->face.size() * 3);
		int cnt = 0;
		for (int i = 0; i < static_cast<int>(pModel->face.size()); i++)
		{
			for (int idx = 0; idx < 3; idx++)
			{
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].pos = pModel->face.at(i).vtx[idx].pos;
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].nom = pModel->face.at(i).vtx[idx].nom;
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].col = XM::F4::Mul(pModel->face.at(i).vtx[idx].col, pModel->mat.matCol);
				m_vecModelBuffer.at(cntMesh).vecVtxData[cnt].tex = pModel->face.at(i).vtx[idx].tex;
				cnt++;
			}
		}

		// 頂点バッファ作成
		// -------- -------- -------- -------- -------- -------- -------- --------
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(VERTEX_3D) * static_cast<int>(m_vecModelBuffer.at(cntMesh).vecVtxData.size());		// バッファサイズ
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// 頂点バッファーとしてバインド
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_vecModelBuffer.at(cntMesh).vecVtxData[0];						// リソースデータのポインタ
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_vecModelBuffer.at(cntMesh).pVertexBuffer)))
		{
			MessageBox(nullptr, "頂点バッファ作成失敗", "Err", MB_ICONSTOP);
			return;
		}

		// インデックスバッファの作成
		// -------- -------- -------- -------- -------- -------- -------- --------
		std::vector<WORD> m_vecIdxData;
		for (int i = 0; i < static_cast<int>(pModel->face.size()); i++)
		{
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 0));
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 1));
			m_vecIdxData.push_back(static_cast<WORD>(i * 3 + 2));
		}
		m_vecModelBuffer.at(cntMesh).numIdx = static_cast<int>(m_vecIdxData.size());

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_vecModelBuffer.at(cntMesh).numIdx;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = &m_vecIdxData[0];
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_vecModelBuffer.at(cntMesh).pIndexBuffer))) {
			MessageBox(nullptr, "インデックスバッファの作成失敗", "Err", MB_ICONSTOP);
			return;
		}

		//テクスチャー用サンプラー作成
		D3D11_SAMPLER_DESC SamDesc;
		ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

		SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pDevice->CreateSamplerState(&SamDesc, &m_vecModelBuffer.at(cntMesh).pTexSampler);

		// 定数バッファ作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CCamera::CBufferModel);
		bd.CPUAccessFlags = 0;
		if (FAILED(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBufferModel))) {
			MessageBox(nullptr, "定数バッファ作成失敗", "Err", MB_ICONSTOP);
			return;
		}

		cntMesh++;
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Uninit(void)
{
	for (auto modelBuffer : m_vecModelBuffer)
	{
		SAFE_RELEASE(modelBuffer.pVertexBuffer);
		SAFE_RELEASE(modelBuffer.pIndexBuffer);
		SAFE_RELEASE(modelBuffer.pTexSampler);
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// 描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModel::Draw(void)
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

	// メッシュ分割されたモデルの取得
	std::vector<XModel::MESH*> *vecpModel = CManager::GetXModelManager()->GetModel(m_strFilePass);

	// メッシュ毎に描画
	// -------- -------- -------- -------- -------- -------- -------- --------	
	int cntMesh = 0;
	for (auto pModel : *vecpModel)
	{
		// プリミティブ
		// -------- -------- -------- -------- -------- -------- -------- --------	
		UINT hStrides = sizeof(VERTEX_3D);
		UINT hOffsets = 0;
		pDeviceContext->IASetInputLayout(m_pVertexLayout);																// 入力レイアウトをコンテキストに設定
		pDeviceContext->IASetVertexBuffers(0, 1, &m_vecModelBuffer.at(cntMesh).pVertexBuffer, &hStrides, &hOffsets);	// 頂点バッファをコンテキストに設定
		pDeviceContext->IASetIndexBuffer(m_vecModelBuffer.at(cntMesh).pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);			// インデックスバッファの設定
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);									// プリミティブ

		// テクスチャの取得
		ID3D11ShaderResourceView *pTex = CManager::GetTexManager()->GetTexInterface(pModel->mat.filePass);

		// シェーダーの設定
		// -------- -------- -------- -------- -------- -------- -------- --------
		// 頂点シェーダーをコンテキストに設定
		pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
		// ワールド座標をシェーダーに渡す
		pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferModel);

		// テクスチャの有無によってピクセルシェーダーを分ける
		// -------- -------- -------- -------- -------- -------- -------- --------	
		if (pTex)
		{
			// テクスチャ用ピクセルシェーダー 
			pDeviceContext->PSSetShader(m_pPixelTexShader, nullptr, 0);
			// テクスチャーをシェーダーに渡す
			pDeviceContext->PSSetSamplers(0, 1, &m_vecModelBuffer.at(cntMesh).pTexSampler);
			pDeviceContext->PSSetShaderResources(0, 1, &pTex);;
		}
		else
		{
			// テクスチャ無し用ピクセルシェーダー 
			pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
		}

		// プリミティブをレンダリング
		pDeviceContext->DrawIndexed(m_vecModelBuffer.at(cntMesh).numIdx, 0, 0);

		// 取り敢えず戻す
		pDeviceContext->VSSetShader(nullptr, nullptr, 0);	// 頂点シェーダーをコンテキストに設定
		pDeviceContext->PSSetShader(nullptr, nullptr, 0);	// ピクセルシェーダーをコンテキストに設定

		cntMesh++;
	}
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 作成処理
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneXModel *CSceneXModel::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority)
{
	// CSceneXModelの生成
	CSceneXModel *pScene3D;
	pScene3D = new CSceneXModel(priority);
	pScene3D->Init();

	pScene3D->SetPos(pos);
	pScene3D->SetRot(rot);
	pScene3D->SetScl(scl);
	pScene3D->m_strFilePass = strFilePass;

	// モデルの作成
	CManager::GetXModelManager()->SetModel(strFilePass);

	// 頂点, シェーダーの作成
	pScene3D->CreateVtxBuffer();

	return pScene3D;
}
