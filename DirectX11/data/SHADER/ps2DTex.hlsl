// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : テクスチャ有りピクセルシェーダー
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
//入力用
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
};

//グローバル
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//ピクセルシェーダー
float4 main(PS_IN IN) : SV_Target
{
	return saturate(IN.col) * txDiffuse.Sample(samLinear, IN.tex);
}

