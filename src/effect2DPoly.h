//--------------------------------------------------------------------------------
//
//	title : VSsheep[ sceneEffect.h ]		Auter : KENSUKE WATANABE
//											Data  : 2016/08/30
//
//--------------------------------------------------------------------------------
#ifndef _EFFECT2DPOLY_H_
#define _EFFECT2DPOLY_H_

#include <vector>
#include "scene2D.h"

static const char* TEXNAME_EFFECT_WTOBLINE = "data/TEXTURE/BG/WtoBline.png";

class CEffect2DPoly : public CScene2D
{
public:
	CEffect2DPoly(int priority = 4) : CScene2D(priority) {}
	~CEffect2DPoly() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	static CEffect2DPoly *Create(int n, std::string texName);

	void SetEffect2DPoly(D3DXVECTOR2 pos, float rot, D3DXVECTOR2 move, float radius, int &r, int &g, int &b, int &a, int life, std::string texName);

	static std::vector<CEffect2DPoly*> *GetVecEffect2DPoly() { return &m_pvecEffect2DPoly; }

private:
	HRESULT MakeVertex();					// �|���S�����_�쐬
	void SetVertex(VERTEX_2D* pVtx);		// ���_���ݒ�

	float m_fRadius;			// ���a
	float m_fRadiusValue;		// ���a�ω���
	int m_colorAlphaValue;		// ���l�̕ω���
	bool m_bUse;				// �g���Ă��邩
	int m_nLife;				// ����

	D3DXVECTOR2 m_move;			// �ړ���

	static int m_maxEffect;
	static int m_cnt;

	static std::vector<CEffect2DPoly*> m_pvecEffect2DPoly;
};

#endif