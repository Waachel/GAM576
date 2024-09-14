//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// ------------------------------------------
// tx - t is texture buuffer, x - slot
// ------------------------------------------
Texture2D mainTexture : register(t0);

// ------------------------------------------
// sx - s is sampler, x - slot
// ------------------------------------------
SamplerState aSampler : register(s0);

// -------------------------------------
//  Pixel Shader Input Sematics
//        TEXCOORD - uv tex coord
//      Note: output layout needs to be the same for input input pixel
// -------------------------------------
struct PixelShaderInput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
    matrix uvMat : MONKEY;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 Sprite_Main(PixelShaderInput inData ) : SV_TARGET
{
	// correct the UV coordinates, originally its (0,0 - 1,1)
	// scale and translate to the correct sub image on the texture

	float4 uv_vec;
	uv_vec.xy =  inData.tex.xy;
	uv_vec.z = 0.0f;
	uv_vec.w = 1.0f;

    float4 uv_corrected = mul(inData.uvMat, uv_vec);

    return mainTexture.Sample(aSampler, uv_corrected.xy);
}


// --- End of File ---

