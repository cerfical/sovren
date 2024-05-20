cbuffer ObjectBuffer : register(b0) {
	float4x4 wvp;
}

float4 main(float3 v : Position) : SV_Position {
	return mul(wvp, float4(v, 1.0));
}
