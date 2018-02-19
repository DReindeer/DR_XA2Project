//--------------------------------------------------------------------------------
//
// シーン2D処理 [number.h]
// Author : KENSUKE WATANABE
//
//--------------------------------------------------------------------------------
#ifndef _CNUMBER_H_
#define _CNUMBER_H_

#include "main.h"

//--------------------------------------------------------------------------------
// クラス定義
//--------------------------------------------------------------------------------
class CNumber
{
public:
	CNumber();		// コンストラクタ
	~CNumber();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size);	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	static CNumber *Create(D3DXVECTOR3 pos, float rot, D3DXVECTOR3 size);	// 生成
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const int disit);	// 生成
	
	void SetNumber(int nNumber);	// 描画したい番号の設定(テクスチャ座標の設定)

	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;		// 頂点バッファへのポインタ
	
	D3DXVECTOR3 m_pos;		// ポリゴンの位置
	D3DXVECTOR3 m_size;		// 大きさ
	float m_rot;			// 角度
	int m_r, m_g, m_b, m_a;	// 色
};

#endif
