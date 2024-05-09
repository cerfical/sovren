cbuffer FrameBuffer : register(b0) {
	float4x4 proj;
}

float4 main(float3 v : Position) : SV_Position {
	return mul(proj, float4(v, 1.0));
}
