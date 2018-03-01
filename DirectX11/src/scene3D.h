// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3Dのシーンの親クラス
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include <string>
#include "scene.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 定数定義
// -------- -------- -------- -------- -------- -------- -------- --------
// 頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット	DIFFUSE:色 TEX1:テクスチャ座標(UVやST)

// -------- -------- -------- -------- -------- -------- -------- --------
// 構造体宣言
// -------- -------- -------- -------- -------- -------- -------- --------
typedef struct
{
	XMFLOAT3 pos;		// 頂点座標
	XMFLOAT3 nom;		// 法線座標
	XMFLOAT4 col;		// 頂点色
	XMFLOAT2 tex;		// テクスチャ座標
}VERTEX_3D;

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene3D : public CScene
{
public:
	CScene3D(int priority = 4) : CScene(priority) { m_classType = CLASSTYPE::SCENE_3D; }
	~CScene3D() {}

	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

	void SetStrFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }
	const std::string &GetStrFilePass(void) { return m_strFilePass; }
	void SetTexturePos(XMFLOAT2 uv, XMFLOAT2 length);

protected:
	XMFLOAT2 m_posUV;				// テクスチャ座標
	float m_widthUV;				// 幅
	float m_heightUV;				// 高さ
	std::string m_strFilePass;		// テクスチャFilePass

	// 定数バッファ
	ID3D11Buffer *m_pConstantBufferModel;
	ID3D11Buffer *m_pConstantBufferMat;		// マテリアル

	// シェーダー
	ID3D11VertexShader *m_pVertexShader;	// 頂点
	ID3D11PixelShader *m_pPixelShader;		// ピクセル
	ID3D11PixelShader *m_pPixelTexShader;	// ピクセル
	ID3D11InputLayout *m_pVertexLayout;		// レイアウト

	ID3D11SamplerState *m_pTexSampler;		// サンプラー
};

#endif
