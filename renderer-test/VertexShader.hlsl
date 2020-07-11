struct VSOut {
	float3 color : Color;
	float4 pos: SV_Position;
};


VSOut main(float2 position : Position, float3 color : Color) {
	VSOut vso;
	vso.pos = float4(position.x, position.y, 0.0f, 1.0f);
	vso.color = color;
	return vso;
}