//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "scene3D.h"
#include "sceneField.h"
#include "sceneSkydome.h"
#include "manager.h"
#include "modeTitle.h"
#include "modeGame.h"
#include "texManager.h"
#include "camera.h"
#include "light.h"
#include "renderer.h"

static const int FACE_X = 3;
static const int FACE_Z = 3;

float meshPosY[FACE_X + 1][FACE_Z + 1] =
{
	{ 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 3.f, 0.f },
	{ 0.f, 0.f, 3.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f }
};

//================================================================================
// ����������
//--------------------------------------------------------------------------------
void CSceneField::Init( void )
{
	// �����Ńt�@�C���ǂݍ��� & �ʂ̐��̎擾
	m_numFaceX = FACE_X;
	m_numFaceZ = FACE_Z;

	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// �p�x
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// �T�C�Y
	m_vecVtxNom.clear();					// ���_�@��
	m_numVtx = 0;							// Vtx��
	m_numIndex = 0;							// �C���f�b�N�X��
	m_pIdxBuffer = nullptr;					// �C���f�b�N�X�o�b�t�@
	m_pVtxBuffer = nullptr;					// ���_�o�b�t�@
	m_strFilePass.clear();					// �e�N�X�`���̖��O
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CSceneField::Uninit()
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if(m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// �e�N���X�̏I��
	CScene3D::Uninit();
}

//================================================================================
// �X�V����
//--------------------------------------------------------------------------------
void CSceneField::Update( void )
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	WORD *pIdx;			// �o�b�t�@�����b�N����
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;
	int id = 0;
	for (int nCntZ = 0; nCntZ < m_numFaceZ; nCntZ++)
	{
		if (nCntZ != 0)	// ��ԏ��߂���Ȃ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1));
			id = ((m_numFaceX + 1) * (nCntZ + 1));
			nCnt++;
		}
		// 2�_�ł�(��O, ��)
		pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * nCntZ);
		id = ((m_numFaceX + 1) * nCntZ);
		nCnt++;
		pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1));
		id = ((m_numFaceX + 1) * (nCntZ + 1));
		nCnt++;

		for (int nCntX = 0; nCntX < m_numFaceX; nCntX++)
		{
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * nCntZ + nCntX + 1);
			id = ((m_numFaceX + 1) * nCntZ + nCntX + 1);
			nCnt++;
			// 2�_�ł�(��O, ��)
			pIdx[nCnt] = static_cast<WORD>((m_numFaceX + 1) * (nCntZ + 1) + nCntX + 1);
			id = ((m_numFaceX + 1) * (nCntZ + 1) + nCntX + 1);
			nCnt++;
		}
		if (nCntZ != m_numFaceZ - 1)	// �Ōザ��Ȃ�������
		{
			// ��_�ł�
			pIdx[nCnt] = static_cast<WORD>(m_numFaceX + (nCntZ * (m_numFaceX + 1)));
			id = (m_numFaceX + (nCntZ * (m_numFaceX + 1)));
			nCnt++;
		}
	}
	//���b�N�̉���
	m_pIdxBuffer->Unlock();

	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			// ���W
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].pos = m_vecVtxPos.at(nCntZ * (m_numFaceX + 1) + nCntX);

			// �F
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].col = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

			// �e�N�X�`�����W
			pVtx[nCntZ * (m_numFaceX + 1) + nCntX].tex = D3DXVECTOR2(1.f * nCntX, 1.f * nCntZ);
		}
	}

	for (int i = 0, max = m_vecVtxNom.size(); i < max; i++)
	{
		// ���_�@���̐ݒ�
		pVtx[i].nom = m_vecVtxNom[i];
	}
	// ���b�N�̉���
	m_pVtxBuffer->Unlock();
}

//================================================================================
// �|���S���`��
//--------------------------------------------------------------------------------
void CSceneField::Draw( void )
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMATRIX mtxScl;					// �g��s��
	D3DXMATRIX mtxRot;					// ��]�s��
	D3DXMATRIX mtxPos;					// ���s�s��

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 scl = GetScl();

	// ���[���h�}�g���N�X�̍쐬
	D3DXMatrixIdentity(&m_mtxWorld);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&mtxScl);		//
	D3DXMatrixIdentity(&mtxRot);		//
	D3DXMatrixIdentity(&mtxPos);		//

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);				// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);	// 
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);			// ���s�ړ��s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �C���f�b�N�X�ԍ����f�o�C�X�ɐݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	if(!m_strFilePass.empty())
	{
		pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(m_strFilePass));
	}else {pDevice->SetTexture(0, nullptr);}

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^ �X�g���[���Ƀo�C���h����
	pDevice->SetStreamSource(0,	m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// �v���~�e�B�u�̐ݒ�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NUM_POLYGON, 0, m_numIndex - 2);
}

//--------------------------------------------------------------------------------
// ���_�쐬
//--------------------------------------------------------------------------------
HRESULT CSceneField::SetVtx()
{
	m_numVtx = (m_numFaceX + 1) * (m_numFaceZ + 1);							// ���_��
	m_numIndex = (2 + 2 * m_numFaceX) * m_numFaceZ + (m_numFaceZ - 1) * 2;	// �C���f�b�N�X��(�|���S���`�掞�ɕK�v�Ȓ��_��)

// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// D3Device�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	//---------------------
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_numVtx,			// �������̃T�C�Y
		D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
		D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
		&m_pVtxBuffer,							// 
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "3D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_numIndex,		// �o�b�t�@��
		D3DUSAGE_WRITEONLY,			// �g�p�t���O
		D3DFMT_INDEX16,				// �K�v�ȃo�b�t�@��
		D3DPOOL_MANAGED,			// �������̊Ǘ����@
		&m_pIdxBuffer,
		nullptr)))
	{
		// ���s������G���[��Ԃ�
		MessageBox(nullptr, "IdxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 1�u���b�N�̕��ƍ���
	D3DXVECTOR3 size = GetSize();
	m_faceSize.x = size.x / m_numFaceX;
	m_faceSize.y = size.z / m_numFaceZ;

	// ���W
	D3DXVECTOR3 pos = GetPos();

	// ���W�̐ݒ�
	for(int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for(int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			// ���W
			m_vecVtxPos.push_back(D3DXVECTOR3(m_faceSize.x * nCntX - size.x * 0.5f + pos.x,
									meshPosY[nCntZ][nCntX] + pos.y,
									m_faceSize.y * (nCntZ)-size.z * 0.5f + pos.z));
		}
	}
	
	// ���_�@���̐ݒ�
	for (int nCntZ = 0; nCntZ <= m_numFaceZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_numFaceX; nCntX++)
		{
			if (nCntZ == 0 || nCntZ == m_numFaceZ || nCntX == 0 || nCntX == m_numFaceX)
			{
				m_vecVtxNom.push_back(D3DXVECTOR3(0.f, 1.f, 0.f));
				continue;
			}

			// ���_�@�����v�Z
			D3DXVECTOR3 dirX, dirZ;
			D3DXVECTOR3 nomX, nomZ;

			int prevX = (nCntZ) * (m_numFaceX + 1) + (nCntX - 1);
			int nextX = (nCntZ) * (m_numFaceX + 1) + (nCntX + 1);
			int prevZ = (nCntZ - 1) * (m_numFaceX + 1) + (nCntX);
			int nextZ = (nCntZ + 1) * (m_numFaceX + 1) + (nCntX);

			dirX = m_vecVtxPos[nextX] - m_vecVtxPos[prevX];
			dirZ = m_vecVtxPos[prevZ] - m_vecVtxPos[nextZ];
			nomX.x = -dirX.y;
			nomX.y = dirX.x;
			nomX.z = 0.f;
			nomZ.x = 0.f;
			nomZ.y = dirZ.z;
			nomZ.z = -dirZ.y;

			D3DXVECTOR3 nom = nomX + nomZ;
			D3DXVec3Normalize(&nom, &nom);
			m_vecVtxNom.push_back(nom);
		}
	}

	// ���b�N�̉���
	m_pVtxBuffer->Unlock();

	return NOERROR;
}

float CSceneField::GetHeight(D3DXVECTOR3 pos, bool *hit)
{
	D3DXVECTOR3 v12, v20, v01;
	D3DXVECTOR3 nom;
	D3DXVECTOR3 p0, p1, p2, p3;
	float y;

	// 1�u���b�N�̕��ƍ���
	D3DXVECTOR3 size = GetSize();
	m_faceSize.x = size.x / m_numFaceX;
	m_faceSize.y = size.z / m_numFaceZ;

	// �l�p�|���S���̍ŏ��̒��_�̃C���f�b�N�X�����߂�
	int iz = static_cast<int>((pos.z + (size.z * 0.5f)) / (m_faceSize.y));
	int ix = static_cast<int>((pos.x + (size.x * 0.5f)) / (m_faceSize.x));
	int i = iz * (m_numFaceX + 1) + ix;
	if ((pos.x + (size.x * 0.5f)) < 0 || ix > m_numFaceX - 1 || (pos.z + (size.z * 0.5f)) < 0 || iz > m_numFaceZ - 1)
	{
		if (hit)*hit = false;
		return 0.f;
	}

	// �l�p�|���S���̊e���_�����߂�
	p0 = m_vecVtxPos[i];
	p1 = m_vecVtxPos[i + 1];
	p2 = m_vecVtxPos[i + (m_numFaceX + 1)];
	p3 = m_vecVtxPos[i + (m_numFaceX + 1) + 1];

	D3DXVECTOR3 vtxVec = p1 - p2; 
	vtxVec.y = 0.f;
	D3DXVECTOR3 posVec = pos - p2;
	posVec.y = 0.f;
	D3DXVECTOR3 cross(0.f, 0.f, 0.f);
	D3DXVec3Cross(&cross, &vtxVec, &posVec);

	if (cross.y >= 0.f)
	{
		TriangleCloss(p1, p2, p0, &nom);
		y = p1.y - ((pos.x - p1.x) * nom.x + (pos.z - p1.z) * nom.z) / nom.y;
		if (hit)*hit = true;
		return y;
	}
	else 
	{
		TriangleCloss(p1, p3, p2, &nom);
		y = p1.y - ((pos.x - p1.x) * nom.x + (pos.z - p1.z) * nom.z) / nom.y;
		if (hit)*hit = true;
		return y;
	}
}

bool CSceneField::TriangleCloss(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 *nom)
{
	D3DXVECTOR3 v01 = p0 - p1;
	D3DXVECTOR3 v12 = p1 - p2;
	D3DXVECTOR3 v20 = p2 - p0;

	// �ʖ@��
	D3DXVECTOR3 vv1 = v12 - v20;
	D3DXVECTOR3 vv2 = v01 - v12;
	D3DXVec3Cross(nom, &vv1, &vv2);
	D3DXVec3Normalize(nom, nom);
	return true;
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneField *CSceneField::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &size, const std::string &strFilePass)
{
	// CSceneField�̐���
	CSceneField *pSceneField;
	pSceneField = new CSceneField();
	pSceneField->Init();

	pSceneField->SetPos(pos);
	pSceneField->SetRot(rot);
	pSceneField->SetSize(size);
	pSceneField->m_strFilePass = strFilePass;

	// ���_�ݒ�
	pSceneField->SetVtx();
	
	// �e�N�X�`���̃Z�b�g
	CManager::GetTexManager()->SetTex(strFilePass);

	return pSceneField;
}
