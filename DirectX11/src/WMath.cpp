// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : êîäwånä÷êîåQ(+DirectXMathÇÃï‚èïä÷êî)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "Wmath.h"

namespace XM
{
	namespace F2
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT2
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// ë´ÇµéZ
		XMFLOAT2 Add(XMFLOAT2 v, XMFLOAT2 addv) {
			XMVECTOR vec = XMLoadFloat2(&v);
			XMVECTOR add = XMLoadFloat2(&addv);
			XMFLOAT2 ans;
			XMStoreFloat2(&ans, XMVectorAdd(vec, add));
			return ans;
		}
		// à¯Ç´éZ
		XMFLOAT2 Sub(XMFLOAT2 v, XMFLOAT2 subv) {
			XMVECTOR vec = XMLoadFloat2(&v);
			XMVECTOR sub = XMLoadFloat2(&subv);
			XMFLOAT2 ans;
			XMStoreFloat2(&ans, XMVectorSubtract(vec, sub));
			return ans;
		}
		// ä|ÇØéZ
		XMFLOAT2 Mul(XMFLOAT2 v, XMFLOAT2 mulv) {
			XMVECTOR vec = XMLoadFloat2(&v);
			XMVECTOR mul = XMLoadFloat2(&mulv);
			XMFLOAT2 ans;
			XMStoreFloat2(&ans, XMVectorMultiply(vec, mul));
			return ans;
		}
		// äÑÇËéZ
		XMFLOAT2 Div(XMFLOAT2 v, XMFLOAT2 divv) {
			XMVECTOR vec = XMLoadFloat2(&v);
			XMVECTOR div = XMLoadFloat2(&divv);
			XMFLOAT2 ans;
			XMStoreFloat2(&ans, XMVectorDivide(vec, div));
			return ans;
		}
		// í∑Ç≥
		float Length(XMFLOAT2 v) {
			XMVECTOR vec = XMLoadFloat2(&v);
			float ans;
			XMStoreFloat(&ans, XMVector2Length(vec));
			return ans;
		}
		// ãóó£
		float Distance(XMFLOAT2 start, XMFLOAT2 end) {
			XMVECTOR posS = XMLoadFloat2(&start);
			XMVECTOR posE = XMLoadFloat2(&end);
			float ans;
			XMStoreFloat(&ans, XMVector2Length(posE - posS));
			return ans;
		}
		// ê≥ãKâª
		XMFLOAT2 Normalize(XMFLOAT2 v) {
			XMVECTOR vec = XMLoadFloat2(&v);
			XMStoreFloat2(&v, XMVector2Normalize(vec));
			return v;
		}
	}
	namespace F3
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT3
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// ë´ÇµéZ
		XMFLOAT3 Add(XMFLOAT3 v, XMFLOAT3 addv) {
			XMVECTOR vec = XMLoadFloat3(&v);
			XMVECTOR add = XMLoadFloat3(&addv);
			XMFLOAT3 ans;
			XMStoreFloat3(&ans, XMVectorAdd(vec, add));
			return ans;
		}
		// à¯Ç´éZ
		XMFLOAT3 Sub(XMFLOAT3 v, XMFLOAT3 subv) {
			XMVECTOR vec = XMLoadFloat3(&v);
			XMVECTOR sub = XMLoadFloat3(&subv);
			XMFLOAT3 ans;
			XMStoreFloat3(&ans, XMVectorSubtract(vec, sub));
			return ans;
		}
		// ä|ÇØéZ
		XMFLOAT3 Mul(XMFLOAT3 v, XMFLOAT3 mulv) {
			XMVECTOR vec = XMLoadFloat3(&v);
			XMVECTOR mul = XMLoadFloat3(&mulv);
			XMFLOAT3 ans;
			XMStoreFloat3(&ans, XMVectorMultiply(vec, mul));
			return ans;
		}
		// äÑÇËéZ
		XMFLOAT3 Div(XMFLOAT3 v, XMFLOAT3 divv) {
			XMVECTOR vec = XMLoadFloat3(&v);
			XMVECTOR div = XMLoadFloat3(&divv);
			XMFLOAT3 ans;
			XMStoreFloat3(&ans, XMVectorDivide(vec, div));
			return ans;
		}
		// í∑Ç≥
		float Length(XMFLOAT3 v) {
			XMVECTOR vec = XMLoadFloat3(&v);
			float ans;
			XMStoreFloat(&ans, XMVector3Length(vec));
			return ans;
		}
		// ãóó£
		float Distance(XMFLOAT3 start, XMFLOAT3 end) {
			XMVECTOR posS = XMLoadFloat3(&start);
			XMVECTOR posE = XMLoadFloat3(&end);
			float ans;
			XMStoreFloat(&ans, XMVector3Length(posE - posS));
			return ans;
		}
		// ê≥ãKâª
		XMFLOAT3 Normalize(XMFLOAT3 v) {
			XMVECTOR vec = XMLoadFloat3(&v);
			XMStoreFloat3(&v, XMVector3Normalize(vec));
			return v;
		}
		// äpìx
		XMFLOAT3 LookAt(float rotY) {
			XMFLOAT3 rot = XMFLOAT3(cosf(rotY), 0.f, sinf(rotY));
			return rot;
		}
		// äpìx
		XMFLOAT3 RotY(XMFLOAT3 move) {
			XMFLOAT3 rot = XMFLOAT3(0.f, static_cast<float>(atan2(move.z, move.x) + PI * 0.5f), 0.f);
			return rot;
		}
	}
	namespace F4
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT4
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// ë´ÇµéZ
		XMFLOAT4 Add(XMFLOAT4 v, XMFLOAT4 addv) {
			XMVECTOR vec = XMLoadFloat4(&v);
			XMVECTOR add = XMLoadFloat4(&addv);
			XMFLOAT4 ans;
			XMStoreFloat4(&ans, XMVectorAdd(vec, add));
			return ans;
		}
		// à¯Ç´éZ
		XMFLOAT4 Sub(XMFLOAT4 v, XMFLOAT4 subv) {
			XMVECTOR vec = XMLoadFloat4(&v);
			XMVECTOR sub = XMLoadFloat4(&subv);
			XMFLOAT4 ans;
			XMStoreFloat4(&ans, XMVectorSubtract(vec, sub));
			return ans;
		}
		// ä|ÇØéZ
		XMFLOAT4 Mul(XMFLOAT4 v, XMFLOAT4 mulv) {
			XMVECTOR vec = XMLoadFloat4(&v);
			XMVECTOR mul = XMLoadFloat4(&mulv);
			XMFLOAT4 ans;
			XMStoreFloat4(&ans, XMVectorMultiply(vec, mul));
			return ans;
		}
		// äÑÇËéZ
		XMFLOAT4 Div(XMFLOAT4 v, XMFLOAT4 divv) {
			XMVECTOR vec = XMLoadFloat4(&v);
			XMVECTOR div = XMLoadFloat4(&divv);
			XMFLOAT4 ans;
			XMStoreFloat4(&ans, XMVectorDivide(vec, div));
			return ans;
		}
		// í∑Ç≥
		float Length(XMFLOAT4 v) {
			XMVECTOR vec = XMLoadFloat4(&v);
			float ans;
			XMStoreFloat(&ans, XMVector4Length(vec));
			return ans;
		}
		// ãóó£
		float Distance(XMFLOAT4 start, XMFLOAT4 end) {
			XMVECTOR posS = XMLoadFloat4(&start);
			XMVECTOR posE = XMLoadFloat4(&end);
			float ans;
			XMStoreFloat(&ans, XMVector4Length(posE - posS));
			return ans;
		}
		// ê≥ãKâª
		XMFLOAT4 Normalize(XMFLOAT4 v) {
			XMVECTOR vec = XMLoadFloat4(&v);
			XMStoreFloat4(&v, XMVector4Normalize(vec));
			return v;
		}
	}
	namespace F4X4
	{
		// ======== ======== ======== ======== ======== ======== ======== ========
		// 
		// 
		//								XMFLOAT4X4
		// 
		// 
		// ======== ======== ======== ======== ======== ======== ======== ========
		// ä|ÇØéZ
		XMFLOAT4X4 Mul(XMFLOAT4X4 m, XMFLOAT4X4 mulm) {
			XMMATRIX mat = XMLoadFloat4x4(&m);
			XMMATRIX mul = XMLoadFloat4x4(&mulm);
			XMFLOAT4X4 ans;
			XMStoreFloat4x4(&ans, XMMatrixMultiply(mat, mul));
			return ans;
		}
		// íPà çsóÒ
		XMFLOAT4X4 Identity(XMFLOAT4X4 *m) {
			XMStoreFloat4x4(m, XMMatrixIdentity());
			return *m;
		}
		XMFLOAT4X4 WorldMatrixMultiply(XMMATRIX *mtxScl, XMMATRIX *mtxRot, XMMATRIX *mtxPos)
		{
			XMMATRIX world = XMMatrixMultiply(XMMatrixMultiply(*mtxScl, *mtxRot), *mtxPos);
			XMFLOAT4X4 ans;
			XMStoreFloat4x4(&ans, world);
			return ans;
		}
	}
	// ======== ======== ======== ======== ======== ======== ======== ========
	// 
	// 
	//								É}ÉgÉäÉNÉXån
	// 
	// 
	// ======== ======== ======== ======== ======== ======== ======== ========
	void ComputeAnglesFromMatrix(XMMATRIX *R, float *angle_x, float *angle_y, float *angle_z)
	{
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, *R);
		double threshold = 0.001;

		if (fabs(mat(2, 1) - 1.0) < threshold) { // R(2,1) = sin(x) = 1ÇÃéû
			*angle_x = static_cast<float>(PI / 2);
			*angle_y = static_cast<float>(0);
			*angle_z = static_cast<float>(atan2(mat(1, 0), mat(0, 0)));
		}
		else if (fabs(mat(2, 1) + 1.0) < threshold) { // R(2,1) = sin(x) = -1ÇÃéû
			*angle_x = static_cast<float>(-PI / 2);
			*angle_y = static_cast<float>(0);
			*angle_z = static_cast<float>(atan2(mat(1, 0), mat(0, 0)));
		}
		else {
			*angle_x = static_cast<float>(asin(mat(2, 1)));
			*angle_y = static_cast<float>(atan2(-mat(2, 0), mat(2, 2)));
			*angle_z = static_cast<float>(atan2(-mat(0, 1), mat(1, 1)));
		}
	}
	// ÉLÉÉÉâÉNÉ^épê®çsóÒéZèoä÷êî
	XMMATRIX* CalcLookAtMatrix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp)
	{
		XMVECTOR vX, vY, vZ;
		vZ = *pLook - *pPos;
		vZ = XMVector3Normalize(vZ);
		vY = XMVector3Normalize(*pUp);
		vX = XMVector3Cross(vY, vZ);
		vX = XMVector3Normalize(vX);
		vY = XMVector3Cross(vZ, vX);
		vY = XMVector3Normalize(vY);

		XMFLOAT3 X, Y, Z;
		XMStoreFloat3(&X, vX);
		XMStoreFloat3(&Y, vY);
		XMStoreFloat3(&Z, vZ);

		XMFLOAT4X4 mat;
		mat._11 = X.x; mat._12 = X.y; mat._13 = X.z; mat._14 = 0;
		mat._21 = Y.x; mat._22 = Y.y; mat._23 = Y.z; mat._24 = 0;
		mat._31 = Z.x; mat._32 = Z.y; mat._33 = Z.z; mat._34 = 0;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 1.0f;

		*pout = XMLoadFloat4x4(&mat);
		return pout;
	}
	// ÉLÉÉÉâÉNÉ^ë©îõépê®çsóÒéZèoä÷êî
	XMMATRIX* CalcLookAtMatrixAxisFix(XMMATRIX* pout, XMVECTOR* pPos, XMVECTOR* pLook, XMVECTOR* pUp)
	{
		XMVECTOR vX, vY, vZ, vD;
		vD = *pLook - *pPos;
		vD = XMVector3Normalize(vD);
		vY = XMVector3Normalize(*pUp);
		vX = XMVector3Cross(vY, vD);
		vX = XMVector3Normalize(vX);
		vZ = XMVector3Cross(vX, vY);
		vZ = XMVector3Normalize(vZ);

		XMFLOAT3 X, Y, Z;
		XMStoreFloat3(&X, vX);
		XMStoreFloat3(&Y, vY);
		XMStoreFloat3(&Z, vZ);

		XMFLOAT4X4 mat;
		mat._11 = X.x; mat._12 = X.y; mat._13 = X.z; mat._14 = 0;
		mat._21 = Y.x; mat._22 = Y.y; mat._23 = Y.z; mat._24 = 0;
		mat._31 = Z.x; mat._32 = Z.y; mat._33 = Z.z; mat._34 = 0;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 1.0f;

		*pout = XMLoadFloat4x4(&mat);
		return pout;
	}
}
