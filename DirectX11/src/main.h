// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ���C��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _MAIN_H_
#define _MAIN_H_

// -------- -------- -------- -------- -------- -------- -------- --------
// �C���N���[�h�t�@�C��
// -------- -------- -------- -------- -------- -------- -------- --------
#include<Windows.h>

#pragma warning(push)
#pragma warning(disable : 4005)

#include <d3d11.h>
#include <d3d11shader.h>
#pragma warning(pop)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")
#pragma comment(lib,"dxguid.lib")

// -------- -------- -------- -------- -------- -------- -------- --------
// �萔��`
// -------- -------- -------- -------- -------- -------- -------- --------
#define SCREEN_WIDTH	( 800 )		// ��ʂ̉���
#define SCREEN_HEIGHT	( 600 )		// ��ʂ̏c��

#endif
