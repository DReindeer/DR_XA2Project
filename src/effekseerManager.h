//--------------------------------------------------------------------------------------
//  エフェクシア管理  ( effekseerManager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _EFFEKSEER_MANAGER_H_
#define _EFFEKSEER_MANAGER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスの定義
//--------------------------------------------------------------------------------------
class CEffekseerManager
{
public:
	typedef enum
	{
		TYPE_000 = 0 ,
		TYPE_001 
	} TYPE;

	CEffekseerManager( );										//  コンストラクタ
	~CEffekseerManager( );										//  デストラクタ

	static void			Init( void );							//  初期化
	static void			Uninit( void );							//  終了
	static void			Update( void );							//  更新
	static void			Draw( void );							//  描画

	static void			Create( TYPE type ,
								float fPosX = 0.0f , 
								float fPosY = 0.0f ,
								float fPosZ = 0.0f );			//  生成

	static void			Stop( ::Effekseer::Handle handle );		//  停止

private:
	static ::EffekseerRenderer::Renderer*	m_renderer;			// 描画管理インスタンス
	static ::EffekseerSound::Sound*			m_sound;			// サウンド管理インスタンス
	static ::Effekseer::Manager*			m_manager;			// エフェクト管理用インスタンス

	static ::Effekseer::Effect*				m_effect;			//  
};

#endif