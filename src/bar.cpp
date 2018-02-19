//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "bar.h"
#include "renderer.h"

std::unordered_map<std::string, CBar2D*> CBar2D::umapBar2D;

//================================================================================
// ������
//--------------------------------------------------------------------------------
void CBar2D::Init()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Init();
	}
}

//================================================================================
// �I��
//--------------------------------------------------------------------------------
void CBar2D::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		if(m_pBar[i] == nullptr)continue;
		m_pBar[i]->Uninit();
	}
}

//================================================================================
// �X�V
//--------------------------------------------------------------------------------
void CBar2D::Update()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Update();
	}

	// ���Ԃɂ���o�[�̒Ǐ]
	followingBar();
}

//================================================================================
// ���ԃo�[�̏���
//--------------------------------------------------------------------------------
void CBar2D::followingBar()
{
	D3DXVECTOR3 folSize = m_pBar[1]->GetSize();		// ���ԃo�[�T�C�Y
	D3DXVECTOR3 hpBarSize = m_pBar[2]->GetSize();	// HP�o�[�T�C�Y

	if (folSize.x <= hpBarSize.x)
	{
		// �T�C�Y�ƍ��W����v�����鏈��
		m_pBar[1]->SetSize(hpBarSize);
		m_pBar[1]->SetPos(m_pBar[2]->GetPos());
		return;
	}

	// �T�C�Y�ƍ��W����v�����鏈��
	if (folSize.x - 0.5f < hpBarSize.x)
	{
		m_pBar[1]->SetSize(hpBarSize);
		m_pBar[1]->SetPos(m_pBar[2]->GetPos());
	}
	else if(folSize.x > hpBarSize.x)
	{// ���ԃo�[����
		m_pBar[1]->AddSize(D3DXVECTOR3(-0.5f, 0.f, 0.f));
		m_pBar[1]->AddPos(D3DXVECTOR3(-0.5f, 0.f, 0.f));
	}
}

//================================================================================
// �`��
//--------------------------------------------------------------------------------
void CBar2D::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pBar[i] == nullptr)continue;
		m_pBar[i]->Draw();
	}
}

//================================================================================
// �S�o�[�X�V
//--------------------------------------------------------------------------------
void CBar2D::UpdateAll()
{
	for (auto bar2D : umapBar2D) { bar2D.second->Update(); }
}

//================================================================================
// �S�o�[�`��
//--------------------------------------------------------------------------------
void CBar2D::DrawAll()
{
	for (auto bar2D : umapBar2D) { bar2D.second->Draw(); }
}

//================================================================================
// �S�o�[���
//--------------------------------------------------------------------------------
void CBar2D::ReleaseAll()
{
	for (auto bar2D : umapBar2D) 
	{
		if (bar2D.second != nullptr)
		{
			// �������
			bar2D.second->Uninit();
			delete bar2D.second;
			bar2D.second = nullptr;
		}
	}
	// �R���e�i����ɂ���
	umapBar2D.clear();
}

//================================================================================
// ����
//--------------------------------------------------------------------------------
CBar2D* CBar2D::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size, int r, int g, int b, int a, float maxVal, std::string strName, int priority)
{
	assert(umapBar2D.count(strName) == 0 && "umapBar2D�Ɋ��ɓ����̃p�X�����݂��܂�");

	CBar2D *pBar2D = new CBar2D;

	pBar2D->m_maxVal = maxVal;
	pBar2D->m_basePos = D3DXVECTOR3(pos.x, pos.y, 0.f);
	pBar2D->m_strName = strName;

	for (int i = 0; i < 3; i++)
	{
		pBar2D->m_pBar[i] = new CScene2D(priority);
		pBar2D->m_pBar[i]->Init();
		pBar2D->m_pBar[i]->SetSize(D3DXVECTOR3(size.x, size.y, 0.f));
		pBar2D->m_pBar[i]->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.f));

		// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		// D3Device�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// ���_���̍쐬
		//---------------------
		if (FAILED(pDevice->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,			// �������̃T�C�Y
			D3DUSAGE_WRITEONLY,					// �g�p���@(�������݂̂�)
			FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g(���Ȃ�)
			D3DPOOL_MANAGED,					// �o�b�t�@�̊Ǘ����@(����ɕ������Ă����)
			pBar2D->m_pBar[i]->GetVtxBuffer(),							// 
			nullptr)))
		{
			// ���s������G���[��Ԃ�
			MessageBox(nullptr, "2D�pVtxBuffer�̍쐬�Ɏ��s���܂���", "�x��", MB_OK);
			return nullptr;
		}
	}
	pBar2D->m_pBar[0]->SetSize(D3DXVECTOR3(size.x + 2, size.y + 2, 0.f));
	pBar2D->m_pBar[0]->SetCol(int(r * 0.01f), int(g * 0.01f), int(b * 0.01f), a);
	pBar2D->m_pBar[1]->SetCol(int(r * 0.5f), int(g * 0.5f), int(b * 0.5f), a);
	pBar2D->m_pBar[2]->SetCol(r, g, b, a);

	umapBar2D[strName] = pBar2D;

	return umapBar2D[strName];
}

//================================================================================
// �o�[�̒l�̑����ݒ�
//--------------------------------------------------------------------------------
void CBar2D::AddVal(float addVal, std::string strName)
{
	if (umapBar2D.count(strName) == 0)return;
	CBar2D* pBar2D = umapBar2D[strName];

	D3DXVECTOR3 maxSize = pBar2D->m_pBar[0]->GetSize();
	D3DXVECTOR3 size = pBar2D->m_pBar[2]->GetSize();

	// �����Ńo�[�̕��̑������Z�o
	float val = addVal * maxSize.x / pBar2D->m_maxVal;
	
	if (size.x + val >= maxSize.x)
	{// ���^���������ꍇ�͍ő�T�C�Y�ɍ��킹��
		pBar2D->m_pBar[1]->SetSize(maxSize);
		pBar2D->m_pBar[1]->SetPos(pBar2D->m_basePos);
		pBar2D->m_pBar[2]->SetSize(maxSize);
		pBar2D->m_pBar[2]->SetPos(pBar2D->m_basePos);
	}
	else if (size.x + val <= 0.f)
	{// 0�ȉ��������ꍇ��0
		pBar2D->m_pBar[1]->SetSize(D3DXVECTOR3(0.f, maxSize.y, 0.f));
		pBar2D->m_pBar[1]->SetPos(D3DXVECTOR3(pBar2D->m_basePos.x, 0.f, 0.f));
		pBar2D->m_pBar[2]->SetSize(D3DXVECTOR3(0.f, maxSize.y, 0.f));
		pBar2D->m_pBar[2]->SetPos(D3DXVECTOR3(pBar2D->m_basePos.x, 0.f, 0.f));
	}
	else
	{
		pBar2D->m_pBar[2]->AddSize(D3DXVECTOR3(val, 0.f, 0.f));
		pBar2D->m_pBar[2]->AddPos(D3DXVECTOR3(val * 1.f, 0.f, 0.f));
	}
}
