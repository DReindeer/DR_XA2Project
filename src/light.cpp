//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "light.h"
#include "renderer.h"

#include <assert.h>

//================================================================================
// ������
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
// ������
//--------------------------------------------------------------------------------
void CLight::Init()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	for (int n = 0; n < MAX_LIGHT; n++)
	{
		ZeroMemory(&m_light, sizeof(D3DLIGHT9));	// ������
		m_light[n].Type = D3DLIGHT_DIRECTIONAL;		// �^�C�v

		if (n == 0)
		{
			m_light[n].Position.x = 1.f;
			m_light[n].Position.y = 1.f;
			m_light[n].Position.z = 1.f;
			m_light[n].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);	// �f�B���N�V���i�����C�g(���s����)�̐F
			m_light[n].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);	// �A���r�G���g���C�g(����)�̐F
			m_light[n].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}
		else if (n == 1)
		{
			m_light[n].Position.x = 0.f;
			m_light[n].Position.y = -1.f;
			m_light[n].Position.z = 0.f;
			m_light[n].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);	// �f�B���N�V���i�����C�g(���s����)�̐F
			m_light[n].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);	// �A���r�G���g���C�g(����)�̐F
			m_light[n].Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}

		D3DXVECTOR3 vecDir(0.f - m_light[n].Position.x, 0.f - m_light[n].Position.y, 0.f - m_light[n].Position.z);	// �f�B���N�V���i�����C�g�̌���
		D3DXVec3Normalize((D3DXVECTOR3*)&m_light[n].Direction, &vecDir);	// �p�x�͂��̂܂܂ɂ��āA�P�ʃx�N�g���ɒ����𒼂�

		pDevice->SetLight(n, &m_light[n]);					// �C���f�b�N�Xn�ԂɃ��C�g���Z�b�g
		pDevice->LightEnable(n, TRUE);						// �C���f�b�N�Xn�Ԃ̃��C�g�̃X�C�b�`������
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�g�̕\����L���ɂ���
}

D3DXVECTOR3 CLight::GetPos(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "�����̒l���s��");
	return (D3DXVECTOR3)m_light[n].Position;
}

D3DXVECTOR3 CLight::GetVecAtZERO(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "�����̒l���s��");

	D3DXVECTOR3 lightNom;
	D3DXVECTOR3 pos = GetPos(n);
	D3DXVec3Normalize(&lightNom, &pos);
	lightNom *= -1;

	return lightNom;
}

D3DXVECTOR3 CLight::GetDir(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "�����̒l���s��");
	return (D3DXVECTOR3)m_light[n].Direction;
}

D3DXVECTOR4 CLight::GetAmbient(int n)
{
	assert(n > 0 || n < MAX_LIGHT && "�����̒l���s��");
	D3DXVECTOR4 ambient(m_light[n].Ambient.r, m_light[n].Ambient.g, m_light[n].Ambient.b, m_light[n].Ambient.a);
	return ambient;
}
