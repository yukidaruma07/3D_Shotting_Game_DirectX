struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET {
    float r = 0.299 * input.color.r;
    float g = 0.587 * input.color.g;
    float b = 0.114 * input.color.b;
    float gray = r + g + b;
	return float4(gray, gray, gray, 1.0f);
}