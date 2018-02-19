//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include <stdlib.h>
#include "main.h"
#include "stdio.h"
#include "utility.h"
#include "modelManager.h"
#include "texManager.h"
#include "motionManager.h"
#include "sceneModel.h"

#include <assert.h>

//================================================================================
// 静的メンバ変数
//--------------------------------------------------------------------------------
std::unordered_map<std::string, CMotionManager*> CMotionManager::m_umapMotionManager;	// モーションマネージャ

int CMotionManager::m_fileSize = 0;				// ファイル文字数
int CMotionManager::m_currentSrc = 0;			// 現在位置
char *CMotionManager::m_pSrcData = nullptr;		// 文字列格納用ポインタ
char CMotionManager::m_buf[MAX_PATH] = { "" };	// バッファ(一時文字格納用)

//================================================================================
// モーション読み込み
//--------------------------------------------------------------------------------
void CMotionManager::LoadMotion(const std::string &strFilePass)
{
	assert(!strFilePass.empty() && "モーションのファイルパスが不明です");

	// 要素の有無を確かめる
	if (m_umapMotionManager.count(strFilePass) == 1)
	{// ありますよ
		return;
	}

	// 初期化
	m_fileSize = 0;			// ファイル文字数
	m_currentSrc = 0;		// 現在位置	
	m_pSrcData = nullptr;	// 文字列格納用ポインタ

	// ファイル読み込み
	FILE *pFile;			// ファイルポインタ
	fopen_s(&pFile, &strFilePass[0], "r");
	// nullptrチェック
	if (pFile == nullptr) {
		assert(!"モーションのファイルパスが読み込めませんでした");
	}

	// モーションマネージャの実体の生成
	CMotionManager *pMotManager = new CMotionManager;

	fseek(pFile, 0, SEEK_END);			// 
	m_fileSize = ftell(pFile);			// 
	fseek(pFile, 0, SEEK_SET);			// ファイルサイズ取得

	m_pSrcData = (char *)malloc(m_fileSize);	// 読み込みバッファー確保

	fread(m_pSrcData, m_fileSize, 1, pFile);	// ファイル読み込み
	fclose(pFile);								// ファイルを閉じる

	// スクリプト読み込み開始
	seekSrcData("SCRIPT");
	if (m_currentSrc >= m_fileSize)
	{
		free(m_pSrcData);
		assert(!"スクリプトの中身に[SCRIPT]がありません");
	}

	//========================================
	// モデル数の取得
	//========================================
	seekSrcData("NUM_MODEL = ");
	pMotManager->m_numModel = getNumInt();

	//========================================
	// モデルファイル名の取得
	//========================================
	PARTS *pParts;
	CModelManager *pModelManager = CManager::GetModelManager();
	for (int i = 0; i < pMotManager->m_numModel; i++)
	{
		seekSrcData("MODEL_FILENAME = ");
		pParts = new PARTS;
		pParts->filePass = getChar();

		// モデルの登録
		pModelManager->SetModel(pParts->filePass);

		pMotManager->m_vecParts.push_back(pParts);
	}
	pMotManager->m_vecParts.shrink_to_fit();

	//========================================
	// キャラクター情報
	//========================================
	// 移動量
	seekSrcData("MOVE = ");
	pMotManager->m_chara.move = getNumFloat();
	// ジャンプ量
	seekSrcData("JUMP = ");
	pMotManager->m_chara.jump = getNumFloat();
	// あたり判定の半径
	seekSrcData("RADIUS = ");
	pMotManager->m_chara.rad = getNumFloat();
	// パーツ数
	seekSrcData("NUM_PARTS = ");
	pMotManager->m_chara.numParts = getNumInt();

	//========================================
	// パーツ情報の読み取り
	//========================================
	float x, y, z;
	for (int i = 0; i < pMotManager->m_chara.numParts; i++)
	{
		// インデックス
		seekSrcData("INDEX = ");
		pMotManager->m_vecParts[i]->index = getNumInt();
		// 親のインデックス
		seekSrcData("PARENT = ");
		pMotManager->m_vecParts[i]->parent = getNumInt();
		// 座標
		seekSrcData("POS = ");
		x = getNumFloat();
		y = getNumFloat();
		z = getNumFloat();
		pMotManager->m_vecParts[i]->pos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pMotManager->m_vecParts[i]->basePos = D3DXVECTOR3(x, y, z);
		// 角度
		seekSrcData("ROT = ");
		x = getNumFloat();
		y = getNumFloat();
		z = getNumFloat();
		pMotManager->m_vecParts[i]->rot = D3DXVECTOR3(0.f, 0.f, 0.f);
		pMotManager->m_vecParts[i]->baseRot = D3DXVECTOR3(x, y, z);
	}

	//========================================
	// モーション情報
	//========================================
	int motionIndex = 0;
	bool bMotion = false;
	MOTION *pMotion;		// モーション
	// モーション分ループ
	while (1)
	{
		// 末尾まで行ったらループを抜ける
		if (strcmp(&m_pSrcData[m_currentSrc], "END_SCRIPT") == 0 || m_currentSrc >= m_fileSize)
		{
			break;
		}
		if (bMotion == true)
		{
			seekSrcData("END_MOTIONSET");
			bMotion = false;
		}
		// モーションがあったら
		if (seekSrcData("MOTIONSET"))
		{
			bMotion = true;

			// -------- -------- -------- --------
			// モーションの生成
			// -------- -------- -------- --------
			pMotion = new MOTION;
			// モーションの初期化
			pMotion->index = 0;
			pMotion->numKey = 0;
			pMotion->loop = false;

			pMotion->index = motionIndex;	// インデックスの設定
		}
		else
		{
			break;
		}

		// ループするかどうか
		seekSrcData("LOOP = ");
		pMotion->loop = (bool)getNumInt();
		// キーの数
		seekSrcData("NUM_KEY = ");
		pMotion->numKey = getNumInt();

		// -------- -------- -------- --------
		// キーの宣言
		// -------- -------- -------- --------
		KEY *pKey;

		// キー数分ループ
		for (int cntKey = 0, end = pMotion->numKey; cntKey < end; cntKey++)
		{
			// 移動フレーム
			seekSrcData("FRAME = "); 
			pKey = new KEY;
			// キーの初期化
			pKey->flame = 0;
			pKey->index = 0;
			pKey->flame = getNumInt();
			pKey->index = cntKey;			// インデックスの取得

			// -------- -------- -------- --------
			// ムーブパーツの宣言
			// -------- -------- -------- --------
			MOVEPARTS *pMoveParts;

			// パーツ分ループ
			for (int cntParts = 0; cntParts < pMotManager->m_chara.numParts; cntParts++)
			{
				pMoveParts = new MOVEPARTS;
				// ムーブパーツの初期化
				pMoveParts->index = 0;
				pMoveParts->pos = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rot = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->posParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rotParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);

				pMoveParts->index = cntParts;		// インデックスの設定

				// 座標
				seekSrcData("POS = ");
				x = getNumFloat();
				y = getNumFloat();
				z = getNumFloat();
				pMoveParts->pos = D3DXVECTOR3(x, y, z);
				// 角度
				seekSrcData("ROT = ");
				x = getNumFloat();
				y = getNumFloat();
				z = getNumFloat();
				pMoveParts->rot = D3DXVECTOR3(x, y, z);
				seekSrcData("END_KEY");

				// フレーム毎の移動量の初期化
				pMoveParts->posParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rotParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);

				// ムーブパーツをプッシュバック
				pKey->lisMoveParts.push_back(pMoveParts);
			}
			seekSrcData("END_KEYSET");

			// キーをプッシュバック
			pMotion->lisKey.push_back(pKey);
		}

		// モーションをプッシュバック
		pMotManager->m_lisMotion.push_back(pMotion);
		motionIndex++;	// インデックスを進める
	}

	// モーションの情報をマネージャに入れる
	m_umapMotionManager[strFilePass] = pMotManager;

	// バッファ解放
	free(m_pSrcData);
}

//================================================================================
// パーツリストの取得(vector)
//--------------------------------------------------------------------------------
void CMotionManager::GetCharaPartsList(MOTION_DATA *pMotionData, D3DXVECTOR3 scl)
{
	if (m_umapMotionManager.count(pMotionData->motionFilePass) == 0)
	{
		LoadMotion(pMotionData->motionFilePass);
	}

	if (pMotionData->pVecModel == nullptr)
	{
		pMotionData->pVecModel = new std::vector<CSceneModel*>;
	}

	PARTS *pParts;
	CSceneModel *pModel;

	// パーツ数
	CMotionManager *p = m_umapMotionManager[pMotionData->motionFilePass];
	int numParts = p->m_chara.numParts;

	for (int i = 0; i < numParts; i++)
	{
		pParts = p->m_vecParts[i];
		pModel = CSceneModel::Create(pParts->pos, pParts->rot, D3DXVECTOR3(1.f, 1.f, 1.f), pParts->filePass);
		pMotionData->pVecModel->push_back(pModel);
	}

	// 親パーツのファイルパスを調べる
	for (int i = 0; i < numParts; i++)
	{
		pParts = p->m_vecParts[i];
		pModel = pMotionData->pVecModel->at(i);
		if (pParts->parent == -1)
		{
			// 親のスケールを再設定
			pModel->SetScl(scl);
			// パーツの親はnull
			pModel->SetParent(nullptr);
			continue;
		}
		// パーツに親のポインタを設定
		CSceneModel *parent = pMotionData->pVecModel->at(pParts->parent);
		pModel->SetParent(parent);
	}
	pMotionData->pVecModel->shrink_to_fit();
}

//================================================================================
// モーションをさせる
//--------------------------------------------------------------------------------
void CMotionManager::UpdateMotion(MOTION_DATA *pMotionData)
{
	if (pMotionData == nullptr) { assert(!"引数がnullです"); }
	if (m_umapMotionManager.count(pMotionData->motionFilePass) == 0) { assert(!"ファイル名が設定されていません"); }

	// モーションデータの取得
	CMotionManager *p = m_umapMotionManager[pMotionData->motionFilePass];
	MOTION *pMotion = p->GetMotion(pMotionData->motionType);

	// パーツ数
	int numParts = p->m_chara.numParts;
	/*
	for (int cntParts = 0; cntParts < numParts; cntParts++)
	{
		pParts = pMotionData->pVecModel->at(cntParts);
		KEY* pKey = p->GetKey(pMotion, pMotionData->numKey);
		MOVEPARTS* pMoveParts = p->GetMoveParts(pKey, cntParts);
		pParts->SetRot(pMoveParts->rotParFlame);	// 回転
		pParts->SetPos(pMoveParts->posParFlame);	// 座標
	}
	*/

	// カウントを進める
	pMotionData->cntFlame++;

	CSceneModel *pParts;
	KEY* pKey = nullptr;
	KEY* pKeyNext = nullptr;
	// フレームカウントがキーのフレーム数になったら次のキーへ
	if (pMotionData->cntFlame >= p->GetKey(pMotion, pMotionData->numKey)->flame)
	{
		pMotionData->cntFlame = 0;		// フレームを初期化
		pMotionData->numKey++;			// 参照するキーのインデックスを進める

		// キーが突き抜けたら初期化
		if (p->GetKey(pMotion, pMotionData->numKey) == nullptr)
		{
			pMotionData->numKey = 0;	// キーをを0に戻す
		}

		pKey = p->GetKey(pMotion, pMotionData->numKey);			// 現在のキー
		pKeyNext = p->GetKey(pMotion, pMotionData->numKey + 1);	// 次のキー
		if (pMotion->loop == false)
		{// ループなしの場合は、現在のキーをもう一度
			pKeyNext = pKey;
		}
		else if (pKeyNext == nullptr)
		{// 最初のキー
			pKeyNext = p->GetKey(pMotion, 0);
		}
	}

	MOVEPARTS* pMoveParts = nullptr;			// 現在の補間
	MOVEPARTS* pMovePartsNext = nullptr;		// 
	D3DXVECTOR3 rotParFlame, posParFlame;

	// モーションブレンド
	MOVEPARTS* pBlendMoveParts;			// 次の補間
	MOVEPARTS* pBlendMovePartsNext;		// 
	D3DXVECTOR3 BlendRotParFlame, BlendPosParFlame;

	float rate = (float)pMotionData->cntFlame / pKey->flame;
	MOTION *pMotionNext = p->GetMotion(pMotionData->motionTypeNext);
	pKeyNext = p->GetKey(pMotionNext, 0);

	//float blendRate = (float)pMotionData->cntBlendFlame / pKeyNext->flame;
	float motionBlendRate = pMotionData->motionBlendFrame / 30.f;

	if (motionBlendRate >= 1.f)
	{
		pMotionData->motionType = pMotionData->motionTypeNext;
	}

	if (pMotionData->motionTypeNext == pMotionData->motionType)
	{
		// パーツ毎に1フレームに移動する値を代入
		for (int n = 0; n < numParts; n++)
		{
			// 現在の補間
			pMoveParts = p->GetMoveParts(pKey, n);				// 現在ムーブパーツ
			pMovePartsNext = p->GetMoveParts(pKeyNext, n);		// 次のムーブパーツ
			pMoveParts->rotParFlame = pMoveParts->rot * (1.f - rate) + pMovePartsNext->rot * (rate);	// 角度
			pMoveParts->posParFlame = pMoveParts->pos * (1.f - rate) + pMovePartsNext->pos * (rate);	// 座標

			pParts = pMotionData->pVecModel->at(n);
			pParts->SetRot(pMoveParts->rotParFlame);	// 回転
			pParts->SetPos(pMoveParts->posParFlame);	// 座標
		}
	}
	else
	{
		// パーツ毎に1フレームに移動する値を代入
		for (int n = 0; n < numParts; n++)
		{
			// 現在の補間
			pMoveParts = p->GetMoveParts(pKey, n);				// 現在ムーブパーツ
			pMovePartsNext = p->GetMoveParts(pKeyNext, n);		// 次のムーブパーツ
			rotParFlame = pMoveParts->rot * (1.f - rate) + pMovePartsNext->rot * (rate);	// 角度
			posParFlame = pMoveParts->pos * (1.f - rate) + pMovePartsNext->pos * (rate);	// 座標

			// 次の補間
			pMotion = p->GetMotion(pMotionData->motionTypeNext);
			pKey = p->GetKey(pMotion, pMotionData->numKey);
			pMotionNext = p->GetMotion(pMotionData->motionTypeNext);
			pKeyNext = p->GetKey(pMotionNext, 0);

			pBlendMoveParts = p->GetMoveParts(pKey, n);				// 次のモーションの現在ムーブパーツ
			pBlendMovePartsNext = p->GetMoveParts(pKeyNext, n);		// 次のモーションの次のムーブパーツ
			BlendRotParFlame = pBlendMoveParts->rot * (1.f - rate) + pBlendMovePartsNext->rot * (rate);	// 角度
			BlendPosParFlame = pBlendMoveParts->pos * (1.f - rate) + pBlendMovePartsNext->pos * (rate);	// 座標

			pMoveParts->rotParFlame = rotParFlame;
			pMoveParts->posParFlame = posParFlame;

			pParts = pMotionData->pVecModel->at(n);
			pParts->SetRot(pMoveParts->rotParFlame);	// 回転
			pParts->SetPos(pMoveParts->posParFlame);	// 座標
		}
	}
}

//================================================================================
// モーションタイプの設定
//--------------------------------------------------------------------------------
void CMotionManager::SetMotionType(MOTION_DATA *pMotionData, int motionTypeNext)
{
	if (pMotionData == nullptr) { assert(!"引数がnullです"); }
	if (m_umapMotionManager.count(pMotionData->motionFilePass) == 0) { assert(!"ファイル名が設定されていません"); }
	if (pMotionData->motionType == motionTypeNext) { return; }

	pMotionData->motionTypeNext = motionTypeNext;
}

//================================================================================
// 文字列の次の文字まで進める
//--------------------------------------------------------------------------------
bool CMotionManager::seekSrcData(char *texName)
{
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	strcat_s(m_buf, texName);
	int len = strlen(&m_buf[0]);
	while (1)
	{
		if (strncmp(&m_pSrcData[m_currentSrc], m_buf, len) == 0)
		{
			m_currentSrc += len;// - 1;
			return true;
		}
		else
		{
			m_currentSrc++;
		}
		if (m_currentSrc >= m_fileSize) { break; }
	}
	return false;
}

//================================================================================
// 文字列をint型で取得(特殊)
//--------------------------------------------------------------------------------
int CMotionManager::getNumInt()
{
	int numVtx = 0;		// 総数
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// 空白前が値のため、空白が来るまでループ
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			numVtx = atoi(m_buf);		// 頂点数の総数の取得
			m_currentSrc++;
			return numVtx;
		}
		else
		{// 数字 or . or - それ以外は無視
			if (m_pSrcData[m_currentSrc] >= 48 && m_pSrcData[m_currentSrc] < 57 || m_pSrcData[m_currentSrc] == 46 || m_pSrcData[m_currentSrc] == 45)
			{
				// 文字列の連結
				strncat_s(&m_buf[0], MAX_PATH, &m_pSrcData[m_currentSrc], 1);
			}
			m_currentSrc++;
		}
	}
	return numVtx;
}

//================================================================================
// 文字列をfloat型で取得(特殊)
//--------------------------------------------------------------------------------
float CMotionManager::getNumFloat()
{
	float numVtx = 0.f;	// 総数
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// 空白前が値のため、空白が来るまでループ
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			numVtx = static_cast<float>(atof(m_buf));		// 頂点数の総数の取得
			m_currentSrc++;
			return numVtx;
		}
		else
		{// 数字 or . or - それ以外は無視
			if (m_pSrcData[m_currentSrc] >= 48 && m_pSrcData[m_currentSrc] < 57 || m_pSrcData[m_currentSrc] == 46 || m_pSrcData[m_currentSrc] == 45)
			{
				// 文字列の連結
				strncat_s(&m_buf[0], MAX_PATH, &m_pSrcData[m_currentSrc], 1);
			}
			m_currentSrc++;
		}
	}
	return numVtx;
}

//================================================================================
// 文字列をchar型で取得(特殊)
//--------------------------------------------------------------------------------
char *CMotionManager::getChar()
{
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// 空白前が値のため、空白が来るまでループ
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			m_currentSrc++;
			return &m_buf[0];
		}
		else
		{
			// 文字列の連結
			strncat_s(&m_buf[0], MAX_PATH, &m_pSrcData[m_currentSrc], 1);
			m_currentSrc++;
		}

		if (strncmp(&m_pSrcData[m_currentSrc], "#", 1) == 0)
		{
			return nullptr;
		}
	}
	return &m_buf[0];
}

//================================================================================
// モーション情報全消去
//--------------------------------------------------------------------------------
void CMotionManager::ReleaseAll()
{
	// コンテナを末尾までループ
	for (auto it : m_umapMotionManager)
	{
		// モーション
		if (it.second != nullptr)
		{
			if (it.second->m_pSrcData != nullptr)
			{
				delete it.second->m_pSrcData;
				it.second->m_pSrcData = nullptr;
			}
			delete it.second;
			it.second = nullptr;
		}
	}
	m_umapMotionManager.clear();
}

// モーションマネージャ
CMotionManager *CMotionManager::GetMotionManager(std::string strName)
{
	if (m_umapMotionManager.count(strName) >= 1)
	{
		return m_umapMotionManager[strName];
	}
	assert(!"引数のモーション名の指定が間違っています");
	return nullptr;
}

// モーション
CMotionManager::MOTION *CMotionManager::GetMotion(int index)
{
	for (auto pMotion : m_lisMotion)
	{
		if (pMotion->index == index) { return pMotion; }
	}
	return nullptr;
}

// キー
CMotionManager::KEY *CMotionManager::GetKey(MOTION *pMotion, int index)
{
	for (auto pKey : pMotion->lisKey)
	{
		if (pKey->index == index) { return pKey; }
	}
	return nullptr;
}

// pMoveParts
CMotionManager::MOVEPARTS *CMotionManager::GetMoveParts(KEY *pKey, int index)
{
	for (auto pMoveParts : pKey->lisMoveParts)
	{
		if (pMoveParts->index == index) { return pMoveParts; }
	}
	return nullptr;
}