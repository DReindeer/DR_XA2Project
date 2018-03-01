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
	float3 nor : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 col2: COLOR2;		//計算結果をピクセルシェーダーに送る用
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
	float4 Light;		//ライト
}

VS_OUT main(VS_IN IN)
{
	VS_OUT OUT;

	//頂点処理
	OUT.pos = mul(IN.pos, World);
	OUT.pos = mul(OUT.pos, View);
	OUT.pos = mul(OUT.pos, Projection);
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//光源処理
	float3 L = normalize(Light.xyz);

	//法線ベクトル処理
	float3 nor;
	//ワールド変換
	nor = mul(IN.nor, (float3x3)World);
	nor = normalize(nor);

	//光源と法線の内積を求める
	OUT.col2 = max(0.01, dot(nor, L));

	OUT.col2.a = 1.0f;	//α値を無効にする

	return OUT;
}