// ======== ======== ======== ======== ======== ======== ======== ========
//
//	title : classMake[ modelManager.h ]		Auter : KENSUKE WATANABE
//														Data  : 2017/04/27
//
// -------- -------- -------- -------- -------- -------- -------- --------
//■Update : 2017/04/27
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "main.h"
#include <unordered_map>
#include "XModelLoader.h"

// ======== ======== ======== ======== ======== ======== ======== ========
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CXModelManager
{
public:

	CXModelManager(){}
	~CXModelManager(){}

	void Init(void);
	void Uninit(void);
	
	bool SetModel(const std::string &strFilePass);

	std::vector<XModel::MESH*> *GetModel(const std::string &strFilePass);
	
	bool Release(const std::string &strFilePass);			// 解放
	void ReleaseAll(void);									// 全開放

private:
	std::unordered_map<std::string, std::vector<XModel::MESH*> *> m_unmapModel;	// ファイルパス
};

#endif