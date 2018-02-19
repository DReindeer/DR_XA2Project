//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "light.h"
#include "renderer.h"

#include <assert.h>

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
CLight::CLight()
{
	m_light[0].Position = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_light[0].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_light[0].Ambient = D3DXCOLOR(2.f, 2.f, 2.f, 1.f);

	m_light[1].Position = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_light[1].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_light[1].Ambient = D3DXCOLOR(2.f, 2.f, 2.f, 1.f);
}

//================================================================================
// 初期化
//--------------------------------------------------------------------------------
void CLight::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int n = 0; n < MAX_LIGHT; n++)
	{
		ZeroMemory(&m_light, sizeof(D3DLIGHT9));	// 初期化
		m_light[n].Type = D3DLIGHT_DIRECTIONAL;		// タイプ

		if (n == 0)
		{
			m_light[n].Position.x = 1.f;
			m_light[n].Position.y = 1.f;
			m_light[n].Position.z = 1.f;
			m_light[n].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);	// ディレクショナルライト(平行光源)の色
			m_light[n].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);	// アンビエントライト(環境光)の色
			m_light[n].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}
		else if (n == 1)
		{
			m_light[n].Position.x = 0.f;
			m_light[n].Position.y = -1.f;
			m_light[n].Position.z = 0.f;
			m_light[n].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);	// ディレクショナルライト(平行光源)の色
			m_light[n].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);	// アンビエントライト(環境光)の色
			m_light[n].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}

		D3DXVECTOR3 vecDir(0.f - m_light[n].Position.x, 0.f - m_light[n].Position.y, 0.f - m_light[n].Position.z);	// ディレクショナルライトの向き
		D3DXVec3Normalize((D3DXVECTOR3*)&m_light[n].Direction, &vecDir);	// 角度はそのままにして、単位ベクトルに長さを直す

		pDevice->SetLight(n, &m_light[n]);					// インデックスn番にライトをセット
		pDevice->LightEnable(n, TRUE);						// インデックスn番のライトのスイッチを入れる
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライトの表示を有効にする
}

D3DXVECTOR3 CLight::GetPos(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "引数の値が不正");
	return (D3DXVECTOR3)m_light[n].Position;
}

D3DXVECTOR3 CLight::GetVecAtZERO(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "引数の値が不正");

	D3DXVECTOR3 lightNom;
	D3DXVECTOR3 pos = GetPos(n);
	D3DXVec3Normalize(&lightNom, &pos);
	lightNom *= -1;

	return lightNom;
}

D3DXVECTOR3 CLight::GetDir(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "引数の値が不正");
	return (D3DXVECTOR3)m_light[n].Direction;
}

D3DXVECTOR4 CLight::GetAmbient(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "引数の値が不正");
	D3DXVECTOR4 ambient(m_light[n].Ambient.r, m_light[n].Ambient.g, m_light[n].Ambient.b, m_light[n].Ambient.a);
	return ambient;
}
