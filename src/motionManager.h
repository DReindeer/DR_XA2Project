#ifndef _MOTIONMANAGER_H_
#define _MOTIONMANAGER_H_

#include "main.h"
#include <string>
#include <unordered_map>

// --------モーション読み込みクラス
class CSceneModel;
class CMotionManager
{
public:
	typedef struct
	{
		int motionType;				// 現在モーションの種類
		int cntFlame;				// 現在フレーム数
		int numKey;					// 現在キー数

		int motionTypeNext;			// 次のモーションの種類
		int cntBlendFlame;			// 次のフレーム数

		int motionBlendFrame;		// モーション間補間時のフレーム数

		std::string motionFilePass;
		std::vector<CSceneModel*> *pVecModel;	// パーツ情報
	}MOTION_DATA;

	typedef struct
	{
		float move;					// 移動量
		float jump;					// ジャンプ量
		float rad;					// 半径
		int numParts;				// パーツ数 (PARTSクラスの総数)
	}CHARA;

	// パーツ情報一時読み取り用構造体
	typedef struct
	{
		int index, parent;
		D3DXVECTOR3 pos, rot, basePos, baseRot;
		std::string filePass;
	}PARTS;

	// パーツ座標情報
	typedef struct
	{
		D3DXVECTOR3 pos;			// 座標
		D3DXVECTOR3 rot;			// 回転量
		int index;					// パーツインデックス

		D3DXVECTOR3 posParFlame;	// 1フレームに移動する座標
		D3DXVECTOR3 rotParFlame;	// 1フレームに回転する量
	}MOVEPARTS;

	// キー情報
	typedef struct
	{
		int	flame;					// 実行フレーム数
		int index;					// キーインデックス

		std::list<MOVEPARTS*> lisMoveParts;	// 移動パーツ
	}KEY;

	// モーション情報
	typedef struct
	{
		bool loop;					// 繰り返しの有無
		int numKey;					// キー数 (MOTION毎のKEY構造体の総数)
		int index;					// モーションインデックス

		std::list<KEY*> lisKey;		// キー
	}MOTION;

	// コンストラクタ, デストラクタ
	CMotionManager() {}
	~CMotionManager() {}

	// -------- -------- -------- --------
	// 皆が使うモーションに関する場所
	static void GetCharaPartsList(MOTION_DATA *pMotionData, D3DXVECTOR3 scl = D3DXVECTOR3(1.f, 1.f, 1.f));	// パーツのリストを取得
	void UpdateMotion(MOTION_DATA *pMotionData);
	void SetMotionType(MOTION_DATA *pMotionData, int motionTypeNext);

	// -------- -------- -------- --------
	// マネージャで記述
	// 生成
	static void LoadMotion(const std::string &strFilePass);	// モーションマネージャに追加
	// 全消去
	static void ReleaseAll();							// モデル全消去

	// 取得
	CHARA GetChara() { return m_chara; }
	CMotionManager *GetMotionManager(std::string strName);			// モーションマネージャの取得
	MOTION *GetMotion(int index);									// モーションの取得
	KEY *GetKey(MOTION *pMotion, int index);						// Keyの取得
	MOVEPARTS *GetMoveParts(KEY *pKey, int index);					// MovePartsの取得

private:
	// メンバ関数

	static bool seekSrcData(char *texName);	// 指定文字列まで読み進める
	static int getNumInt();				// intの値を返す
	static float getNumFloat();			// floatの値を返す
	static char *getChar();				// charのポインタを返す
	
	// メンバ変数
	static int m_fileSize;					// ファイル文字数
	static int m_currentSrc;				// 現在位置
	static char *m_pSrcData;				// 文字列格納用ポインタ
	static char m_buf[MAX_PATH];			// バッファ(一時文字格納用)

	int m_numModel;							// モデル数
	CHARA m_chara;							// キャラクター
	std::vector<PARTS*> m_vecParts;			// パーツ
	std::list<MOTION*> m_lisMotion;			// モーション
	static std::unordered_map<std::string, CMotionManager*> m_umapMotionManager;	// モーションマネージャ
};

#endif