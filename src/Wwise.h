// ======== ======== ======== ======== ======== ======== ======== ========
//
//			[Wwise.h] 							Auter : KENSUKE WATANABE
//												Data  : 2017/09/06
//
// -------- -------- -------- -------- -------- -------- -------- --------
//■Update : 2018/02/06
// 
// ● CheckEndEvent() - イベントが終了しているかどうか判別 : 2018/02/06
// 
// ======== ======== ======== ======== ======== ======== ======== ========
//
// 使い方！
//
// 〇 Init(), Uninit(), Update() はManager.cppに書いて下さい
// 〇 リスナー(m_pMainListener)は、プレイヤー等の位置や向きに合わせて
//    毎フレーム設定し直して下さい。(Wwiseクラス内にメインリスナー用の関数あり)
//
// ① CreateNoListenerGameObj() - ゲームオブジェクトを作成！
// ② SetEvent() - イベントを再生！
// ③ Stop() - プログラム側でイベントの停止！(これの使用はミュージックの人と要相談)
// 
// -------- -------- -------- -------- -------- -------- -------- --------
#ifndef _WWISE_H_
#define _WWISE_H_

#include <windows.h>
#include <unordered_map>
#include <assert.h>

// Release時に定義させる
#ifndef _DEBUG
#define AK_OPTIMIZED
#endif

// Wwise(サウンドエンジン)を使用するために必要
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// サウンドエンジン
#include <AK/IBytes.h>

#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>	// タイプ

#include "Wwise_IDs.h"									// イベントのID

using namespace AK;
using namespace AK::SoundEngine;

// ======== ======== ======== ======== ======== ======== ======== ========
// 構造体
// -------- -------- -------- -------- -------- -------- -------- --------
// ゲームオブジェクト
struct WWISE_GAMEOBJ
{
	D3DXVECTOR3 pos, rot;		// 座標, 回転角			- 数学的な0度からの回転角
	D3DXVECTOR3 vel;			// 速度					- ドップラー効果で必要？
};

// 終了したイベントの情報を格納 - コールバック関数で使用
struct WWISE_ENDEVENT_GAMEOBJ
{
	AkGameObjectID gameObjID;	// ゲームオブジェクトID
	AkUniqueID eventID;			// イベントID
};

// 前方宣言
class CAkFilePackageLowLevelIOBlocking;

// ======== ======== ======== ======== ======== ======== ======== ========
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class Wwise
{
public:
	Wwise(){}
	~Wwise(){}

	void Init(HWND hWnd);	// 初期化
	void Uninit();			// 終了
	void Update();			// 更新

	// ゲームオブジェクトの生成
	// -------- -------- -------- -------- -------- -------- -------- --------
	WWISE_GAMEOBJ *CreateNoListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr, char *gameObjName = nullptr);	// Listener無しオブジェクト
	WWISE_GAMEOBJ *CreateMainListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *gameObjName = "Listener (Default)");	// メインリスナー
	void DeleteGameObj(void *thisPtr);


	// イベント送信
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetEvent(int eventID, void *thisPtr);		// 指定ゲームオブジェクトにIDでイベントを送信

	// イベントが既に終了しているか調べる
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool CheckEndEvent(int eventID, void *thisPtr = nullptr);

	// ゲームオブジェクトの再生を停止
	// -------- -------- -------- -------- -------- -------- -------- --------
	void Stop(void *thisPtr);


	// イベントの状態を変更
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetState(int stateGroup, int stateID);			// ステートの設定
	void SetGameParam(int rtpcID, float value);			// ゲームパラメーター(RTPC)の設定


	// マテリアル
	// -------- -------- -------- -------- -------- -------- -------- --------
	int GetMaterialId(char *name);						// 名前からマテリアルIDの取得
//	void SetMaterial(void *gameObject, int materialid);	// ゲームオブジェクトにマテリアルIDを設定


	// メインリスナー系
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetMainListenerEvent(int eventID);		// メインリスナー(プレイヤー)にイベントを送信
	void StopMainListener();
	void SetPos(const D3DXVECTOR3 &pos) { m_pMainListener->pos = pos; }		// 座標
	void SetRot(const D3DXVECTOR3 &rot) { m_pMainListener->rot = rot; }		// 回転
	void SetVel(const D3DXVECTOR3 &vel) { m_pMainListener->vel = vel; }		// 速度


	// 各ゲームオブジェクトの位置、座標の設定
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetPos(void *thisPtr, const D3DXVECTOR3 &pos) {			// 座標
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[thisPtr]->pos = pos;
	}
	void SetRot(void *thisPtr, const D3DXVECTOR3 &rot) {			// 回転
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[thisPtr]->rot = rot;
	}
	void SetVel(void *thisPtr, const D3DXVECTOR3 &vel) {			// 速度
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[thisPtr]->vel = vel;
	}

//	void SetMultiplePositions(void *thisPtr, WWISE_GAMEOBJ *pArrayGameObj) {
//		AkChannelEmitter *AkArrayPos;
//		// 上方向ベクトル
//		AkVector top;
//		float fRadPitch = pArrayGameObj[0].rot.z;
//		float fCosPitch = cos(fRadPitch);
//		float fSinPitch = sin(fRadPitch);
//		float fRadYaw = pArrayGameObj[0].rot.y - D3DX_PI * 0.5f;
//		float fCosYaw = cos(fRadYaw);
//		float fSinYaw = sin(fRadYaw);
//		top.X = -fCosYaw * fSinPitch;
//		top.Y = fCosPitch;
//		top.Z = -fSinYaw * fSinPitch;
//		AkArrayPos->position.Set(VecToAkVector(pArrayGameObj[0].pos), VecToAkVector(pArrayGameObj[0].rot), top);
//		AK::SoundEngine::SetMultiplePositions((AkGameObjectID)thisPtr, AkArrayPos, 
//	}

	// バックグラウンド/フォアグラウンド時の処理
	// false すべてのオーディオレンダリングを停止する
	// true  一部を停止する (イベントの処理は継続するが、オーディオをレンダリングをしない)
	// -------- -------- -------- -------- -------- -------- -------- --------
	void Suspend(bool suspend) { SoundEngine::Suspend(suspend); }	// バックグラウンド/フォアグラウンド時の処理
	void WakeupFromSuspend() { SoundEngine::WakeupFromSuspend(); }	// Suspendで停止した処理の再開
	

	// D3DXVECTOR3とAkVectorの変換
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector);
	static AkVector VecToAkVector(const D3DXVECTOR3 &vec);


	// キャプチャリストへエラーメッセージを送る
	// -------- -------- -------- -------- -------- -------- -------- --------
	void PostMessage(char *szMessage);

private:
	// ゲームオブジェクトの登録(直接は呼び出さない)
	// -------- -------- -------- -------- -------- -------- -------- --------
	void registPtr(void *thisPtr, char *pName = nullptr);
	void registPtr(const AkGameObjectID &ID, char *pName = nullptr);
	void unregistPtr(void *thisPtr);

	// 低レベルI/O
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CAkFilePackageLowLevelIOBlocking m_lowLevelIO;			

	// ゲームオブジェクト
	// -------- -------- -------- -------- -------- -------- -------- --------
	std::unordered_map<void*, WWISE_GAMEOBJ*> m_umapGameObj;		// ゲームオブジェクト
	WWISE_GAMEOBJ *m_pMainListener;									// メインリスナーへのポインタ
	AkGameObjectID m_mainListenerID;								// メインリスナーのID

	// ダミーコールバック
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void dummyCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<int> dummies;
	static CAkLock dummiesLock;

	// ゲームオブジェクトのイベントの終了を確認する処理
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void EndEventCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<WWISE_ENDEVENT_GAMEOBJ *> m_pEndEvents;
};

#endif

/*
// 事前に(bit列で)コールバックフラグを立てる時に使用できるフラグ一覧 - AkCallback.hの27行で定義されています。
AK_EndOfEvent						コールバックは、イベントの終了時にトリガされます。
AK_EndOfDynamicSequenceItem			動的シーケンスアイテムの最後に到達したときに呼び出されるコールバック。
AK_Marker							再生中にマーカーに遭遇したときに呼び出されるコールバック。
AK_Duration							音声の持続時間がサウンドエンジンによって認識されたときにコールバックされます。
AK_SpeakerVolumeMatrix				各フレームでトリガされたコールバック。クライアントがスピーカのボリュームマトリックスを変更できるようにします。
AK_Starvation 						再生がストリームの枯渇のためにフレームをスキップするときに発生するコールバック。
AK_MusicPlaylistSelect				音楽プレイリストコンテナが再生する次のアイテムを選択しなければならないときにコールバックされます。
AK_MusicPlayStarted					「再生」または「シーク」コマンドが実行されたときに呼び出されるコールバック（「シーク」コマンドは、AK :: SoundEngine :: SeekOnEvent（）から発行されます）。インタラクティブミュージック階層のオブジェクトにのみ適用されます。
AK_MusicSyncBeat					ミュージックビートで通知を有効にする。
AK_MusicSyncBar						ミュージックバーで通知を有効にする。
AK_MusicSyncEntry					Music Entry Cueで通知を有効にする。
AK_MusicSyncExit					ミュージックExit Cueで通知を有効にする。
AK_MusicSyncGrid					ミュージックグリッドで通知を有効にする。
AK_MusicSyncUserCue					ミュージックカスタムキューの通知を有効にする。
AK_MusicSyncPoint					ミュージックスイッチの移行同期ポイントで通知を有効にする。
AK_MusicSyncAll						AK_MusicSync登録に関するすべての通知を受信する場合は、このフラグを使用します。
AK_MIDIEvent						MIDIイベントの通知を有効にする。
AK_CallbackBits						すべてのコールバック型のビットマスク。
AK_EnableGetSourcePlayPosition		AK :: SoundEngine :: GetSourcePlayPosition（）で使用する再生位置情報を有効にします。
AK_EnableGetMusicPlayPosition		AK :: MusicEngine :: GetPlayingSegmentInfo（）を介して照会された音楽オブジェクトの再生位置情報を有効にします。
AK_EnableGetSourceStreamBuffering	AK :: SoundEngine :: GetSourceStreamBuffering（）で使用するためのストリームバッファリング情報を有効にします。

*/