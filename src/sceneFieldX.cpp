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
#include "sceneFieldX.h"
#include "manager.h"
#include "modelManager.h"
#include "texManager.h"
#include "camera.h"
#include "light.h"
#include "lightcamera.h"
#include "depthShadow.h"
#include "deferredRendering.h"
#include "renderer.h"
#include "utility.h"

//================================================================================
// ����������
//--------------------------------------------------------------------------------
void CSceneFieldX::Init()
{
	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// �p�x
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
	SetSize(D3DXVECTOR3(1.f, 1.f, 1.f));	// �T�C�Y
	m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pIdxBuffer = nullptr;					// �C���f�b�N�X�o�b�t�@
	m_pVtxBuffer = nullptr;					// ���_�o�b�t�@
	m_strFilePass.clear();					// �e�N�X�`���̖��O

	// �e�N���X�̏�����
	CScene3D::Init();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CSceneFieldX::Uninit()
{
	// �e�N���X�̏I��
	CScene3D::Uninit();
}

//================================================================================
// �X�V����
//--------------------------------------------------------------------------------
void CSceneFieldX::Update()
{

}
/*
//================================================================================
// �|���S���`��
//--------------------------------------------------------------------------------
void CSceneFieldX::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���[���h�}�g���N�X�̍쐬
	D3DXMATRIX mtxScl;				// �g��s��
	D3DXMATRIX mtxRot;				// ��]�s��
	D3DXMATRIX mtxPos;				// ���s�s��

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&mtxScl);	//
	D3DXMatrixIdentity(&mtxRot);	//
	D3DXMatrixIdentity(&mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	D3DXVECTOR3 scl = GetScl();

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// ��]�s��
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);			// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̎擾
	CModelManager::MODEL* pModel = CManager::GetModelManager()->GetModel(m_strFilePass);

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �V�F�[�_�[�̐ݒ�
	//--------------------------------------------------------------------------------
	// �J����
	CCamera *pCamera = CManager::GetCamera();

	// ���C�g
	D3DXVECTOR3 lightDirL;
	D3DXVECTOR3 lightDirW = CManager::GetLightCamera()->GetDir();
	D3DXMATRIX mtxWorldInverse;
	D3DXMATRIX mtxWIT;

	D3DXVECTOR3 eye = pCamera->GetPos();
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	m_pShaderSet->pPConstantTable->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));
	// �s�N�Z���V�F�[�_�[�̐ݒ�(�e�N�X�`������)
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "posEyeW", eye, sizeof(D3DXVECTOR3));
	m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "lightDirW", &lightDirW, sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&mtxWorldInverse, nullptr, &m_mtxWorld);
	D3DXVec3TransformNormal(&lightDirL, &lightDirW, &mtxWorldInverse);
	m_pShaderSet->pVConstantTable->SetValue(pDevice, "lightDirL", &lightDirL, sizeof(D3DXVECTOR3));

	m_pShaderSet->pVConstantTable->SetValue(pDevice, "mtxWorld", m_mtxWorld, sizeof(D3DXMATRIX));

	D3DXVECTOR4 lightAmb = CManager::GetLight()->GetAmbient(0);
	m_pShaderSet->pVConstantTable->SetValue(pDevice, "ambent", lightAmb, sizeof(D3DXVECTOR4));

	D3DXMatrixInverse(&mtxWIT, nullptr, &m_mtxWorld);	// �t�s��
	D3DXMatrixTranspose(&mtxWIT, &mtxWIT);				// �]�u�s��
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWIT", &mtxWIT);

	// �}�g���N�X�̐ݒ�
	D3DXMATRIX mtxWVP = (m_mtxWorld * pCamera->GetView() * pCamera->GetProj());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWVP", &mtxWVP);
	D3DXMATRIX mtxWV = (m_mtxWorld * pCamera->GetView());
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxWV", &mtxWV);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "far", pCamera->GetFar());

	// ���C�g�̃}�g���N�X�̐ݒ�
	CLightCamera *pLightCamera = CManager::GetLightCamera();
	D3DXMATRIX mtxView = pLightCamera->GetMtxView();
	D3DXMATRIX mtxProj = pLightCamera->GetMtxProj();
	D3DXMATRIX mtxLightWV = m_mtxWorld * mtxView;
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxLightWV", &mtxLightWV);
	D3DXMATRIX mtxLightWVP = m_mtxWorld * mtxView * mtxProj;
	m_pShaderSet->pVConstantTable->SetMatrix(pDevice, "mtxLightWVP", &mtxLightWVP);
	m_pShaderSet->pVConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);

	// �[�x�}�b�v�̎擾
	LPDIRECT3DTEXTURE9 depthMap = CManager::GetDepthShadow()->GetTexture();
	UINT sampleIndex = m_pShaderSet->pPConstantTable->GetSamplerIndex("depthSampler");
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetTexture(sampleIndex, depthMap);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// �f�o�C�X�Ƀ}�e���A����ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);

		// �s�N�Z���V�F�[�_�[�̐ݒ�
		//------------------------------------------------------------------------
		float power = pMat->MatD3D.Power + 50.f;
		m_pShaderSet->pPConstantTable->SetFloat(pDevice, "speclarPower", power);
		m_pShaderSet->pPConstantTableNoTex->SetFloat(pDevice, "speclarPower", power);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "resolution", D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), sizeof(D3DXVECTOR2));

		D3DXVECTOR3 modelEmissive(pMat[nCntMat].MatD3D.Emissive.r, pMat[nCntMat].MatD3D.Emissive.g, pMat[nCntMat].MatD3D.Emissive.b);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelEmissive", modelEmissive, sizeof(D3DXVECTOR3));

		D3DXVECTOR4 modelDiffuse = D3DXVECTOR4(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, pMat[nCntMat].MatD3D.Diffuse.a);
		m_pShaderSet->pPConstantTable->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));
		m_pShaderSet->pPConstantTableNoTex->SetValue(pDevice, "modelDiffuse", modelDiffuse, sizeof(D3DXVECTOR4));

		m_pShaderSet->pPConstantTable->SetFloat(pDevice, "lightFar", pLightCamera->GetFar());
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			//------------------------------------------------------------------------
			pDevice->SetPixelShader(m_pShaderSet->pPixelShader);
		}
		else
		{// �e�N�X�`�����Ȃ����null
			pDevice->SetTexture(0, nullptr);
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			//------------------------------------------------------------------------
			pDevice->SetPixelShader(m_pShaderSet->pPixelShaderNoTex);
		}
		// �`��
		pModel->pMeshClone->DrawSubset(nCntMat);
	}

	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(sampleIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}
*/

//--------------------------------------------------------------------------------
// �f�t�@�[�h�����_�����O�p
//--------------------------------------------------------------------------------
void CSceneFieldX::DrawDefferdRendering()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���[���h�}�g���N�X�̍쐬
	D3DXMATRIX mtxScl;						// �g��s��
	D3DXMATRIX mtxRot;						// ��]�s��
	D3DXMATRIX mtxPos;						// ���s�s��

	D3DXMatrixIdentity(&m_mtxWorld);// 
	D3DXMatrixIdentity(&mtxScl);	// 
	D3DXMatrixIdentity(&mtxRot);	// 
	D3DXMatrixIdentity(&mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	D3DXVECTOR3 scl = GetScl();

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);					// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);	// ��]�s��
	D3DXMatrixTranslation(&mtxPos, m_pos.x, m_pos.y, m_pos.z);			// ���s�s��

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxPos);

	// ���f���̎擾
	CModelManager::MODEL* pModel = CManager::GetModelManager()->GetModel(m_strFilePass);

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �f�t�@�[�h�����_�����O�̎擾
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	pDevice->GetMaterial(&matDef);

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// �f�o�C�X�Ƀ}�e���A����ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			//------------------------------------------------------------------------
			pDefferdRendering->SetPXShader(&pMat[nCntMat].MatD3D);
		}
		else
		{// �e�N�X�`�����Ȃ����null
			pDevice->SetTexture(0, nullptr);
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			//------------------------------------------------------------------------
			pDefferdRendering->SetNoTexturePXShader(&pMat[nCntMat].MatD3D);
		}
		// �`��
		pModel->pMeshClone->DrawSubset(nCntMat);
	}

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//================================================================================
// �t�B�[���h���̎O�p�|���S���̒��Ƀv���C���[�����邩���ׂ�
//--------------------------------------------------------------------------------
bool CSceneFieldX::GetHeight(D3DXVECTOR3 pos, float* y, FIELD_POLY *pPoly)
{
	D3DXVECTOR3 nom;

	// �ǂ̎O�p�|���S�����ɂ��邩�m�F
	for (int i = 0; i < static_cast<int>(m_vecFieldPoly.size()); i++)
	{
		// �O�p�|���S�����ɂ��Ȃ���΂�蒼��
		if (TriangleCloss(pos, m_vecFieldPoly.at(i), &nom) == false) { continue; };

		// �|���S�����ɂ�����
		*pPoly = *m_vecFieldPoly.at(i);
		if (y)*y = m_vecFieldPoly.at(i)->p[0].y - ((pos.x - m_vecFieldPoly.at(i)->p[0].x) * nom.x + (pos.z - m_vecFieldPoly.at(i)->p[0].z) * nom.z) / nom.y;
		return true;
	}

	// �����ɂ����܂���ł����I
	if (y)*y = 0.f;
	return false;
}

//================================================================================
// �����擾
//--------------------------------------------------------------------------------
bool CSceneFieldX::TriangleCloss(D3DXVECTOR3 pos, CSceneFieldX::FIELD_POLY *fieldPoly, D3DXVECTOR3 *nom)
{
	// �O�p�|���S�����ɂ��邩�m�F
	D3DXVECTOR3 v10 = fieldPoly->p[1] - fieldPoly->p[0];
	D3DXVECTOR3 v02 = fieldPoly->p[0] - fieldPoly->p[2];
	D3DXVECTOR3 v21 = fieldPoly->p[2] - fieldPoly->p[1];
	D3DXVECTOR3 vpxp0 = pos - fieldPoly->p[0];
	D3DXVECTOR3 vpxp1 = pos - fieldPoly->p[1]; 
	D3DXVECTOR3 vpxp2 = pos - fieldPoly->p[2];
	D3DXVECTOR3 vc0;

	D3DXVec3Cross(&vc0, &v10, &vpxp1);	// v10
	if (vc0.y < 0.f)return false;

	D3DXVec3Cross(&vc0, &v02, &vpxp0);	// v02
	if (vc0.y < 0.f)return false;

	D3DXVec3Cross(&vc0, &v21, &vpxp2);	// v21
	if (vc0.y < 0.f)return false;

	// �O�p�|���S���̓����ł�
	D3DXVECTOR3 vv1 = v02 - v21;
	D3DXVECTOR3 vv2 = v10 - v02;

	// �ʖ@��
	D3DXVec3Cross(nom, &vv1, &vv2);
	D3DXVec3Normalize(nom, nom);
	return true;
}

//================================================================================
// �_���O�ɂ��鎞�Ɉ�ԋ߂��O�p�|���S���̕ӂ�T��
//--------------------------------------------------------------------------------
bool CSceneFieldX::TriangleNearLine(D3DXVECTOR3 pos, CSceneFieldX::FIELD_POLY *fieldPoly, D3DXVECTOR3 *outPosS, D3DXVECTOR3 *outPosG)
{
	// �O�p�|���S�����ɂ��邩�m�F
	D3DXVECTOR3 v10 = fieldPoly->p[1] - fieldPoly->p[0];
	D3DXVECTOR3 v02 = fieldPoly->p[0] - fieldPoly->p[2];
	D3DXVECTOR3 v21 = fieldPoly->p[2] - fieldPoly->p[1];
	D3DXVECTOR3 vpxp0 = pos - fieldPoly->p[0];
	D3DXVECTOR3 vpxp1 = pos - fieldPoly->p[1];
	D3DXVECTOR3 vpxp2 = pos - fieldPoly->p[2];
	D3DXVECTOR3 vc0;

	D3DXVec3Cross(&vc0, &v10, &vpxp1);	// v10
	if (vc0.y < 0.f)
	{
		*outPosS = fieldPoly->p[0];
		*outPosG = fieldPoly->p[1];
		return true;
	}

	D3DXVec3Cross(&vc0, &v02, &vpxp0);	// v02
	if (vc0.y < 0.f)
	{
		*outPosS = fieldPoly->p[2];
		*outPosG = fieldPoly->p[0];
		return true;
	}

	D3DXVec3Cross(&vc0, &v21, &vpxp2);	// v21
	if (vc0.y < 0.f)
	{
		*outPosS = fieldPoly->p[1];
		*outPosG = fieldPoly->p[2];
		return true;
	}
	*outPosS = *outPosG = D3DXVECTOR3();
	return false;
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneFieldX *CSceneFieldX::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// CSceneFieldX�̐���
	CSceneFieldX *pFieldX = new CSceneFieldX(pry);
	pFieldX->Init();

	pFieldX->SetPos(pos);
	pFieldX->SetRot(rot);
	pFieldX->SetScl(scl);
	pFieldX->SetFilePass(strFilePass);

	//---------------------
	// ���f���̃Z�b�g
	//---------------------
	CModelManager *modelManager = CManager::GetModelManager();
	if (modelManager->SetModel(strFilePass))
	{
		// �e�N�X�`���̃Z�b�g
		D3DXMATERIAL* pMat = CManager::GetModelManager()->GetModelMat(strFilePass);
		for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(strFilePass); nCntMat++)
		{
			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (pMat[nCntMat].pTextureFilename != nullptr && lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{
				CManager::GetTexManager()->SetTex(pMat[nCntMat].pTextureFilename);
			}
			else { pMat[nCntMat].pTextureFilename = nullptr; }
		}

		//---------------------------------------------
		// ���f�����_�̎擾
		//---------------------------------------------
		LPD3DXMESH pMesh = nullptr;
		pMesh = modelManager->GetModelMesh(strFilePass);

		// Vtx�o�b�t�@�����b�N����
		BYTE *pVtx = nullptr;							// ���_�o�b�t�@
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
		DWORD FVF = pMesh->GetFVF();					// ���_�t�H�[�}�b�g���擾����B
		DWORD VertexSize = D3DXGetFVFVertexSize(FVF);	// ���_�̃T�C�Y���擾����B

		// ���f���̒��_�̍ő�l�A�ŏ��l�̎擾
		D3DXVECTOR3 Vertex;
		D3DXVECTOR3 min = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 max = D3DXVECTOR3(0.f, 0.f, 0.f);
		for (int cntVtx = 0; cntVtx < static_cast<int>(pMesh->GetNumVertices()); cntVtx++)
		{
			Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// ���_���W���擾����B
			pFieldX->m_centerPos += Vertex;						// �S���_�̍��W�𑫂�����
			min.x = RETURN_MIN(min.x, Vertex.x);		// X�ŏ��l
			max.x = RETURN_MAX(max.x, Vertex.x);		// X�ő�l
			min.y = RETURN_MIN(min.y, Vertex.y);		// Y�ŏ��l
			max.y = RETURN_MAX(max.y, Vertex.y);		// Y�ő�l
			min.z = RETURN_MIN(min.z, Vertex.z);		// Z�ŏ��l
			max.z = RETURN_MAX(max.z, Vertex.z);		// Z�ő�l
			pVtx += VertexSize;							// �i�߂�
		}
		// ��,����,���s�̎擾
		pFieldX->SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));
		// Vtx�o�b�t�@�̃A�����b�N
		pMesh->UnlockVertexBuffer();

		pFieldX->m_centerPos /= static_cast<FLOAT>(pMesh->GetNumVertices());

		// Vtx�o�b�t�@�����b�N����(�A�h���X��擪�ɖ߂�)
		pVtx = nullptr;
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
		pMesh->UnlockVertexBuffer();

		// Idx�o�b�t�@�����b�N����
		WORD *pIdx = nullptr;
		pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
		WORD IdxNum[3];	//���_�ԍ�(�C���f�b�N�X)

		FIELD_POLY *fieldPoly;

		// �|���S�����_�����������[�v
		int numFace = static_cast<int>(pMesh->GetNumFaces());
		for (int i = 0; i < numFace; i++)
		{
			IdxNum[0] = pIdx[i * 3 + 0]; // ���_1
			IdxNum[1] = pIdx[i * 3 + 1]; // ���_2
			IdxNum[2] = pIdx[i * 3 + 2]; // ���_3

										 // �ʏ��ɋl�߂�
			fieldPoly = new FIELD_POLY;
			XMODEL_VTX vtx = *(XMODEL_VTX*)(pVtx + IdxNum[0] * VertexSize);
			fieldPoly->p[0] = vtx.pos * CManager::GetMasterScale();
			fieldPoly->n[0] = vtx.nom;
			fieldPoly->uv[0] = vtx.uv;
			vtx = *(XMODEL_VTX*)(pVtx + IdxNum[1] * VertexSize);
			fieldPoly->p[1] = vtx.pos * CManager::GetMasterScale();
			fieldPoly->n[1] = vtx.nom;
			fieldPoly->uv[1] = vtx.uv;
			vtx = *(XMODEL_VTX*)(pVtx + IdxNum[2] * VertexSize);
			fieldPoly->p[2] = vtx.pos * CManager::GetMasterScale();
			fieldPoly->n[2] = vtx.nom;
			fieldPoly->uv[2] = vtx.uv;

			pFieldX->m_vecFieldPoly.push_back(fieldPoly);
		}
		// Idx�o�b�t�@���A�����b�N
		pMesh->UnlockIndexBuffer();
		// ���T�C�Y
		pFieldX->m_vecFieldPoly.shrink_to_fit();
	}

	return pFieldX;
}
