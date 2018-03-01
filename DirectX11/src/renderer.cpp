// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : レンダラー
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include <tchar.h>
#include "renderer.h"
#include "utility.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// 静的メンバ変数
// -------- -------- -------- -------- -------- -------- -------- --------
ID3D11DeviceContext *CRenderer::m_pDeviceContext = nullptr;
XMFLOAT2 CRenderer::m_screenSize = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);

// ======== ======== ======== ======== ======== ======== ======== ========
// CRenderer::コンストラクタ
// -------- -------- -------- -------- -------- -------- -------- --------
CRenderer::CRenderer()
{
	// 値のクリア
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CEnemy::デストラクタ
// -------- -------- -------- -------- -------- -------- -------- --------
CRenderer::~CRenderer()
{

}

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化処理
// ======== ======== ======== ======== ======== ======== ======== ========
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext)))
	{
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		if (FAILED(D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE_SOFTWARE,
			nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext)))
		{
			// 生成失敗
			MessageBox(hWnd, _T("D3D11CreateDevice"), _T("Err"), MB_ICONSTOP);
			return E_FAIL;
		}
	}

	//インターフェース取得
	m_pDXGI = nullptr;
	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&m_pDXGI))) {
		MessageBox(hWnd, _T("QueryInterface"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//アダプター取得
	m_pAdapter = nullptr;
	if (FAILED(m_pDXGI->GetAdapter(&m_pAdapter))) {
		MessageBox(hWnd, _T("GetAdapter"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//ファクトリー取得
	m_pDXGIFactory = nullptr;
	m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory);
	if (m_pDXGIFactory == nullptr) {
		MessageBox(hWnd, _T("GetParent"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//ALT+Enterでフルスクリーンを許可する
	if (FAILED(m_pDXGIFactory->MakeWindowAssociation(hWnd, 0))) {
		MessageBox(hWnd, _T("MakeWindowAssociation"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//スワップチェイン作成
	m_pDXGISwpChain = nullptr;
	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = (UINT)m_screenSize.x;
	hDXGISwapChainDesc.BufferDesc.Height = (UINT)m_screenSize.y;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = hWnd;
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(m_pDXGIFactory->CreateSwapChain(m_pDevice, &hDXGISwapChainDesc, &m_pDXGISwpChain))) {
		MessageBox(hWnd, _T("CreateSwapChain"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//ステンシル用テクスチャー作成
	ID3D11Texture2D* hpTexture2dDepth = nullptr;
	D3D11_TEXTURE2D_DESC hTexture2dDesc;
	hTexture2dDesc.Width = hDXGISwapChainDesc.BufferDesc.Width;
	hTexture2dDesc.Height = hDXGISwapChainDesc.BufferDesc.Height;
	hTexture2dDesc.MipLevels = 1;
	hTexture2dDesc.ArraySize = 1;
	hTexture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	hTexture2dDesc.SampleDesc = hDXGISwapChainDesc.SampleDesc;
	hTexture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	hTexture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hTexture2dDesc.CPUAccessFlags = 0;
	hTexture2dDesc.MiscFlags = 0;
	if (FAILED(m_pDevice->CreateTexture2D(&hTexture2dDesc, nullptr, &hpTexture2dDepth))) {
		MessageBoxW(hWnd, L"CreateTexture2D", L"Err", MB_ICONSTOP);
		return E_FAIL;
	}

	//ステンシルターゲット作成
	m_pDepthStencilView = nullptr;
	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = hTexture2dDesc.Format;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hDepthStencilViewDesc.Flags = 0;
	if (FAILED(m_pDevice->CreateDepthStencilView(hpTexture2dDepth, &hDepthStencilViewDesc, &m_pDepthStencilView))) {
		MessageBoxW(hWnd, L"CreateDepthStencilView", L"Err", MB_ICONSTOP);
		return E_FAIL;
	}

	//そのスワップチェインのバックバッファ取得
	m_pBackBuffer = nullptr;
	if (FAILED(m_pDXGISwpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))) {
		MessageBox(hWnd, _T("SwpChain GetBuffer"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//そのバックバッファから描画ターゲット生成
	m_pRenderTargetView = nullptr;
	if (FAILED(m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &m_pRenderTargetView))) {
		MessageBox(hWnd, _T("CreateRenderTargetView"), _T("Err"), MB_ICONSTOP);
		return E_FAIL;
	}

	//更にその描画ターゲットをコンテキストに設定
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	// ビューポート設定
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_vp.Width = m_screenSize.x;
	m_vp.Height = m_screenSize.y;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;

	return S_OK;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了処理
// ======== ======== ======== ======== ======== ======== ======== ========
void CRenderer::Uninit(void)
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pDXGISwpChain);
	SAFE_RELEASE(m_pDXGIFactory);
	SAFE_RELEASE(m_pAdapter);
	SAFE_RELEASE(m_pDXGI);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 描画の先頭に行う処理 
// ======== ======== ======== ======== ======== ======== ======== ========
bool CRenderer::DrawBigin(void)
{
	//ビューポート設定
	m_pDeviceContext->RSSetViewports(1, &m_vp);

	// バックバッファのクリア
	float ClearColor[] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return true;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 描画の後に行う処理 
// ======== ======== ======== ======== ======== ======== ======== ========
void CRenderer::DrawEnd(void)
{
	// バックバッファとフロントバッファの入れ替え
	m_pDXGISwpChain->Present(0, 0);
}
