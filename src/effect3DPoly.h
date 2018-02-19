//================================================================================
//
//	title : VSsheep[ sceneEffect.h ]		Auter : KENSUKE WATANABE
//											Data  : 2016/08/30
//
//--------------------------------------------------------------------------------
#ifndef _EFFECT3DPOLY_H_
#define _EFFECT3DPOLY_H_

#include <vector>
#include "scene3D.h"

static const char* TEXNAME_EFFECT_WTOBLINE = "data/TEXTURE/BG/WtoBline.png";

class CEffect3DPoly : public CScene3D
{
public:
	CEffect3DPoly(int priority = 4): CScene3D(priority){}
	~CEffect3DPoly() {}

	void Init();
	void Uninit();
	void Update();
//	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用
	static CEffect3DPoly *Create(int n, std::string texName);

	static void SetEffect3DPoly(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float radius, float r, float g, float b, float a, float life, std::string texName);
	static void FlashAll();

	static std::vector<CEffect3DPoly*> *GetVecEffect3DPoly() { return &m_pvecEffect3DPoly; }

private:
	HRESULT MakeVertex();		// ポリゴン頂点作成
	void SetVertex();			// 頂点情報設定

	float m_fRadius;			// 半径
	float m_fRadiusValue;		// 半径変化量
	float m_fColorAlphaValue;	// α値の変化量
	bool m_bUse;				// 使われているか
	float m_nLife;				// 寿命

	D3DXVECTOR3 m_move;			// 移動量

	static int m_maxEffect;
	static int m_cnt;

	static std::vector<CEffect3DPoly*> m_pvecEffect3DPoly;
};

#endif
