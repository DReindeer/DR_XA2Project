//================================================================================
//
//	title : ポリゴン表示プログラム[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include <string>
#include "scene.h"

//--------------------------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(int priority = 4) :CScene(priority) { m_classType = CLASSTYPE::SCENE_2D; }
	~CScene2D();

	void Init();				// 初期化
	void Uninit();				// 終了
	void Update();				// 更新
	void Draw();				// 描画
	static CScene2D *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass, int priority = 4, CTexManager *pUseTexManager = nullptr);

	void SetStrFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }
	const std::string &GetStrFilePass() { return m_strFilePass; }
	void SetTexturePos(D3DXVECTOR2 uv, D3DXVECTOR2 length);	

	void ScreenResize();

protected:
	void SetUseTexManager(CTexManager *pUseTexManager) { m_pUseTexManager = pUseTexManager; }

	D3DXVECTOR2 m_posUV;		// テクスチャ座標
	float m_widthUV;			// 幅
	float m_heightUV;			// 高さ
	std::string m_strFilePass;	// テクスチャFilePass

private:
	CTexManager *m_pUseTexManager;	// 使用するテクスチャマネージャ
};

#endif
