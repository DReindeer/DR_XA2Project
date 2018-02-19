//--------------------------------------------------------------------------------------
//  エフェクシア管理   ( camera.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "effekseerManager.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
::EffekseerRenderer::Renderer*	CEffekseerManager::m_renderer = NULL;	//  描画管理インスタンス
::EffekseerSound::Sound*		CEffekseerManager::m_sound = NULL;		//  サウンド管理インスタンス
::Effekseer::Manager*			CEffekseerManager::m_manager = NULL;	//  エフェクト管理用インスタンス
::Effekseer::Effect*			CEffekseerManager::m_effect = NULL;		//  

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
CEffekseerManager::CEffekseerManager( )
{

}

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
CEffekseerManager::~CEffekseerManager( )
{

}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の初期化処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer( )->GetDevice( );

	//IXAudio2*		  pXAudio = CManager::GetSound( )->GetXAudio( );

	// 描画管理インスタンスの生成
	m_renderer = ::EffekseerRendererDX9::Renderer::Create( pDevice , 2000 );

	// サウンド管理インスタンスの生成
	//m_sound = ::EffekseerSound::Sound::Create( pXAudio , 16 , 16 );

	// エフェクト管理用インスタンスの生成
	m_manager = ::Effekseer::Manager::Create( 2000 );

	// 描画方法の指定、独自に拡張しない限り定形文です。
	m_manager->SetSpriteRenderer( m_renderer->CreateSpriteRenderer( ) );
	m_manager->SetRibbonRenderer( m_renderer->CreateRibbonRenderer( ) );
	m_manager->SetRingRenderer( m_renderer->CreateRingRenderer( ) );
	m_manager->SetTrackRenderer( m_renderer->CreateTrackRenderer( ) );
	m_manager->SetModelRenderer( m_renderer->CreateModelRenderer( ) );

	// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
	m_manager->SetTextureLoader( m_renderer->CreateTextureLoader( ) );
	m_manager->SetModelLoader( m_renderer->CreateModelLoader( ) );

	// サウンド再生用インスタンスの指定
	//m_manager->SetSoundPlayer( m_sound->CreateSoundPlayer( ) );

	// サウンドデータの読込方法の指定(圧縮フォーマット、パッケージ等から読み込む場合拡張する必要があります。)
	//m_manager->SetSoundLoader( m_sound->CreateSoundLoader( ) );

	// 座標系の指定( RHで右手系、LHで左手系 )
	//m_manager->SetCoordinateSystem( ::Effekseer::CoordinateSystem::LH );

	// 投影行列を設定
	m_renderer->SetProjectionMatrix( ::Effekseer::Matrix44( ).PerspectiveFovLH( D3DX_PI / 3.0f ,
									 ( float )SCREEN_WIDTH / SCREEN_HEIGHT , 1.0f , 50000.0f ) );

	// カメラ行列を設定
	m_renderer->SetCameraMatrix( ::Effekseer::Matrix44( ).LookAtLH( ::Effekseer::Vector3D( 0.0f , 0.0f , -10.0f ) ,
																	::Effekseer::Vector3D( 0.0f , 0.0f , 0.0f ) ,
																	::Effekseer::Vector3D( 0.0f, 1.0f, 0.0f ) ) );

	// エフェクトの読込
	m_effect = Effekseer::Effect::Create( m_manager , ( const EFK_CHAR* )L"data/effekseer/bullet01.efk" );
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の終了処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Uninit( void )
{
	if( m_effect != NULL )
	{
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		ES_SAFE_RELEASE( m_effect );
	}

	if( m_manager != NULL )
	{
		// エフェクト管理用インスタンスを破棄
		m_manager->Destroy( );
	}

	if( m_sound != NULL )
	{
		// サウンド用インスタンスを破棄
		m_sound->Destroy( );
	}

	if( m_renderer != NULL )
	{
		// 描画用インスタンスを破棄
		m_renderer->Destroy( );
	}
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の更新処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Update( void )
{
	//  カメラ情報の取得
	CCamera* pCamera = CGame::GetCamera( );

	// 投影行列を設定
	m_renderer->SetProjectionMatrix( ::Effekseer::Matrix44( ).PerspectiveFovLH( D3DX_PI / 3.0f ,
									 ( float )SCREEN_WIDTH / SCREEN_HEIGHT , 1.0f , 50000.0f ) );

	//  カメラクラスのポインタがある場合
	if( pCamera != NULL )
	{
		//  カメラの視点、注視点、上向き方向
		::Effekseer::Vector3D posEye( pCamera->GetCameraPosEye( ).x , pCamera->GetCameraPosEye( ).y , pCamera->GetCameraPosEye( ).z );
		::Effekseer::Vector3D posAt( pCamera->GetCameraPosAt( ).x , pCamera->GetCameraPosAt( ).y , pCamera->GetCameraPosAt( ).z );
		::Effekseer::Vector3D vecUp( 0.0f, 1.0f, 0.0f );
		
		// カメラ行列を設定
		m_renderer->SetCameraMatrix( ::Effekseer::Matrix44( ).LookAtLH( posEye , posAt , vecUp ) );
	}

	// 3Dサウンド用リスナー設定の更新
	//m_sound->SetListener( pos , posAt , vecUp );

	// 全てのエフェクトの更新
	m_manager->Update( );
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の描画処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Draw( void )
{
	if( m_renderer != NULL )
	{
		//  レンダリング開始
		m_renderer->BeginRendering( );

		if( m_manager != NULL )
		{
			//  描画
			m_manager->Draw( );
		}

		//  レンダリング終了
		m_renderer->EndRendering( );
	}
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の描画処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Create( TYPE type , float fPosX , float fPosY , float fPosZ )
{
	::Effekseer::Handle handle;

	switch( type )
	{
		case TYPE_000:
		{
			break;
		}
		case TYPE_001:
		{
			break;
		}
	}

	// エフェクトの再生
	handle = m_manager->Play( m_effect, fPosX , fPosY + 5.0f , fPosZ );
	m_manager->SetRotation( handle , 0.0f , 0.0f , 0.0f );
	m_manager->SetScale( handle , 10 , 10 , 10 );
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の描画処理
//--------------------------------------------------------------------------------------
void CEffekseerManager::Stop( ::Effekseer::Handle handle )
{
	if( m_manager != NULL )
	{
		// エフェクト管理用インスタンスを破棄
		m_manager->StopEffect( handle );
	}
}