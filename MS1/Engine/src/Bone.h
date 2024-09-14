//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BONE_H
#define BONE_H

#include "MathEngine.h"

namespace Azul
{
	struct Bone : public Align16
	{
		Vec3  T;
		Quat  Q;
		Vec3  S;
	};
}

#endif

// --- End of File ---
