//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_BASIC_COMPUTE_H
#define SHADER_OBJECT_BASIC_COMPUTE_H

#include "ShaderObject.h"

namespace Azul
{
	class ShaderObject_BasicCompute : public ShaderObject
	{
	public:

	public:
		ShaderObject_BasicCompute() = delete;
		ShaderObject_BasicCompute(const ShaderObject_BasicCompute &) = delete;
		ShaderObject_BasicCompute &operator = (const ShaderObject_BasicCompute &) = delete;
		virtual ~ShaderObject_BasicCompute();

		ShaderObject_BasicCompute(ShaderObject_BasicCompute::Name _name);


		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------


	};
}

#endif

// --- End of File ---
