//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMB_H
#define ANIMB_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "SkeletonB.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimB
	{
	public:
		AnimB();
		AnimB(const AnimB &) = delete;
		AnimB &operator = (const AnimB &) = delete;
		~AnimB();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		SkeletonB   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---