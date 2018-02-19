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
// �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
std::unordered_map<std::string, CMotionManager*> CMotionManager::m_umapMotionManager;	// ���[�V�����}�l�[�W��

int CMotionManager::m_fileSize = 0;				// �t�@�C��������
int CMotionManager::m_currentSrc = 0;			// ���݈ʒu
char *CMotionManager::m_pSrcData = nullptr;		// ������i�[�p�|�C���^
char CMotionManager::m_buf[MAX_PATH] = { "" };	// �o�b�t�@(�ꎞ�����i�[�p)

//================================================================================
// ���[�V�����ǂݍ���
//--------------------------------------------------------------------------------
void CMotionManager::LoadMotion(const std::string &strFilePass)
{
	assert(!strFilePass.empty() && "���[�V�����̃t�@�C���p�X���s���ł�");

	// �v�f�̗L�����m���߂�
	if (m_umapMotionManager.count(strFilePass) == 1)
	{// ����܂���
		return;
	}

	// ������
	m_fileSize = 0;			// �t�@�C��������
	m_currentSrc = 0;		// ���݈ʒu	
	m_pSrcData = nullptr;	// ������i�[�p�|�C���^

	// �t�@�C���ǂݍ���
	FILE *pFile;			// �t�@�C���|�C���^
	fopen_s(&pFile, &strFilePass[0], "r");
	// nullptr�`�F�b�N
	if (pFile == nullptr) {
		assert(!"���[�V�����̃t�@�C���p�X���ǂݍ��߂܂���ł���");
	}

	// ���[�V�����}�l�[�W���̎��̂̐���
	CMotionManager *pMotManager = new CMotionManager;

	fseek(pFile, 0, SEEK_END);			// 
	m_fileSize = ftell(pFile);			// 
	fseek(pFile, 0, SEEK_SET);			// �t�@�C���T�C�Y�擾

	m_pSrcData = (char *)malloc(m_fileSize);	// �ǂݍ��݃o�b�t�@�[�m��

	fread(m_pSrcData, m_fileSize, 1, pFile);	// �t�@�C���ǂݍ���
	fclose(pFile);								// �t�@�C�������

	// �X�N���v�g�ǂݍ��݊J�n
	seekSrcData("SCRIPT");
	if (m_currentSrc >= m_fileSize)
	{
		free(m_pSrcData);
		assert(!"�X�N���v�g�̒��g��[SCRIPT]������܂���");
	}

	//========================================
	// ���f�����̎擾
	//========================================
	seekSrcData("NUM_MODEL = ");
	pMotManager->m_numModel = getNumInt();

	//========================================
	// ���f���t�@�C�����̎擾
	//========================================
	PARTS *pParts;
	CModelManager *pModelManager = CManager::GetModelManager();
	for (int i = 0; i < pMotManager->m_numModel; i++)
	{
		seekSrcData("MODEL_FILENAME = ");
		pParts = new PARTS;
		pParts->filePass = getChar();

		// ���f���̓o�^
		pModelManager->SetModel(pParts->filePass);

		pMotManager->m_vecParts.push_back(pParts);
	}
	pMotManager->m_vecParts.shrink_to_fit();

	//========================================
	// �L�����N�^�[���
	//========================================
	// �ړ���
	seekSrcData("MOVE = ");
	pMotManager->m_chara.move = getNumFloat();
	// �W�����v��
	seekSrcData("JUMP = ");
	pMotManager->m_chara.jump = getNumFloat();
	// �����蔻��̔��a
	seekSrcData("RADIUS = ");
	pMotManager->m_chara.rad = getNumFloat();
	// �p�[�c��
	seekSrcData("NUM_PARTS = ");
	pMotManager->m_chara.numParts = getNumInt();

	//========================================
	// �p�[�c���̓ǂݎ��
	//========================================
	float x, y, z;
	for (int i = 0; i < pMotManager->m_chara.numParts; i++)
	{
		// �C���f�b�N�X
		seekSrcData("INDEX = ");
		pMotManager->m_vecParts[i]->index = getNumInt();
		// �e�̃C���f�b�N�X
		seekSrcData("PARENT = ");
		pMotManager->m_vecParts[i]->parent = getNumInt();
		// ���W
		seekSrcData("POS = ");
		x = getNumFloat();
		y = getNumFloat();
		z = getNumFloat();
		pMotManager->m_vecParts[i]->pos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pMotManager->m_vecParts[i]->basePos = D3DXVECTOR3(x, y, z);
		// �p�x
		seekSrcData("ROT = ");
		x = getNumFloat();
		y = getNumFloat();
		z = getNumFloat();
		pMotManager->m_vecParts[i]->rot = D3DXVECTOR3(0.f, 0.f, 0.f);
		pMotManager->m_vecParts[i]->baseRot = D3DXVECTOR3(x, y, z);
	}

	//========================================
	// ���[�V�������
	//========================================
	int motionIndex = 0;
	bool bMotion = false;
	MOTION *pMotion;		// ���[�V����
	// ���[�V���������[�v
	while (1)
	{
		// �����܂ōs�����烋�[�v�𔲂���
		if (strcmp(&m_pSrcData[m_currentSrc], "END_SCRIPT") == 0 || m_currentSrc >= m_fileSize)
		{
			break;
		}
		if (bMotion == true)
		{
			seekSrcData("END_MOTIONSET");
			bMotion = false;
		}
		// ���[�V��������������
		if (seekSrcData("MOTIONSET"))
		{
			bMotion = true;

			// -------- -------- -------- --------
			// ���[�V�����̐���
			// -------- -------- -------- --------
			pMotion = new MOTION;
			// ���[�V�����̏�����
			pMotion->index = 0;
			pMotion->numKey = 0;
			pMotion->loop = false;

			pMotion->index = motionIndex;	// �C���f�b�N�X�̐ݒ�
		}
		else
		{
			break;
		}

		// ���[�v���邩�ǂ���
		seekSrcData("LOOP = ");
		pMotion->loop = (bool)getNumInt();
		// �L�[�̐�
		seekSrcData("NUM_KEY = ");
		pMotion->numKey = getNumInt();

		// -------- -------- -------- --------
		// �L�[�̐錾
		// -------- -------- -------- --------
		KEY *pKey;

		// �L�[�������[�v
		for (int cntKey = 0, end = pMotion->numKey; cntKey < end; cntKey++)
		{
			// �ړ��t���[��
			seekSrcData("FRAME = "); 
			pKey = new KEY;
			// �L�[�̏�����
			pKey->flame = 0;
			pKey->index = 0;
			pKey->flame = getNumInt();
			pKey->index = cntKey;			// �C���f�b�N�X�̎擾

			// -------- -------- -------- --------
			// ���[�u�p�[�c�̐錾
			// -------- -------- -------- --------
			MOVEPARTS *pMoveParts;

			// �p�[�c�����[�v
			for (int cntParts = 0; cntParts < pMotManager->m_chara.numParts; cntParts++)
			{
				pMoveParts = new MOVEPARTS;
				// ���[�u�p�[�c�̏�����
				pMoveParts->index = 0;
				pMoveParts->pos = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rot = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->posParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rotParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);

				pMoveParts->index = cntParts;		// �C���f�b�N�X�̐ݒ�

				// ���W
				seekSrcData("POS = ");
				x = getNumFloat();
				y = getNumFloat();
				z = getNumFloat();
				pMoveParts->pos = D3DXVECTOR3(x, y, z);
				// �p�x
				seekSrcData("ROT = ");
				x = getNumFloat();
				y = getNumFloat();
				z = getNumFloat();
				pMoveParts->rot = D3DXVECTOR3(x, y, z);
				seekSrcData("END_KEY");

				// �t���[�����̈ړ��ʂ̏�����
				pMoveParts->posParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);
				pMoveParts->rotParFlame = D3DXVECTOR3(0.f, 0.f, 0.f);

				// ���[�u�p�[�c���v�b�V���o�b�N
				pKey->lisMoveParts.push_back(pMoveParts);
			}
			seekSrcData("END_KEYSET");

			// �L�[���v�b�V���o�b�N
			pMotion->lisKey.push_back(pKey);
		}

		// ���[�V�������v�b�V���o�b�N
		pMotManager->m_lisMotion.push_back(pMotion);
		motionIndex++;	// �C���f�b�N�X��i�߂�
	}

	// ���[�V�����̏����}�l�[�W���ɓ����
	m_umapMotionManager[strFilePass] = pMotManager;

	// �o�b�t�@���
	free(m_pSrcData);
}

//================================================================================
// �p�[�c���X�g�̎擾(vector)
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

	// �p�[�c��
	CMotionManager *p = m_umapMotionManager[pMotionData->motionFilePass];
	int numParts = p->m_chara.numParts;

	for (int i = 0; i < numParts; i++)
	{
		pParts = p->m_vecParts[i];
		pModel = CSceneModel::Create(pParts->pos, pParts->rot, D3DXVECTOR3(1.f, 1.f, 1.f), pParts->filePass);
		pMotionData->pVecModel->push_back(pModel);
	}

	// �e�p�[�c�̃t�@�C���p�X�𒲂ׂ�
	for (int i = 0; i < numParts; i++)
	{
		pParts = p->m_vecParts[i];
		pModel = pMotionData->pVecModel->at(i);
		if (pParts->parent == -1)
		{
			// �e�̃X�P�[�����Đݒ�
			pModel->SetScl(scl);
			// �p�[�c�̐e��null
			pModel->SetParent(nullptr);
			continue;
		}
		// �p�[�c�ɐe�̃|�C���^��ݒ�
		CSceneModel *parent = pMotionData->pVecModel->at(pParts->parent);
		pModel->SetParent(parent);
	}
	pMotionData->pVecModel->shrink_to_fit();
}

//================================================================================
// ���[�V������������
//--------------------------------------------------------------------------------
void CMotionManager::UpdateMotion(MOTION_DATA *pMotionData)
{
	if (pMotionData == nullptr) { assert(!"������null�ł�"); }
	if (m_umapMotionManager.count(pMotionData->motionFilePass) == 0) { assert(!"�t�@�C�������ݒ肳��Ă��܂���"); }

	// ���[�V�����f�[�^�̎擾
	CMotionManager *p = m_umapMotionManager[pMotionData->motionFilePass];
	MOTION *pMotion = p->GetMotion(pMotionData->motionType);

	// �p�[�c��
	int numParts = p->m_chara.numParts;
	/*
	for (int cntParts = 0; cntParts < numParts; cntParts++)
	{
		pParts = pMotionData->pVecModel->at(cntParts);
		KEY* pKey = p->GetKey(pMotion, pMotionData->numKey);
		MOVEPARTS* pMoveParts = p->GetMoveParts(pKey, cntParts);
		pParts->SetRot(pMoveParts->rotParFlame);	// ��]
		pParts->SetPos(pMoveParts->posParFlame);	// ���W
	}
	*/

	// �J�E���g��i�߂�
	pMotionData->cntFlame++;

	CSceneModel *pParts;
	KEY* pKey = nullptr;
	KEY* pKeyNext = nullptr;
	// �t���[���J�E���g���L�[�̃t���[�����ɂȂ����玟�̃L�[��
	if (pMotionData->cntFlame >= p->GetKey(pMotion, pMotionData->numKey)->flame)
	{
		pMotionData->cntFlame = 0;		// �t���[����������
		pMotionData->numKey++;			// �Q�Ƃ���L�[�̃C���f�b�N�X��i�߂�

		// �L�[���˂��������珉����
		if (p->GetKey(pMotion, pMotionData->numKey) == nullptr)
		{
			pMotionData->numKey = 0;	// �L�[����0�ɖ߂�
		}

		pKey = p->GetKey(pMotion, pMotionData->numKey);			// ���݂̃L�[
		pKeyNext = p->GetKey(pMotion, pMotionData->numKey + 1);	// ���̃L�[
		if (pMotion->loop == false)
		{// ���[�v�Ȃ��̏ꍇ�́A���݂̃L�[��������x
			pKeyNext = pKey;
		}
		else if (pKeyNext == nullptr)
		{// �ŏ��̃L�[
			pKeyNext = p->GetKey(pMotion, 0);
		}
	}

	MOVEPARTS* pMoveParts = nullptr;			// ���݂̕��
	MOVEPARTS* pMovePartsNext = nullptr;		// 
	D3DXVECTOR3 rotParFlame, posParFlame;

	// ���[�V�����u�����h
	MOVEPARTS* pBlendMoveParts;			// ���̕��
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
		// �p�[�c����1�t���[���Ɉړ�����l����
		for (int n = 0; n < numParts; n++)
		{
			// ���݂̕��
			pMoveParts = p->GetMoveParts(pKey, n);				// ���݃��[�u�p�[�c
			pMovePartsNext = p->GetMoveParts(pKeyNext, n);		// ���̃��[�u�p�[�c
			pMoveParts->rotParFlame = pMoveParts->rot * (1.f - rate) + pMovePartsNext->rot * (rate);	// �p�x
			pMoveParts->posParFlame = pMoveParts->pos * (1.f - rate) + pMovePartsNext->pos * (rate);	// ���W

			pParts = pMotionData->pVecModel->at(n);
			pParts->SetRot(pMoveParts->rotParFlame);	// ��]
			pParts->SetPos(pMoveParts->posParFlame);	// ���W
		}
	}
	else
	{
		// �p�[�c����1�t���[���Ɉړ�����l����
		for (int n = 0; n < numParts; n++)
		{
			// ���݂̕��
			pMoveParts = p->GetMoveParts(pKey, n);				// ���݃��[�u�p�[�c
			pMovePartsNext = p->GetMoveParts(pKeyNext, n);		// ���̃��[�u�p�[�c
			rotParFlame = pMoveParts->rot * (1.f - rate) + pMovePartsNext->rot * (rate);	// �p�x
			posParFlame = pMoveParts->pos * (1.f - rate) + pMovePartsNext->pos * (rate);	// ���W

			// ���̕��
			pMotion = p->GetMotion(pMotionData->motionTypeNext);
			pKey = p->GetKey(pMotion, pMotionData->numKey);
			pMotionNext = p->GetMotion(pMotionData->motionTypeNext);
			pKeyNext = p->GetKey(pMotionNext, 0);

			pBlendMoveParts = p->GetMoveParts(pKey, n);				// ���̃��[�V�����̌��݃��[�u�p�[�c
			pBlendMovePartsNext = p->GetMoveParts(pKeyNext, n);		// ���̃��[�V�����̎��̃��[�u�p�[�c
			BlendRotParFlame = pBlendMoveParts->rot * (1.f - rate) + pBlendMovePartsNext->rot * (rate);	// �p�x
			BlendPosParFlame = pBlendMoveParts->pos * (1.f - rate) + pBlendMovePartsNext->pos * (rate);	// ���W

			pMoveParts->rotParFlame = rotParFlame;
			pMoveParts->posParFlame = posParFlame;

			pParts = pMotionData->pVecModel->at(n);
			pParts->SetRot(pMoveParts->rotParFlame);	// ��]
			pParts->SetPos(pMoveParts->posParFlame);	// ���W
		}
	}
}

//================================================================================
// ���[�V�����^�C�v�̐ݒ�
//--------------------------------------------------------------------------------
void CMotionManager::SetMotionType(MOTION_DATA *pMotionData, int motionTypeNext)
{
	if (pMotionData == nullptr) { assert(!"������null�ł�"); }
	if (m_umapMotionManager.count(pMotionData->motionFilePass) == 0) { assert(!"�t�@�C�������ݒ肳��Ă��܂���"); }
	if (pMotionData->motionType == motionTypeNext) { return; }

	pMotionData->motionTypeNext = motionTypeNext;
}

//================================================================================
// ������̎��̕����܂Ői�߂�
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
// �������int�^�Ŏ擾(����)
//--------------------------------------------------------------------------------
int CMotionManager::getNumInt()
{
	int numVtx = 0;		// ����
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// �󔒑O���l�̂��߁A�󔒂�����܂Ń��[�v
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			numVtx = atoi(m_buf);		// ���_���̑����̎擾
			m_currentSrc++;
			return numVtx;
		}
		else
		{// ���� or . or - ����ȊO�͖���
			if (m_pSrcData[m_currentSrc] >= 48 && m_pSrcData[m_currentSrc] < 57 || m_pSrcData[m_currentSrc] == 46 || m_pSrcData[m_currentSrc] == 45)
			{
				// ������̘A��
				strncat_s(&m_buf[0], MAX_PATH, &m_pSrcData[m_currentSrc], 1);
			}
			m_currentSrc++;
		}
	}
	return numVtx;
}

//================================================================================
// �������float�^�Ŏ擾(����)
//--------------------------------------------------------------------------------
float CMotionManager::getNumFloat()
{
	float numVtx = 0.f;	// ����
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// �󔒑O���l�̂��߁A�󔒂�����܂Ń��[�v
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			numVtx = static_cast<float>(atof(m_buf));		// ���_���̑����̎擾
			m_currentSrc++;
			return numVtx;
		}
		else
		{// ���� or . or - ����ȊO�͖���
			if (m_pSrcData[m_currentSrc] >= 48 && m_pSrcData[m_currentSrc] < 57 || m_pSrcData[m_currentSrc] == 46 || m_pSrcData[m_currentSrc] == 45)
			{
				// ������̘A��
				strncat_s(&m_buf[0], MAX_PATH, &m_pSrcData[m_currentSrc], 1);
			}
			m_currentSrc++;
		}
	}
	return numVtx;
}

//================================================================================
// �������char�^�Ŏ擾(����)
//--------------------------------------------------------------------------------
char *CMotionManager::getChar()
{
	for (int i = 0; i < MAX_PATH; i++) { m_buf[i] = '\0'; }
	while (1)
	{
		// �󔒑O���l�̂��߁A�󔒂�����܂Ń��[�v
		if (m_pSrcData[m_currentSrc] == 32 || m_pSrcData[m_currentSrc] == 10 || m_pSrcData[m_currentSrc] == 9)
		{
			m_currentSrc++;
			return &m_buf[0];
		}
		else
		{
			// ������̘A��
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
// ���[�V�������S����
//--------------------------------------------------------------------------------
void CMotionManager::ReleaseAll()
{
	// �R���e�i�𖖔��܂Ń��[�v
	for (auto it : m_umapMotionManager)
	{
		// ���[�V����
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

// ���[�V�����}�l�[�W��
CMotionManager *CMotionManager::GetMotionManager(std::string strName)
{
	if (m_umapMotionManager.count(strName) >= 1)
	{
		return m_umapMotionManager[strName];
	}
	assert(!"�����̃��[�V�������̎w�肪�Ԉ���Ă��܂�");
	return nullptr;
}

// ���[�V����
CMotionManager::MOTION *CMotionManager::GetMotion(int index)
{
	for (auto pMotion : m_lisMotion)
	{
		if (pMotion->index == index) { return pMotion; }
	}
	return nullptr;
}

// �L�[
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