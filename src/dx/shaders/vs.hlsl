cbuffer FrameBuffer : register(b0) {
	float4x4 proj;
}

cbuffer ObjectBuffer : register(b1) {
	float4x4 world;
}

float4 main(float3 v : Position) : SV_Position {
	return mul(proj, mul(world, float4(v, 1.0)));
}
