//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifdef _DEBUG		// Debug時のみ実行

//================================================================================
// インクルード
//================================================================================
#include "main.h"
#include <assert.h>
#include "debugStr.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
// 静的メンバ変数
//--------------------------------------------------------------------------------
std::unordered_map<std::string, CDebugStr*> CDebugStr::m_umapDebugStr;

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CDebugStr::CDebugStr()
{

}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CDebugStr::~CDebugStr()
{

}

//--------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------
void CDebugStr::Init(int line, std::string name, std::vector<float> vecVal)
{
	assert(!vecVal.empty() && "値が空です");

	// 値の代入
	m_nLine = line;
	m_nNumValue = vecVal.size();
	m_aName = name;
	for (int i = 0; i < m_nNumValue; i++)
	{
		m_vecValue.push_back(vecVal[i]);
	}

	// 文字情報の設定
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXCreateFont(pDevice,		// デバイス
		18,						// 文字の高さ
		0,						// 文字の幅(0にすると高さに合わせた比率になる)
		0,						// フォントの太さ(0だとデフォルト)
		0,						// ミップマップ（重くなる)　デバッグなので汚くておっけー
		false,					// イタリック体
		SHIFTJIS_CHARSET,		// フォントの文字セット
		OUT_DEFAULT_PRECIS,		// フォントが合致しなかった時の指定
		DEFAULT_QUALITY,		// 選ぶフォントのクオリティ
		DEFAULT_PITCH,			// ピッチとファミリー
		"Terminal",
		&m_pDebugFont);
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CDebugStr::Update()
{
	std::string temp;
	for (int nCntLine = 0; nCntLine < DEBUG_DISPLAY_LINE; nCntLine++)
	{
		if (m_nLine == nCntLine)
		{
			// [表示名 + : ]
			temp += (m_aName + ": ");
			std::string strBuff;

			// [値 + , 値 + , 値 + ...]
			for(int i = 0; i < m_nNumValue; i++)
			{
				strBuff += std::to_string(m_vecValue.at(i));

				if (i != m_nNumValue - 1)strBuff += ", ";
			}
			temp += strBuff;
		}
		// 改行文字を加えて改行
		if (nCntLine >= 1)temp += "\n";
	}
	m_strDebug = temp;
}

//--------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------
void CDebugStr::Draw()
{
//	RECT rect = { 0, DEBUG_DISPLAY_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT };	// デバッグ情報表示領域
//	m_pDebugFont->DrawText(
//		nullptr,							// デバイスコンテキスト nullptrででる
//		&m_strDebug[0],						// 文字列
//		-1,
//		&rect,								// 書く範囲
//		DT_LEFT,							// 整形方法 DT_LEFT:左詰
//		D3DCOLOR_RGBA(255, 255, 0, 255)		// 文字色
//	);
}

//--------------------------------------------------------------------------------
// 全更新
//--------------------------------------------------------------------------------
void CDebugStr::UpdateAll()	
{
	std::for_each(m_umapDebugStr.begin(), m_umapDebugStr.end(), [](std::pair<const std::string, CDebugStr*> &umapDebugStr) {umapDebugStr.second->Update(); });
}

//--------------------------------------------------------------------------------
// 全描画
//--------------------------------------------------------------------------------
void CDebugStr::DrawAll()
{
	std::for_each(m_umapDebugStr.begin(), m_umapDebugStr.end(), [](std::pair<const std::string, CDebugStr*> &umapDebugStr) {umapDebugStr.second->Draw(); });
}

//--------------------------------------------------------------------------------
// 全解放
//--------------------------------------------------------------------------------
void CDebugStr::ReleaseAll()
{
	// コンテナを末尾までループ
	for (auto it = m_umapDebugStr.begin(); it != m_umapDebugStr.end();)
	{
		// 要素(second)の表示用の値のクリア
		m_umapDebugStr[it->first]->m_vecValue.clear();

		// 要素(second)のフォントの解放
		if (m_umapDebugStr[it->first]->m_pDebugFont != nullptr)
		{
			m_umapDebugStr[it->first]->m_pDebugFont->Release();
			m_umapDebugStr[it->first]->m_pDebugFont = nullptr;
		}
		// 要素(second)の解放
		if (m_umapDebugStr[it->first] != nullptr)
		{
			delete m_umapDebugStr[it->first];
			m_umapDebugStr[it->first] = nullptr;
		}
		m_umapDebugStr.erase(it++);		// ファイルパス削除
	}
	// 自分自身を空にする(全要素クリア)
	m_umapDebugStr.clear();
}

//--------------------------------------------------------------------------------
// デバッグ文字設定
//--------------------------------------------------------------------------------
void CDebugStr::Set(std::string name, std::vector<float> vecVal)
{
	// 要素の有無を確かめる
	if (m_umapDebugStr.count(name) == 1)
	{// ありますよ
		assert(!vecVal.empty() && "値が空です");

		for (int i = 0; i < m_umapDebugStr[name]->m_nNumValue; i++)
		{
			m_umapDebugStr[name]->m_vecValue[i] = vecVal[i];
		}
	}
}

//--------------------------------------------------------------------------------
// デバッグ文字生成
//--------------------------------------------------------------------------------
void CDebugStr::Create(int line, std::string name, std::vector<float> vecVal)
{
	// インスタンスの生成
	CDebugStr *debugStr = new CDebugStr();

	// 要素の追加
	m_umapDebugStr[name] = debugStr;

	// 初期化
	debugStr->Init(line, name, vecVal);
}

#endif

