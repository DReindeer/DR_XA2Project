//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "mode.h"
#include "manager.h"
#include <list>
#include <mutex>

//--------------------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------------------
#define NUM_VERTEX		( 4 )			// 頂点数
#define NUM_POLYGON		( 2 )			// ポリゴン数(プリミティブ数)
#define MAX_SCENE_PRIORITY	( 10 )		// 表示優先順位

// 頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット	DIFFUSE:色 TEX1:テクスチャ座標(UVやST)
// 頂点フォーマット
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//--------------------------------------------------------------------------------
// 構造体宣言
//--------------------------------------------------------------------------------
typedef struct
{// 頂点構造体
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nom;	// 法線座標
	D3DXVECTOR4 col;	// 色
	D3DXVECTOR2 tex;	// UV
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 pos;	// 
	float rhw;			// 座標変換済み頂点フラグ(必ず1.fが入る)
	D3DCOLOR col;		// 頂点の色
	D3DXVECTOR2 tex;	// テクスチャ
}VERTEX_2D;

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------
class CScene
{
public:
	// オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,		// 無し
		MODEL,
		PLAYER,					// プレイヤー
		ENEMY,					// 敵
		GRAVITY_OBJ,			// 
		BULLET,					// 弾
		EXPLOSION,				// 爆発
		EFFECT,					// エフェクト
		PARTICLE,				// パーティクル
		FIELD,					// 
		FIELDX,
		FONT,					// 
		OBJTYPE_MAX				// 最大値
	}OBJTYPE;

	// オブジェクトのクラスの種類
	typedef enum
	{
		CLASSTYPE_NONE = 0,		// 無し
		SCENE_2D,				// 

		SCENE_3DPOLY,			// 
		SCENE_MODEL,			// モデル
		SCENE_CYLINDER,			// シリンダー
		SCENE_FIELD,			// フィールド
		SCENE_SKYDOME,			// スカイドーム
		CLASSTYPE_MAX			// 最大値
	}CLASSTYPE;

	// ポリゴンの種類
	typedef enum
	{
		POLYGON_TYPE_NONE = -1,
		POLYGON_TYPE_2D = 0,	// 2D
		POLYGON_TYPE_3D,		// 3D
	}POLYGON_TYPE;

	CScene(int priority);		// 
	~CScene();					// デストラクタ

	virtual void Init() = 0;	// 初期化
	virtual void Uninit() = 0;	// 終了
	virtual void Update() = 0;	// 更新
	virtual void Draw() {};		// 描画
	virtual void DrawDefferdRendering(){}	// デファードレンダリング用

	static void UpdateAll(int priority);	// 指定priority全更新
	static void UpdateAll();				// 全更新
	static void DrawAll(int priority);		// 指定priority全描画
	static void DrawAll();					// 全描画
	static void DrawDefferdRenderingAll();	// デファードレンダリング用全描画
	void Release() { m_delete = true; }		// リリース
	static void RemoveAll();
	static void ReleaseAll();				// 全リリース
	static void FlagReleaseAll();
	static void TypeRelease(const OBJTYPE &selectObjType);

	// Deviceを占有ロックしてバッファ書き込み
	static void WriteBuffer(LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, VERTEX_3D* pInVtx, int numVtx
							,LPDIRECT3DINDEXBUFFER9 pIdxBuffer, WORD *pInIdx, int numIdx);

	// 取得
	LPDIRECT3DVERTEXBUFFER9 *GetVtxBuffer() { return &m_pVtxBuffer; }
	const D3DXVECTOR3 &GetPos() { return m_pos; }		// 座標
	const D3DXVECTOR3 &GetScl() { return m_scl; }		// 拡大率
	const D3DXVECTOR3 &GetRot() { return m_rot; }		// 角度
	const D3DXVECTOR3 &GetSize() { return m_size; }		// サイズ
	const int &GetR() { return m_r; }
	const int &GetG() { return m_g; }
	const int &GetB() { return m_b; }
	const int &GetA() { return m_a; }

	// 設定
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }		// 座標
	void SetScl(const D3DXVECTOR3 &scl) { m_scl = scl * CManager::GetMasterScale();	}		// 拡大率
	void SetScl(const float &scl) { m_scl = D3DXVECTOR3(scl, scl, scl); }		// 拡大率
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }		// 角度
	void SetSize(const D3DXVECTOR3 &size) { m_size = size; }	// サイズ
	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

	// 加算
	void AddPos(const D3DXVECTOR3 &pos) { m_pos += pos; }		// 座標
	void AddScl(const D3DXVECTOR3 &scl) { m_scl += scl * CManager::GetMasterScale(); }		// 拡大率
	void AddScl(const float &scl) { m_scl += D3DXVECTOR3(scl, scl, scl) * CManager::GetMasterScale();}		// 拡大率
	void AddRot(const D3DXVECTOR3 &rot) { m_rot += rot; }		// 角度
	void AddSize(const D3DXVECTOR3 &size) { m_size += size; }	// サイズ
	void AddCol(const int &r, const int &g, const int &b, const int &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	// タイプ
	void SetObjType(OBJTYPE type) { m_objType = type; }			// 生成時にオブジェクトのタイプを設定
	OBJTYPE GetObjType() { return m_objType; }					// オブジェクトタイプの取得
	void SetClassType(CLASSTYPE type) { m_classType = type; }	// 生成時にクラスのタイプを設定
	CLASSTYPE GetClassType() { return m_classType; }			// クラスタイプの取得

// 描画優先度
	const int &GetPriority() { return m_priority; }				// priorityの取得

// 唯一描画
	static void DrawTypeOnly(CLASSTYPE classType) {
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		m_drawTypeOnly = classType;
	}
	static CScene *GetSceneClassType(const CLASSTYPE &selectObjType);

	// scene
	static std::list<CScene*> *GetScene(int priority) {
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		if (priority < 0 || priority >= MAX_SCENE_PRIORITY)MessageBox(nullptr, "PRIORITYの指定が間違っています", "警告", MB_OK);
		return &m_lisScene[priority];			// シーンの先頭
	}

	// ライト
	bool GetLightOff() { return m_lightOff; }
	void SetLightOff(bool lightOff = true) { m_lightOff = lightOff; }
	// アルファ
	bool GetAlpha() { return m_alpha; }
	void SetAlpha(bool alpha = true) { m_alpha = alpha; }
	// 描画するか
	bool GetDraw() { return m_drawFlag; }
	void SetDrawFlag(bool draw = true) { m_drawFlag = draw; }
	// シーン跨ぎで消さないでほしいフラグ
	bool GetOverall() { return m_overall; }
	void SetOverall(bool overall = true) { m_overall = overall; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// Vtxバッファ

	D3DXVECTOR3 m_pos;			// 座標
	D3DXVECTOR3 m_rot;			// 回転角
	D3DXVECTOR3 m_size;			// サイズ
	int m_r, m_g, m_b, m_a;

	OBJTYPE m_objType;			// オブジェクトのタイプ
	CLASSTYPE m_classType;		// クラスのタイプ
	int m_priority;				// 描画優先度
	static CLASSTYPE m_drawTypeOnly;	// 唯一描画したいものだけを描画

	bool m_lightOff;
	bool m_alpha;
	bool m_drawFlag;

private:
	D3DXVECTOR3 m_scl;				// 拡大率
	static std::list<CScene*> m_lisScene[MAX_SCENE_PRIORITY];
	bool m_delete;					// 解放フラグ
	bool m_overall;					// 遷移で消されない様にするフラグ
	CMode::MODETYPE m_createModeType;
};

#endif
