//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "AnimTime.h"
#include "FontSprite.h"

namespace Azul
{
	class Anim;
	class TimerController;

	class AnimController
	{
	public:
		AnimController(Anim *pAnim, AnimTime delta);
		void ChangeAnim(bool wantsFirstAnim, FontSprite* f);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();

		TimerController* poTimerControl;
		Anim* poAnim;


	private:
		
		

	};
}

#endif

// --- End of File ---
