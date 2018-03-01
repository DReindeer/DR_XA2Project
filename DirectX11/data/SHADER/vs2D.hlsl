// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : ���_�V�F�[�_�[
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

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	matrix View;		//�r���[�ϊ��s��
	matrix Projection;	//�����ˉe�ϊ��s��
}

cbuffer CBufferModel : register(b1)
{
	matrix World;		//���[���h�ϊ��s��
}


VS_OUT main(VS_IN IN)
{
	VS_OUT OUT; 

	//���_����
	OUT.pos = mul(IN.pos, World);
	OUT.pos = mul(OUT.pos, Projection);
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	return OUT;
}