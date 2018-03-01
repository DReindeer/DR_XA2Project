// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : X���f���ǂݍ���
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include <string.h>
#include "XModelLoader.h"
#include "utility.h"

// -------- -------- -------- -------- -------- -------- -------- --------
//  
// 
// X���f���f�[�^�ǂݍ���
// 
// 
// -------- -------- -------- -------- -------- -------- -------- --------
namespace XModel
{
	// -------- -------- -------- -------- -------- -------- -------- --------
	// 
	// ���f���ǂݍ���
	// 
	// -------- -------- -------- -------- -------- -------- -------- --------
	std::vector<MESH*> *Loader::Load(std::string strFilePass)
	{
		// �t�@�C�����J��
		// -------- -------- -------- -------- -------- -------- -------- --------
		FILE *fp;
		fopen_s(&fp, &strFilePass[0], "r");
		if (fp == nullptr) {
			MessageBox(nullptr, "�w�肳�ꂽ���O�̃t�@�C���͑��݂��܂���", "�x��", S_OK);
		}

		// �t�@�C���ǂݍ���
		// -------- -------- -------- -------- -------- -------- -------- --------
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		char *file = new char[size];
		fseek(fp, 0, SEEK_SET);
		fread(file, size, 1, fp);
		std::string strData;
		strData = file;
		delete[] file;

		int count = 0;
		std::string strBuf;

		// �C���X�^���X�̐���
		// -------- -------- -------- -------- -------- -------- -------- --------
		MESH *pBaseModel = new MESH;

		Utility::Character::CutStrToStr(&strData, &count, "Mesh {");
		Utility::Character::CutStrToStr(&strData, &count, "Mesh {");
		Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
		int numVtx = atoi(&strBuf[0]);

		// ���_�擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		std::vector<XMFLOAT3> vecVtx;
		XMFLOAT3 vtx;
		for (int i = 0; i < numVtx; i++)
		{
			Utility::Character::GetStrToToken(&strData, &count, ";", ",\r\n ", &strBuf);
			vtx.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			vtx.y = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			vtx.z = static_cast<float>(atof(&strBuf[0]));
			vecVtx.push_back(vtx);
		}
		Utility::Character::CutStrToStr(&strData, &count, ";");

		// �ʏ��擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		FACE face;
		face.numMat = 0;
		for (int i = 0; i < 3; i++)
		{
			face.vtx[i].pos = XMFLOAT3(0.f, 0.f, 0.f);
			face.vtx[i].nom = XMFLOAT3(0.f, 0.f, 0.f);
			face.vtx[i].col = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			face.vtx[i].tex = XMFLOAT2(0.f, 0.f);
		}

		Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
		int numFace = atoi(&strBuf[0]);
		for (int i = 0; i < numFace; i++)
		{
			// �t�F�C�X�Ɏg�p����Ă���C���f�b�N�X�̎擾
			Utility::Character::GetStrToToken(&strData, &count, ";", ",\r\n ", &strBuf);
			face.numIdx = atoi(&strBuf[0]);

			// 0
			Utility::Character::GetStrToToken(&strData, &count, ",", "", &strBuf);
			face.idx[0] = atoi(&strBuf[0]);
			face.vtx[0].pos = vecVtx.at(face.idx[0]);
			// 1
			Utility::Character::GetStrToToken(&strData, &count, ",", "", &strBuf);
			face.idx[1] = atoi(&strBuf[0]);
			face.vtx[1].pos = vecVtx.at(face.idx[1]);
			// 2
			Utility::Character::GetStrToToken(&strData, &count, ",;", "", &strBuf);
			face.idx[2] = atoi(&strBuf[0]);
			face.vtx[2].pos = vecVtx.at(face.idx[2]);
			// �v�b�V���o�b�N
			pBaseModel->face.push_back(face);

			if (face.numIdx == 4)
			{
				// 0
				face.idx[0] = face.idx[0];
				face.vtx[0].pos = face.vtx[0].pos;
				// 1
				face.idx[1] = face.idx[2];
				face.vtx[1].pos = face.vtx[2].pos;
				// 2
				Utility::Character::GetStrToToken(&strData, &count, ";", "", &strBuf);
				face.idx[2] = atoi(&strBuf[0]);
				face.vtx[2].pos = vecVtx.at(face.idx[2]);
				// �v�b�V���o�b�N
				pBaseModel->face.push_back(face);
			}
		}

		// �e�ʂɑΉ�����}�e���A���̎擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		Utility::Character::CutStrToStr(&strData, &count, "MeshMaterialList {");

		Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
		int numMat = atoi(&strBuf[0]);
		Utility::Character::CutStrToStr(&strData, &count, ";");

		int cntIdx = 0;
		for (int i = 0; i < numFace; i++)
		{
			// 
			Utility::Character::GetStrToToken(&strData, &count, ",;", "", &strBuf);
			pBaseModel->face.at(cntIdx).numMat = atoi(&strBuf[0]);

			if (pBaseModel->face.at(cntIdx).numIdx == 4)
			{
				cntIdx++;
				pBaseModel->face.at(cntIdx).numMat = pBaseModel->face.at(cntIdx - 1).numMat;
			}
			cntIdx++;
		}

		// �}�e���A�����̎擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		std::vector<MATERIAL> vecMat;
		MATERIAL mat; 
		mat.matCol = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		mat.power = 0.f;
		mat.specular = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		mat.emissive = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		mat.filePass = "\0";
		for (int i = 0; i < numMat; i++)
		{
			Utility::Character::CutStrToStr(&strData, &count, "Material {");
			// �J���[(�f�t���[�Y)
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.matCol.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.matCol.y = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.matCol.z = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToStr(&strData, &count, ";;", "\r\n ", &strBuf);
			mat.matCol.w = static_cast<float>(atof(&strBuf[0]));
			// �p���[
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.power = static_cast<float>(atof(&strBuf[0]));
			// �J���[(�X�y�L����)
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.specular.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.specular.y = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToStr(&strData, &count, ";;", "\r\n ", &strBuf);
			mat.specular.z = static_cast<float>(atof(&strBuf[0]));
			mat.specular.w = 1.f;
			// �J���[(�G�~�b�V�u)
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.emissive.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			mat.emissive.y = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToStr(&strData, &count, ";;", "\r\n ", &strBuf);
			mat.emissive.z = static_cast<float>(atof(&strBuf[0]));
			mat.emissive.w = 1.f;

			// �e�N�X�`��
			char texData[32];
			strncpy_s(&texData[0], 32, &strData[count], _TRUNCATE);
			if (strstr(&texData[0], "TextureFilename") != nullptr ? true : false)
			{
				Utility::Character::CutStrToStr(&strData, &count, "TextureFilename {");
				Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n\" ", &strBuf);
				mat.filePass = &strBuf[0];
			}

			vecMat.push_back(mat);
		}

		// �@�����̎擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		Utility::Character::CutStrToStr(&strData, &count, "MeshNormals {");
		Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
		int numNom = atoi(&strBuf[0]);

		std::vector<XMFLOAT3> vecNom;
		XMFLOAT3 nom;
		for (int i = 0; i < numNom; i++)
		{
			Utility::Character::GetStrToToken(&strData, &count, ";", ",\r\n ", &strBuf);
			nom.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			nom.y = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			nom.z = static_cast<float>(atof(&strBuf[0]));
			vecNom.push_back(nom);
		}
		Utility::Character::CutStrToStr(&strData, &count, ";");

		// �e�ʂɑΉ�����@�����̎擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		Utility::Character::CutStrToStr(&strData, &count, ";");
		cntIdx = 0;
		int numIdx = 0;
		int idx = 0;
		for (int cntFace = 0; cntFace < numFace; cntFace++)
		{
			// �ʂ̒��_��
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			numIdx = atoi(&strBuf[0]);

			// 0
			Utility::Character::GetStrToToken(&strData, &count, ",", "", &strBuf);
			idx = atoi(&strBuf[0]);
			pBaseModel->face.at(cntIdx).vtx[0].nom = vecNom.at(idx);
			// 1
			Utility::Character::GetStrToToken(&strData, &count, ",", "", &strBuf);
			idx = atoi(&strBuf[0]);
			pBaseModel->face.at(cntIdx).vtx[1].nom = vecNom.at(idx);
			// 2
			Utility::Character::GetStrToToken(&strData, &count, ",;", "", &strBuf);
			idx = atoi(&strBuf[0]);
			pBaseModel->face.at(cntIdx).vtx[2].nom = vecNom.at(idx);

			if (pBaseModel->face.at(cntIdx).numIdx == 4)
			{
				cntIdx++;
				// 0
				pBaseModel->face.at(cntIdx).vtx[0].nom = pBaseModel->face.at(cntIdx - 1).vtx[0].nom;
				// 1
				pBaseModel->face.at(cntIdx).vtx[1].nom = pBaseModel->face.at(cntIdx - 1).vtx[2].nom;
				// 2
				Utility::Character::GetStrToToken(&strData, &count, ",;", "", &strBuf);
				idx = atoi(&strBuf[0]);
				pBaseModel->face.at(cntIdx).vtx[2].nom = vecNom.at(idx);
			}
			cntIdx++;
		}

		// UV���̎擾
		// -------- -------- -------- -------- -------- -------- -------- --------
		Utility::Character::CutStrToStr(&strData, &count, "MeshTextureCoords {");
		Utility::Character::CutStrToStr(&strData, &count, ";");
		std::vector<XMFLOAT2> vecTex;
		XMFLOAT2 tex;
		for (int cntVtx = 0; cntVtx < numVtx; cntVtx++)
		{
			Utility::Character::GetStrToToken(&strData, &count, ";", ",\r\n ", &strBuf);
			tex.x = static_cast<float>(atof(&strBuf[0]));
			Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
			tex.y = static_cast<float>(atof(&strBuf[0]));
			vecTex.push_back(tex);
		}
		Utility::Character::CutStrToStr(&strData, &count, ";");

		for (int cntFace = 0; cntFace < static_cast<int>(pBaseModel->face.size()); cntFace++)
		{
			pBaseModel->face.at(cntFace).vtx[0].tex = vecTex.at(pBaseModel->face.at(cntFace).idx[0]);
			pBaseModel->face.at(cntFace).vtx[1].tex = vecTex.at(pBaseModel->face.at(cntFace).idx[1]);
			pBaseModel->face.at(cntFace).vtx[2].tex = vecTex.at(pBaseModel->face.at(cntFace).idx[2]);
		}

		// ���_�J���[
		char texData[32];
		strncpy_s(&texData[0], 32, &strData[count], _TRUNCATE);
		if (strstr(&texData[0], "MeshVertexColors {") != nullptr ? true : false)
		{
			// ���_�J���[���̎擾
			// -------- -------- -------- -------- -------- -------- -------- --------
			Utility::Character::CutStrToStr(&strData, &count, "MeshVertexColors {");
			Utility::Character::CutStrToStr(&strData, &count, ";");
			std::vector<XMFLOAT4> vecCol;
			XMFLOAT4 col;
			for (int cntVtx = 0; cntVtx < numVtx; cntVtx++)
			{
				Utility::Character::GetStrToToken(&strData, &count, ";", ",\r\n ", &strBuf);
				Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
				col.x = static_cast<float>(atof(&strBuf[0]));
				Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
				col.y = static_cast<float>(atof(&strBuf[0]));
				Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
				col.z = static_cast<float>(atof(&strBuf[0]));
				Utility::Character::GetStrToToken(&strData, &count, ";", "\r\n ", &strBuf);
				col.w = static_cast<float>(atof(&strBuf[0]));
				vecCol.push_back(col);
			}
			Utility::Character::CutStrToStr(&strData, &count, ";");

			for (int cntFace = 0; cntFace < static_cast<int>(pBaseModel->face.size()); cntFace++)
			{
				pBaseModel->face.at(cntFace).vtx[0].col = vecCol.at(pBaseModel->face.at(cntFace).idx[0]);
				pBaseModel->face.at(cntFace).vtx[1].col = vecCol.at(pBaseModel->face.at(cntFace).idx[1]);
				pBaseModel->face.at(cntFace).vtx[2].col = vecCol.at(pBaseModel->face.at(cntFace).idx[2]);
			}
		}
		else
		{
			for (int cntFace = 0; cntFace < static_cast<int>(pBaseModel->face.size()); cntFace++)
			{
				pBaseModel->face.at(cntFace).vtx[0].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
				pBaseModel->face.at(cntFace).vtx[1].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
				pBaseModel->face.at(cntFace).vtx[2].col = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			}
		}

		// �}�e���A�����Ƀ��f���𕪊�
		// -------- -------- -------- -------- -------- -------- -------- --------
		std::vector<MESH*> *vecModel = new std::vector<MESH*>;
		MESH *pMesh;
		for (int cntMat = 0; cntMat < numMat; cntMat++)
		{
			pMesh = new MESH;
			// ���b�V������
			for (int cntFace = 0; cntFace < static_cast<int>(pBaseModel->face.size()); cntFace++)
			{
				if (pBaseModel->face.at(cntFace).numMat == cntMat)
				{
					// ���_
					for (int cntVtx = 0; cntVtx < 3; cntVtx++)
					{
						face.vtx[cntVtx].col = pBaseModel->face.at(cntFace).vtx[cntVtx].col;
						face.vtx[cntVtx].pos = pBaseModel->face.at(cntFace).vtx[cntVtx].pos;
						face.vtx[cntVtx].tex = pBaseModel->face.at(cntFace).vtx[cntVtx].tex;
						face.vtx[cntVtx].nom = pBaseModel->face.at(cntFace).vtx[cntVtx].nom;
					}
					// �C���f�b�N�X
					face.idx[0] = pBaseModel->face.at(cntFace).idx[0];
					face.idx[1] = pBaseModel->face.at(cntFace).idx[1];
					face.idx[2] = pBaseModel->face.at(cntFace).idx[2];
					// �ʒ��_��
					face.numIdx = pBaseModel->face.at(cntFace).numIdx;
					// �g�p�}�e���A��
					face.numMat = pBaseModel->face.at(cntFace).numMat;

					pMesh->face.push_back(face);
				}
			}
			// �}�e���A��
			pMesh->mat.matCol = vecMat.at(cntMat).matCol;
			pMesh->mat.power = vecMat.at(cntMat).power;
			pMesh->mat.specular = vecMat.at(cntMat).specular;
			pMesh->mat.emissive = vecMat.at(cntMat).emissive;
			pMesh->mat.filePass = vecMat.at(cntMat).filePass;

			// ���b�V��
			vecModel->push_back(pMesh);
		}

		// �t�@�C�������
		fclose(fp);
		delete pBaseModel;

		return vecModel;
	}
}
