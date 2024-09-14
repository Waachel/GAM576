//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_BasicCompute.h"
#include "Mesh.h"

// User generated headers... 
#include "BasicCompute.Cx.h"

namespace Azul
{
	ShaderObject_BasicCompute::~ShaderObject_BasicCompute()
	{

	}

	ShaderObject_BasicCompute::ShaderObject_BasicCompute(ShaderObject_BasicCompute::Name _name)
		: ShaderObject(_name)
	{
		this->CreateComputeShader(g_BasicCompute_CxShader, sizeof(g_BasicCompute_CxShader));
	}

}

// --- End of File ---
