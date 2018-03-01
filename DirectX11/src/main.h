// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : メイン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _MAIN_H_
#define _MAIN_H_

// -------- -------- -------- -------- -------- -------- -------- --------
// インクルードファイル
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
// 定数定義
// -------- -------- -------- -------- -------- -------- -------- --------
#define SCREEN_WIDTH	( 800 )		// 画面の横幅
#define SCREEN_HEIGHT	( 600 )		// 画面の縦幅

#endif
