// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : メイン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#pragma warning(disable : 4099)
#include "main.h"
#include "manager.h"
#include "renderer.h"

#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要

// マクロ定義
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"フレームワーク"	// ウインドウのキャプション名


#ifdef _DEBUG
int g_nCountFPS = 0;	// FPSカウンタ
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//ウィンドウの矩形
	RECT dr;											//デスクトップの矩形

	AdjustWindowRect(&cr, WS_OVERLAPPEDWINDOW, FALSE);
	int ww = cr.right - cr.left;	//ウィンドウの幅
	int wh = cr.bottom - cr.top;	//ウィンドウの高さ

									//デスクトップの大きさを取得
	GetWindowRect(GetDesktopWindow(), &dr);
	int dw = dr.right - dr.left;	//デスクトップの幅
	int dh = dr.bottom - dr.top;	//デスクトップの高さ

	int wx = ww > dw ? 0 : (dw / 2) - (ww / 2);
	int wy = wh > dh ? 0 : (dh / 2) - (wh / 2);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		nullptr,
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		nullptr
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	HWND hWnd;
	hWnd = CreateWindowEx(0,	//拡張ウィンドウスタイル
		CLASS_NAME,				//ウィンドウクラスの名前
		WINDOW_NAME,			//ウィンドウの名前	ゲーム名を入れる
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		wx,						//ウィンドウの左上X
		wy,						//ウィンドウの左上Y
		ww,						//ウィンドウの幅
		wh,						//ウィンドウの高さ
		nullptr,					//親ウィンドウのハンドル
		nullptr,					//メニューハンドルまたは子ウィンドウのハンドル
		hInstance,				//インスタンスハンドル
		nullptr);					//ウィンドウ作成データ

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	//指定されたウィンドウの表示	引数:ウィンドウの表示方法を指定するフラグ	nCmdShowではなく自分で設定したほうが安全
	UpdateWindow(hWnd);			//ウィンドウの状態を更新する


	// マネージャーの生成
	CManager *pManager = nullptr;
	pManager = new CManager();

	// 初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE))) { return -1; }
	XMFLOAT2 screenSize = CManager::GetRenderer()->GetScreenSize();

	//各カウンタの初期化
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;		//現在の時間
	DWORD dwExecLastTime = timeGetTime();	//実行終了時の時間 timeGetTime():現在の時刻をミリ秒単位で取得 もっと精度よく計測したい場合はマルチメディアタイマー
	DWORD dwFPSLastTime = dwExecLastTime;	//dwExecLastTimeと同じ値を入れる

	//分解能の設定
	timeBeginPeriod(1);

	//メッセージループ（ゲーム版)
	for (;;)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			//Windowの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//PeekMessageにメッセージが来てなかったら
			dwCurrentTime = timeGetTime();	//現在のシステム時刻を取得
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
#endif
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1000/60秒以上たっていたら
				dwExecLastTime = dwCurrentTime;	//処理した時刻を更新

				// 更新処理
				pManager->Update(hWnd);

				// 描画処理
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}

	// オブジェクトの破棄
	if (pManager != nullptr)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);	//タイマ分解能を元に戻す

	return (int)msg.wParam;
}

// ウィンドウのコールバック関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
