//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#pragma pack_matrix( row_major )

// ------------------------------------------
// bx - b is constant buffer, x - slot
// ------------------------------------------

cbuffer AA0: register(b0)  // slot 0  (ConstantBufferSlot::Projection)
{
	matrix projectionMatrix;
}

cbuffer AA1 : register(b1) // slot 1 (ConstantBufferSlot::View)
{
	matrix viewMatrix;
}

cbuffer AA2 : register(b2) // slot 2 (ConstantBufferSlot::World)
{
	matrix worldMatrix;
}

cbuffer AA3 : register(b5) // slot 3 (ConstantBufferSlot::UV_Correction)
{
	matrix uvMatrix;
}

// ------------------------------------------------------
// Sematics Vertex Input:
//      POSITION - used in vertex layout description
//      COLOR - used in vertex layout description
// ------------------------------------------------------

struct AppData_pos  //  vertex constant buffer, slot 0 (VertexSlot::Position)
{
	float3 position : POSITION;
};

struct AppData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
{
	float2 tex: TEXCOORD;  // uv location
};

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage 
//      Note: output layout needs to be the same for input input pixel
// ------------------------------------------------------------
struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
	float4 position : SV_POSITION;
    matrix uvMat : MONKEY;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput Sprite_Main(AppData_pos inPos, AppData_tex inTex)
{
	VertexShaderOutput outValue;

	// Mat = World * View * Proj
	matrix Mat = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
	outValue.position = mul(float4(inPos.position, 1.0f), Mat);
	outValue.tex = inTex.tex;

	// pass the uv Matrix to pixel shader
	outValue.uvMat = uvMatrix;

	return outValue;
}

// --- End of File ---
