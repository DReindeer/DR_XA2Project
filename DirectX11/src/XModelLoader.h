
#ifndef _XMODELLOADER_H_
#define _XMODELLOADER_H_

#include <vector>
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 
// 
// X���f���f�[�^�ǂݍ���
// 
// 
// -------- -------- -------- -------- -------- -------- -------- --------
namespace XModel
{
	// ���_
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct VERTEX
	{
		XMFLOAT3 pos;
		XMFLOAT3 nom;
		XMFLOAT2 tex;
		XMFLOAT4 col;
	};

	// ��
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct FACE
	{
		int numMat;
		int numIdx;
		int idx[3];
		VERTEX vtx[3];
	};

	// �}�e���A��
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct MATERIAL
	{
		XMFLOAT4 matCol;
		float power;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		std::string filePass;
	};

	// ���f��
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct MESH
	{
		MATERIAL mat;
		std::vector<FACE> face;
	};

	// -------- -------- -------- -------- -------- -------- -------- --------
	// 
	// �ǂݍ��݃N���X
	// 
	// -------- -------- -------- -------- -------- -------- -------- --------
	class Loader
	{
	public:
		std::vector<MESH*> *Load(std::string strFilePass);
	};
}

#endif
