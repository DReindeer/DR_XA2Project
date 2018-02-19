//================================================================================
//
//	title : ポリゴン表示プログラム[ CSceneModel.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include "main.h"
#include <string>
#include "scene3D.h"

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CSceneModel : public CScene3D
{
public:
	CSceneModel(int priority = 4) :CScene3D(priority), m_pParent(nullptr), m_pUseModelManager(nullptr){ m_classType = CLASSTYPE::SCENE_MODEL; }
	~CSceneModel() {}

	void Init();
	void Uninit();
	void Update();
//	void Draw();
	void DrawDefferdRendering()override;	// デファードレンダリング用

	// 設定
	void SetBasePos(const D3DXVECTOR3 &pos) { m_basePos = pos; }
	void SetBaseRot(const D3DXVECTOR3 &rot) { m_baseRot = rot; }
	void SetSize(const D3DXVECTOR3 &size) { m_size = size; }
	void SetParent(CSceneModel *pParent) { m_pParent = pParent; }
	void SetFilePass(std::string strFilePass){ m_strFilePass = strFilePass; }

	// 取得
	const D3DXVECTOR3 &GetBasePos() { return m_basePos; }
	const D3DXVECTOR3 &GetBaseRot() { return m_baseRot; }
	std::string GetFilePath()const { return m_strFilePass; }
	std::string GetPartsName()const { return m_strPartsName; }
	const D3DXVECTOR3 &GetCenterPos() { return m_centerPos; }

	// マトリクス設定関数
	D3DXMATRIX GetMatrixRot(CSceneModel *pParent);
	D3DXMATRIX GetMatrixPos(CSceneModel *pParent); 
	D3DXMATRIX GetMatrixWorld(CSceneModel *pParts);

	static CSceneModel* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry = 4, CModelManager *pUseModelManager = nullptr);
	static CSceneModel* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, std::string strPartsName, int pry = 4, CModelManager *pUseModelManager = nullptr);
	static CSceneModel* GetParts(std::string partsName); 		// Partsの取得
	static CSceneModel* GetParent(CSceneModel *parts);			// 親Partsの取得

	// モデル生成
	void CreateModel(std::string strFilePass);

protected:
	std::string m_strFilePass;		// テクスチャFilePass
	std::string m_strPartsName;

	D3DXMATRIX m_mtxWorld;			// ワールドマトリクス
	D3DXMATRIX m_mtxRot;			// 
	D3DXMATRIX m_mtxPos;			// 
	D3DXMATRIX m_mtxScl;			// 
	D3DXVECTOR3 m_basePos;			// 座標
	D3DXVECTOR3 m_baseRot;			// 回転量

	CSceneModel* m_pParent;			// 親パーツのポインタ
	D3DXVECTOR3 m_size;				// 幅,高さ,奥行

	D3DXVECTOR3 m_centerPos;		// モデル全頂点の中心座標
	CModelManager *m_pUseModelManager;
};

#endif
