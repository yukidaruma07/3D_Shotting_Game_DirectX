Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix wvpMat;
    float4 diffUse;
    bool isTexture;
}

struct PSInput {
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput IN) : SV_Target
{
    float4 diffUse_ = { 1.0, 0.0, 0.0, 1.0};
    if (isTexture)
    {
        diffUse_ = texture0.Sample(sampler0, IN.uv);

    }
    else
    {
        diffUse_ = diffUse;
    }
    
    return diffUse_;
}