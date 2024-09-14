//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_H
#define ANIM_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{
	class Clip;
	class Mixer;

	class Anim
	{
	public:
		Anim(Clip *pClip, Mixer *pMixer);

		Anim() = delete;
		Anim(const Anim &) = delete;
		Anim &operator = (const Anim &) = delete;
		~Anim();

		void ChangeClip(Clip::Name clipName);

		Anim(Clip::Name clipName);
		void Animate(AnimTime tCurr);
		AnimTime FindMaxTime();

		Clip* pClip;
	private:
		
		Mixer *pMixer;
	};

}

#endif

// --- End of File ---