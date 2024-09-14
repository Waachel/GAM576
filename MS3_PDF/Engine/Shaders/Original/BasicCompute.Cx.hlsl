//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#pragma pack_matrix( row_major )

//--------------------------------------------------------------------------------------
// File: BasicCompute11.hlsl

// This file contains the Compute Shader to perform array A + array B
//--------------------------------------------------------------------------------------

struct BufType
{
    int i;
    float f;
};

// ---------------------------------------------
// tx - t is shader resource views (SRV), x - slot
// ---------------------------------------------

StructuredBuffer<BufType> Buffer0 : register(t0); // slot 0  (SRVBufferSlot::InputA)
StructuredBuffer<BufType> Buffer1 : register(t1); // slot 1  (SRVBufferSlot::InputB)

// ---------------------------------------------
// ux - u is  unordered access views (UAV), x - slot 
// ---------------------------------------------
RWStructuredBuffer<BufType> BufferOut : register(u0); // slot 0  (UAVBufferSlot::OutputA)

[numthreads(1, 1, 1)]
void BasicCompute_Main(uint3 DTid : SV_DispatchThreadID)
{
    BufferOut[DTid.x].i = Buffer0[DTid.x].i + Buffer1[DTid.x].i;
    BufferOut[DTid.x].f = Buffer0[DTid.x].f + Buffer1[DTid.x].f;
}

// --- End of File ---
