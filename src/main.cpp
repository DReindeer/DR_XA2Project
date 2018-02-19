//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene.h"

//--------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�t���[�����[�N"	// �E�C���h�E�̃L���v�V������

//--------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadFont(HINSTANCE hInstance);

#ifdef _DEBUG
int g_nCountFPS = 0;	// FPS�J�E���^
#endif

//================================================================================
// ���C���֐�
//================================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//�E�B���h�E�̋�`
	RECT dr;											//�f�X�N�g�b�v�̋�`

	AdjustWindowRect(&cr, WS_OVERLAPPEDWINDOW, FALSE);
	int ww = cr.right - cr.left;	//�E�B���h�E�̕�
	int wh = cr.bottom - cr.top;	//�E�B���h�E�̍���

									//�f�X�N�g�b�v�̑傫�����擾
	GetWindowRect(GetDesktopWindow(), &dr);
	int dw = dr.right - dr.left;	//�f�X�N�g�b�v�̕�
	int dh = dr.bottom - dr.top;	//�f�X�N�g�b�v�̍���

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
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	HWND hWnd;
	hWnd = CreateWindowEx(0,	// �g���E�B���h�E�X�^�C��
		CLASS_NAME,				// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,			// �E�B���h�E�̖��O	�Q�[����������
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
		wx,						// �E�B���h�E�̍���X
		wy,						// �E�B���h�E�̍���Y
		ww,						// �E�B���h�E�̕�
		wh,						// �E�B���h�E�̍���
		nullptr,				// �e�E�B���h�E�̃n���h��
		nullptr,				// ���j���[�n���h���܂��͎q�E�B���h�E�̃n���h��
		hInstance,				// �C���X�^���X�n���h��
		nullptr);				// �E�B���h�E�쐬�f�[�^

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);	// �w�肳�ꂽ�E�B���h�E�̕\��	����:�E�B���h�E�̕\�����@���w�肷��t���O	nCmdShow�ł͂Ȃ������Őݒ肵���ق������S
	UpdateWindow(hWnd);			// �E�B���h�E�̏�Ԃ��X�V����

	// �}�l�[�W���[�̐���
	CManager *pManager = nullptr;
	pManager = new CManager();

	// ����������
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE))) 
	{
		return -1;
	}

	D3DXVECTOR2 screenSize = pManager->GetRenderer()->GetScreenSize();
	// �������[�h�̐ݒ�
	CManager::SetMode(new CModeTitle);

//	LoadFont(hInstance);

	//�e�J�E���^�̏�����
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;				//���݂̎���
	DWORD dwExecLastTime = timeGetTime();	//���s�I�����̎��� timeGetTime():���݂̎������~���b�P�ʂŎ擾 �����Ɛ��x�悭�v���������ꍇ�̓}���`���f�B�A�^�C�}�[
#ifdef _DEBUG
	DWORD dwFPSLastTime = dwExecLastTime;	//dwExecLastTime�Ɠ����l������
#endif

	//����\�̐ݒ�
	timeBeginPeriod(1);

	MSG msg;
	//msg.hwnd = hWnd;

	//���b�Z�[�W���[�v�i�Q�[����)
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			//Window�̏���
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
		{
			//PeekMessage�Ƀ��b�Z�[�W�����ĂȂ�������
			dwCurrentTime = timeGetTime();	//���݂̃V�X�e���������擾
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
#endif
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1000/60�b�ȏソ���Ă�����
				dwExecLastTime = dwCurrentTime;	//���������������X�V

				// �X�V����
				pManager->Update(hWnd);

				// �`�揈��
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}

	// �I�u�W�F�N�g�̔j��
	if(pManager != nullptr)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);	//�^�C�}����\�����ɖ߂�

	return (int)msg.wParam;
}

//================================================================================
// �v���V�[�W��
//================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LPDIRECT3DDEVICE9 dev = CManager::GetRenderer()->GetDevice();

	switch(uMsg)
	{
	case WM_CREATE:
		break;
	
		// -------- �}�E�X�J�[�\��
	case WM_SETCURSOR:
		//SetCursor(nullptr);
		//dev->ShowCursor(TRUE);
		//return TRUE;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			if (CManager::GetMode()->GetModeType() == CMode::MODETYPE::LOADING) { break; }
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// -------- �t�H���g�̓ǂݍ���
void LoadFont(HINSTANCE hInstance)
{
	HRSRC resource = FindResource(hInstance, "PixelMplus12-Regular", RT_RCDATA);
	LPVOID font = LockResource(resource);
	DWORD size = SizeofResource(nullptr, resource);
	DWORD fontCount = 0;
	AddFontMemResourceEx(font, size, nullptr, &fontCount);
}

#ifdef _DEBUG
//================================================================================
// FPS�ݒ�
//================================================================================
int GetFPS()
{
	return g_nCountFPS;
}
#endif