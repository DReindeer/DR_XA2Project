// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 2Dポリゴンのシーン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include <string>
#include "scene.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 構造体宣言
// -------- -------- -------- -------- -------- -------- -------- --------
typedef struct
{
	XMFLOAT3 pos;		// 頂点座標
	XMFLOAT4 col;		// 頂点色
	XMFLOAT2 tex;		// テクスチャ座標
}VERTEX_2D;

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene2D : public CScene
{
public:
	CScene2D(int priority = 4) : CScene(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DPOLY; }
	~CScene2D() {}

	void Init(void);				// 初期化
	void CreateVtxBuffer(void);
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CScene2D *Create(const XMFLOAT2 &pos, const float &rot, const XMFLOAT2 &scl, const std::string &strFilePass, int priority = 4);

private:
	ID3D11Buffer *m_pVertexBuffer;			// 頂点バッファ
	ID3D11Buffer *m_pIndexBuffer;			// インデックスバッファ
	VERTEX_2D m_pVtxData[NUM_VERTEX];		// 頂点データ

protected:
	XMFLOAT2 m_posUV;						// テクスチャ座標
	float m_widthUV;						// 幅
	float m_heightUV;						// 高さ
	std::string m_strFilePass;				// テクスチャFilePass

	// 定数バッファ
	ID3D11Buffer *m_pConstantBufferModel;
	ID3D11Buffer *m_pConstantBufferMat;		// マテリアル

	// シェーダー
	ID3D11VertexShader *m_pVertexShader;	// 頂点
	ID3D11PixelShader *m_pPixelShader;		// ピクセル
	ID3D11PixelShader *m_pPixelTexShader;	// ピクセル
	ID3D11InputLayout *m_pVertexLayout;		// レイアウト

	ID3D11SamplerState *m_pTexSampler;		// サンプラー
	static ID3D11Buffer *m_pConstantBuffer2DCam;
};

#endif
