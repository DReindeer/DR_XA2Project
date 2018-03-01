// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �e�N�X�`���L��s�N�Z���V�F�[�_�[
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
//���͗p
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
};

//�O���[�o��
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//�s�N�Z���V�F�[�_�[
float4 main(PS_IN IN) : SV_Target
{
	return saturate(IN.col) * txDiffuse.Sample(samLinear, IN.tex);
}

