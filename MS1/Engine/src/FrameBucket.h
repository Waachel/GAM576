//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FRAME_BUCKET
#define FRAME_BUCKET

#include "AnimTimer.h"
#include "MathEngine.h"

namespace Azul
{

	struct Bone : public Align16
	{
		Vec3  T;
		Quat  Q;
		Vec3  S;
	};

	struct FrameBucket
	{
		FrameBucket  *nextBucket;
		FrameBucket  *prevBucket;
		AnimTime      KeyTime;
		Bone         *poBone;
		char          pad[4];
	};

}

#endif

//--- End of File ---