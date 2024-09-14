//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "Clip.h"
#include "ClipMan.h"

namespace Azul
{
	Anim::Anim(Clip *_pClip, Mixer *_pMixer)
		: pClip{_pClip},
		pMixer{_pMixer}
	{
		assert(pClip);
		assert(pMixer);
	}

	Anim::~Anim()
	{

	}

	void Anim::ChangeClip(Clip::Name clipName)
	{
		this->pClip = ClipMan::Find(clipName);
		assert(this->pClip);
	}

	AnimTime Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	void Anim::Animate(AnimTime tCurr)
	{
		this->pClip->AnimateBones(tCurr, this->pMixer);
	}

}

//--- End of File ----