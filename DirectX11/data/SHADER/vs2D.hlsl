// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 頂点シェーダー
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

//変換用行列
cbuffer ConstantBuffer : register(b0)
{
	matrix View;		//ビュー変換行列
	matrix Projection;	//透視射影変換行列
}

cbuffer CBufferModel : register(b1)
{
	matrix World;		//ワールド変換行列
}


VS_OUT main(VS_IN IN)
{
	VS_OUT OUT; 

	//頂点処理
	OUT.pos = mul(IN.pos, World);
	OUT.pos = mul(OUT.pos, Projection);
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	return OUT;
}