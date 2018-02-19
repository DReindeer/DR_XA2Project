//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifdef _DEBUG		// Debug時のみ実行
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------
#define DEBUG_DISPLAY_MAX			(100)	//デバッグ用表示登録可能最大数
#define DEBUG_DISPLAY_CHAR_MAX		(20)	//デバッグ用表示変数名最大文字数
#define DEBUG_DISPLAY_LINE			(20)	//デバッグ用表示列数
#define DEBUG_DISPLAY_VARIABLE_MAX	(3)		//デバッグ用表示最大変数数
#define DEBUG_MAX					(512)	//デバッグ表示文字数

#define DEBUG_DISPLAY_POS_Y			(20)	//デバッグ表示位置(Y座標)

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CDebugStr
{
public:
	CDebugStr();
	~CDebugStr();

	void Init(int line, std::string name, std::vector<float> vecVal);
	void Update();
	void Draw();
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();

	static void Create(int line, std::string name, std::vector<float> vecVal);
	static void Set(std::string name, std::vector<float> vecVal);

private:
	int m_nLine;					// 表示行
	int m_nNumValue;				// 表示変数数
	std::vector<float> m_vecValue;	// 表示値
	std::string m_aName;			// 表示名

	LPD3DXFONT m_pDebugFont;		// フォントへのインターフェースポインタ
	std::string m_strDebug;			// デバッグ表示文字列

	static std::unordered_map<std::string, CDebugStr*> m_umapDebugStr;
};

#endif
#endif