// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �e�N�X�`�������s�N�Z���V�F�[�_�[
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
	float4 col2: COLOR2;
};

//�O���[�o��
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//�s�N�Z���V�F�[�_�[
float4 main(PS_IN IN) : SV_Target
{
	PS_IN OUT;

	OUT.col = saturate(IN.col2) * IN.col;

	return OUT.col;
}

