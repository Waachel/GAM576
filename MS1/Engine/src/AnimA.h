//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIMA_H
#define ANIMA_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "SkeletonA.h"

namespace Azul
{
#define NUM_BONES 12

	class AnimA
	{
	public:
		AnimA();
		AnimA(const AnimA &) = delete;
		AnimA &operator = (const AnimA &) = delete;
		~AnimA();

		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		static void PoseSkeleton(GameObjectAnimSkeleton *node);

	private:
		Clip       *pClip;
		Bone       *poBoneResult;
		SkeletonA   *poSkeleton;
		int        numBones;
	};

}

#endif

// --- End of File ---