
#ifndef _XMODELLOADER_H_
#define _XMODELLOADER_H_

#include <vector>
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// 
// 
// Xモデルデータ読み込み
// 
// 
// -------- -------- -------- -------- -------- -------- -------- --------
namespace XModel
{
	// 頂点
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct VERTEX
	{
		XMFLOAT3 pos;
		XMFLOAT3 nom;
		XMFLOAT2 tex;
		XMFLOAT4 col;
	};

	// 面
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct FACE
	{
		int numMat;
		int numIdx;
		int idx[3];
		VERTEX vtx[3];
	};

	// マテリアル
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct MATERIAL
	{
		XMFLOAT4 matCol;
		float power;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		std::string filePass;
	};

	// モデル
	// -------- -------- -------- -------- -------- -------- -------- --------
	struct MESH
	{
		MATERIAL mat;
		std::vector<FACE> face;
	};

	// -------- -------- -------- -------- -------- -------- -------- --------
	// 
	// 読み込みクラス
	// 
	// -------- -------- -------- -------- -------- -------- -------- --------
	class Loader
	{
	public:
		std::vector<MESH*> *Load(std::string strFilePass);
	};
}

#endif
