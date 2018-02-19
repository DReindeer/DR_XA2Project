//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "modelManager.h"
#include "texManager.h"
#include "sceneModel.h"
#include "camera.h"
#include "light.h"
#include "lightcamera.h"
#include "depthShadow.h"
#include "deferredRendering.h"

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CSceneModel::Init()
{
	m_pUseModelManager = nullptr;
	m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));		// ���W
	SetRot(D3DXVECTOR3(0.f, 0.f, 0.f));		// ��]��
	SetScl(D3DXVECTOR3(1.f, 1.f, 1.f));		// �g�嗦
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CSceneModel::Uninit()
{

}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CSceneModel::Update()
{

}

/*
//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CSceneModel::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&m_mtxScl);	//
	D3DXMatrixIdentity(&m_mtxRot);	//
	D3DXMatrixIdentity(&m_mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	D3DXVECTOR3 scl = GetScl();

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&m_mtxScl, scl.x, scl.y, scl.z);	// �g��s��
	m_mtxRot = GetMatrixRot(this);
	m_mtxPos = GetMatrixPos(this);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̎擾
	CModelManager::MODEL* pModel = m_pUseModelManager->GetModel(m_strFilePass);

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

//	D3DXVECTOR4 lightAmb = CManager::GetLight()->GetAmbient(0);
//	m_pShaderSet->pVConstantTable->SetValue(pDevice, "ambent", lightAmb, sizeof(D3DXVECTOR4));

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

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	pDevice->GetMaterial(&matDef);

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDevice->SetVertexShader(m_pShaderSet->pVertexShader);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(m_strFilePass); nCntMat++)
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
		// �[�x�}�b�v�̎擾
		DepthShadow *pDepthShadow = CManager::GetDepthShadow();
		UINT sampleIndex = m_pShaderSet->pPConstantTable->GetSamplerIndex("depthSampler");
		if (pDepthShadow)
		{
			LPDIRECT3DTEXTURE9 depthMap = pDepthShadow->GetTexture();
			pDevice->SetTexture(sampleIndex, depthMap);
		}
		else
		{
			pDevice->SetTexture(sampleIndex, nullptr);
		}
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

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}
*/

//--------------------------------------------------------------------------------
// �f�B�t�@�[�h�����_�����O�p
//--------------------------------------------------------------------------------
void CSceneModel::DrawDefferdRendering()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);//
	D3DXMatrixIdentity(&m_mtxScl);	//
	D3DXMatrixIdentity(&m_mtxRot);	//
	D3DXMatrixIdentity(&m_mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	D3DXVECTOR3 scl = GetScl();

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&m_mtxScl, scl.x, scl.y, scl.z);	// �g��s��
	m_mtxRot = GetMatrixRot(this);
	m_mtxPos = GetMatrixPos(this);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̎擾
	CModelManager::MODEL* pModel = m_pUseModelManager->GetModel(m_strFilePass);

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DXMATERIAL* pMat = pModel->pMat;
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �f�t�@�[�h�����_�����O�̎擾
	DefferdRendering *pDefferdRendering = CManager::GetDefferdRendering();

	// ���_�V�F�[�_�[�̐ݒ�
	//------------------------------------------------------------------------
	pDevice->SetVertexDeclaration(m_pShaderSet->pVertexDeclaration);
	pDefferdRendering->SetVXShader(m_mtxWorld);

	// �~�b�v�}�b�v
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(m_strFilePass); nCntMat++)
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

	// �~�b�v�}�b�v����
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�

	pDevice->SetPixelShader(nullptr);
	pDevice->SetVertexShader(nullptr);
}

//================================================================================
// ��]�s��
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixRot(CSceneModel *pParts)
{
	D3DXMATRIX mtxrot;
	D3DXMatrixIdentity(&mtxrot);	// �P�ʍs��
	if (pParts->m_pParent == nullptr)
	{
		D3DXMatrixRotationYawPitchRoll(&mtxrot, m_rot.y, m_rot.x, m_rot.z);
		return mtxrot;
	}

	D3DXMatrixRotationYawPitchRoll(&mtxrot, pParts->m_baseRot.y, pParts->m_baseRot.x, pParts->m_baseRot.z);
	return mtxrot * GetMatrixRot(pParts->m_pParent);
}

//================================================================================
// ���s�s��
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixPos(CSceneModel *pParts)
{
	D3DXMATRIX mtxpos;
	D3DXMatrixIdentity(&mtxpos);	// �P�ʍs��
	if (pParts->m_pParent == nullptr)
	{
		D3DXMatrixTranslation(&mtxpos, m_basePos.x + m_pos.x, m_basePos.y + m_pos.y, m_basePos.z + m_pos.z);
		return mtxpos;
	}

	D3DXMatrixTranslation(&mtxpos, pParts->m_basePos.x + pParts->m_pos.x, pParts->m_basePos.y + pParts->m_pos.y, pParts->m_basePos.z + pParts->m_pos.z);
	return mtxpos * GetMatrixWorld(pParts->m_pParent);
}

//================================================================================
// ���[���h�s��
//--------------------------------------------------------------------------------
D3DXMATRIX CSceneModel::GetMatrixWorld(CSceneModel *pParts)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxPos;

	D3DXMatrixIdentity(&mtxWorld);	//
	D3DXMatrixIdentity(&mtxScl);	//
	D3DXMatrixIdentity(&mtxRot);	//
	D3DXMatrixIdentity(&mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXVECTOR3 scl = pParts->GetScl();
	D3DXVECTOR3 rot = pParts->GetRot() + pParts->GetBaseRot();
	D3DXVECTOR3 pos = pParts->GetPos() + pParts->GetBasePos();
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);	// �g��s��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixTranslation(&mtxPos, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);

	if (pParts->m_pParent == nullptr)
	{
		return mtxWorld;
	}
	return mtxWorld * GetMatrixWorld(pParts->m_pParent);
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry, CModelManager *pUseModelManager)
{
	// CScene3D�̐���
	CSceneModel *pSceneModel;
	pSceneModel = new CSceneModel(pry);
	pSceneModel->Init();

	pSceneModel->SetPos(pos);
	pSceneModel->SetRot(rot);
	pSceneModel->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetScl(scl);
	pSceneModel->m_strFilePass = strFilePass;
	pSceneModel->m_strPartsName = strFilePass;

	// ���f���̐���
	if (pUseModelManager)
	{
		pSceneModel->m_pUseModelManager = pUseModelManager;
	}
	else
	{
		pSceneModel->m_pUseModelManager = CManager::GetModelManager();
	}

	pSceneModel->CreateModel(pSceneModel->m_strFilePass);

	return pSceneModel;
}

//================================================================================
// Parts���w��
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, std::string strPartsName, int pry, CModelManager *pUseModelManager)
{
	// CScene3D�̐���
	CSceneModel *pSceneModel;
	pSceneModel = new CSceneModel(pry);
	pSceneModel->Init();

	pSceneModel->SetPos(pos);
	pSceneModel->SetRot(rot);
	pSceneModel->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pSceneModel->SetScl(scl);
	pSceneModel->m_strFilePass = strFilePass;
	pSceneModel->m_strPartsName = strPartsName;

	// ���f���̐���
	if (pUseModelManager)
	{
		pSceneModel->m_pUseModelManager = pUseModelManager;
	}
	else
	{
		pSceneModel->m_pUseModelManager = CManager::GetModelManager();
	}

	pSceneModel->CreateModel(pSceneModel->m_strFilePass);

	return pSceneModel;
}

//================================================================================
// ���f������
//--------------------------------------------------------------------------------
void CSceneModel::CreateModel(std::string strFilePass)
{
	if (m_pUseModelManager == nullptr)
	{
		m_pUseModelManager = CManager::GetModelManager();
	}

	m_pUseModelManager->SetModel(strFilePass);

	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �e�N�X�`���̃Z�b�g
	D3DXMATERIAL* pMat = m_pUseModelManager->GetModelMat(strFilePass);
	for (int nCntMat = 0; nCntMat < (int)m_pUseModelManager->GetModelNumMat(strFilePass); nCntMat++)
	{
		// �g�p���Ă���e�N�X�`��������Γǂݍ���
		if (pMat[nCntMat].pTextureFilename != nullptr && lstrlen(pMat[nCntMat].pTextureFilename) > 0)
		{
			CManager::GetTexManager()->SetTex(pMat[nCntMat].pTextureFilename);
		}
		else { pMat[nCntMat].pTextureFilename = nullptr; }
	}
	//---------------------------------------------
	// ���f���̒��_�̍ő�l�A�ŏ��l�̎擾
	//---------------------------------------------
	LPD3DXMESH pMesh = nullptr;
	pMesh = m_pUseModelManager->GetModelMesh(strFilePass);
	D3DXVECTOR3 Vertex;
	D3DXVECTOR3 min = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 max = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	//�o�b�t�@�����b�N����
	BYTE *pVtx = nullptr;							// ���_�o�b�t�@
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
	DWORD FVF = pMesh->GetFVF();					// ���_�t�H�[�}�b�g���擾����B
	DWORD VertexSize = D3DXGetFVFVertexSize(FVF);	// ���_�̃T�C�Y���擾����B

	// ���_�̎擾
	for (int nCntMesh = 0; nCntMesh < static_cast<int>(pMesh->GetNumVertices()); nCntMesh++)
	{
		Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// ���_���W���擾����B
		m_centerPos += Vertex;						// �S���_�̍��W�𑫂�����
		min.x = RETURN_MIN(min.x, Vertex.x);		// X�ŏ��l
		max.x = RETURN_MAX(max.x, Vertex.x);		// X�ő�l
		min.y = RETURN_MIN(min.y, Vertex.y);		// Y�ŏ��l
		max.y = RETURN_MAX(max.y, Vertex.y);		// Y�ő�l
		min.z = RETURN_MIN(min.z, Vertex.z);		// Z�ŏ��l
		max.z = RETURN_MAX(max.z, Vertex.z);		// Z�ő�l
		pVtx += VertexSize;							// �i�߂�
	}
	//�o�b�t�@�̃A�����b�N
	pMesh->UnlockVertexBuffer();

	// ��,����,���s�̎擾
	SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));

	// �S���_�̒��S���W
	m_centerPos /= static_cast<FLOAT>(pMesh->GetNumVertices());
}

//================================================================================
// Parts�擾
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::GetParts(std::string partsName)
{
	for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
	{
		std::list<CScene*> *lisScene = CScene::GetScene(n);
		for (CScene* pScene : *lisScene)
		{
			if (pScene == nullptr)continue;
			// �^�C�v�̎擾
			CScene::CLASSTYPE type = pScene->GetClassType();

			if (type == CScene::CLASSTYPE::SCENE_MODEL)
			{
				// �_�E���L���X�g
				CSceneModel *pModel = ((CSceneModel*)pScene);

				// �v�����ꂽ�p�[�c��ԋp
				if (pModel->GetPartsName() == partsName) { return pModel; }
			}
		}
	}

	// �p�[�c�͂���܂���ł���
	return nullptr;
}

//================================================================================
// �eParts�擾
//--------------------------------------------------------------------------------
CSceneModel *CSceneModel::GetParent(CSceneModel *pParts)
{
	if (pParts == nullptr) { return nullptr; }
	return pParts->m_pParent;
}
