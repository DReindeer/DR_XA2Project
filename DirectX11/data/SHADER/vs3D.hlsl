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
	float3 nor : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 col2: COLOR2;		//�v�Z���ʂ��s�N�Z���V�F�[�_�[�ɑ���p
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
	float4 Light;		//���C�g
}

VS_OUT main(VS_IN IN)
{
	VS_OUT OUT;

	//���_����
	OUT.pos = mul(IN.pos, World);
	OUT.pos = mul(OUT.pos, View);
	OUT.pos = mul(OUT.pos, Projection);
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	//��������
	float3 L = normalize(Light.xyz);

	//�@���x�N�g������
	float3 nor;
	//���[���h�ϊ�
	nor = mul(IN.nor, (float3x3)World);
	nor = normalize(nor);

	//�����Ɩ@���̓��ς����߂�
	OUT.col2 = max(0.01, dot(nor, L));

	OUT.col2.a = 1.0f;	//���l�𖳌��ɂ���

	return OUT;
}