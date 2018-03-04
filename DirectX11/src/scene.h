// ======== ======== ======== ======== ======== ======== ======== ========
//
//	title : ポリゴン表示プログラム[ scene.h ]		Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
// -------- -------- -------- -------- -------- -------- -------- --------
//■Update : 2016/05/30
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE_H_
#define _SCENE_H_

#include <list>
#include <assert.h>

#include <algorithm>
#include <memory>
#include <fstream>

#include "main.h"
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 定数定義
// -------- -------- -------- -------- -------- -------- -------- --------
#define MAX_SCENE_PRIORITY	( 8 )		// 表示優先順位
#define NUM_VERTEX		( 4 )			// 頂点数
#define NUM_POLYGON		( 2 )			// ポリゴン数(プリミティブ数)

struct BinFile
{
	BinFile(const wchar_t* fpath)
	{
		std::ifstream binfile(fpath, std::ios::in | std::ios::binary);

		if (binfile.is_open()) {
			int fsize = static_cast<int>(binfile.seekg(0, std::ios::end).tellg());
			binfile.seekg(0, std::ios::beg);
			std::unique_ptr<char> code(new char[fsize]);
			binfile.read(code.get(), fsize);
			nSize = fsize;
			Bin = std::move(code);
		}

	}
	const void* get() const { return Bin.get(); }
	int size() const { return nSize; }
private:
	int nSize = 0;
	std::unique_ptr<char> Bin;
};

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene
{
public:
	typedef enum
	{// オブジェクトの種類
		OBJTYPE_NONE = 0,		// 無し
		LISTENER,
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

	typedef enum
	{// オブジェクトのクラスの種類
		CLASSTYPE_NONE = 0,		// 無し
		SCENE_2D,				// 
		SCENE_3D,				// 
		SCENE_3DPOLY,			// 
		SCENE_3DBOX,			// 箱
		SCENE_MODEL,			// モデル
		SCENE_CYLINDER,			// シリンダー
		SCENE_FIELD,			// フィールド
		SCENE_SKYDOME,			// スカイドーム
		CLASSTYPE_MAX			// 最大値
	}CLASSTYPE;
	
	CScene(int priority);	// 
	~CScene();				// デストラクタ

	virtual void Init(void) = 0;	// 初期化
	virtual void Uninit(void) = 0;	// 終了
	virtual void Update(void) = 0;	// 更新
	virtual void Draw(void) = 0;	// 描画

	static void UpdateAll(int priority);		// 指定priority全更新
	static void UpdateAll(void);				// 全更新
	static void DrawAll(int priority);			// 指定priority全描画
	static void DrawAll(void);					// 全描画
	static void ReleaseAll(void);				// 全リリース
	static void TypeRelease(const OBJTYPE &selectObjType);

	// 取得
	XMFLOAT3 GetPos(void)const{return m_pos;}		// 座標
	XMFLOAT3 GetScl(void)const{return m_scl;}		// 拡大率
	XMFLOAT3 GetRot(void)const{return m_rot;}		// 角度
	XMFLOAT3 GetSize(void)const{return m_size;}		// サイズ
	const int &GetR()const { return m_r; }
	const int &GetG()const { return m_g; }
	const int &GetB()const { return m_b; }
	const int &GetA()const { return m_a; }

	// 設定
	void SetPos(const XMFLOAT3 &pos){m_pos = pos;}		// 座標
	void SetScl(const XMFLOAT3 &scl){m_scl = scl;}		// 拡大率
	void SetRot(const XMFLOAT3 &rot){m_rot = rot;}		// 角度
	void SetSize(const XMFLOAT3 &size){m_size = size;}	// サイズ
	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

	// 加算
	void AddPos(XMFLOAT3 pos) { m_pos = XM::F3::Add(m_pos, pos); }		// 座標
	void AddScl(XMFLOAT3 scl){m_scl = XM::F3::Add(m_pos, scl);}			// 拡大率
	void AddRot(XMFLOAT3 rot){m_rot = XM::F3::Add(m_pos, rot);}			// 角度
	void AddSize(XMFLOAT3 size){m_size = XM::F3::Add(m_pos, size);}		// サイズ
	void AddCol(const int &r, const int &g, const int &b, const int &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	// タイプ
	void SetObjType(OBJTYPE type) { m_objType = type; }			// 生成時にオブジェクトのタイプを設定
	OBJTYPE GetObjType(void)const { return m_objType; }			// オブジェクトタイプの取得
	void SetClassType(CLASSTYPE type) { m_classType = type; }	// 生成時にクラスのタイプを設定
	CLASSTYPE GetClassType(void)const { return m_classType; }	// クラスタイプの取得

	// 描画優先度
	const int &GetPriority(void)const { return m_priority; }	// priorityの取得

	CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType);

	// scene
	static std::list<CScene*> *GetScene(int priority) {
		assert((priority > 0 || priority < MAX_SCENE_PRIORITY) && "PRIORITYの指定が間違っています");
		return &m_lisScene[priority];			// シーンの先頭
	}
	
protected:
	void Release(void) { m_delete = true; }	// リリース

	XMFLOAT3 m_pos;			// 座標
	XMFLOAT3 m_scl;			// 拡大率
	XMFLOAT3 m_rot;			// 回転角
	XMFLOAT3 m_size;		// サイズ
	int m_r, m_g, m_b, m_a;

	OBJTYPE m_objType;			// オブジェクトのタイプ
	CLASSTYPE m_classType;		// クラスのタイプ
	int m_priority;				// 描画優先度

private:
	
	static std::list<CScene*> m_lisScene[MAX_SCENE_PRIORITY];
	static int m_numScene[MAX_SCENE_PRIORITY];		// シーンの総数
	bool m_delete;									// 解放フラグ
};

#endif

